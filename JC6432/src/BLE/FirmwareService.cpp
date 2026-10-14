#include "FirmwareService.h"

// Global instance
FirmwareService otaService;

// ═══════════════════════════════════════════════════════════════
// BLEOTA TX IMPLEMENTATION
// ═══════════════════════════════════════════════════════════════

BleOtaTx::BleOtaTx(NimBLECharacteristic* controlChar, NimBLECharacteristic* statusChar, NimBLEServer* server)
    : _controlChar(controlChar)
    , _statusChar(statusChar)
    , _server(server)
    , _mtu(DEFAULT_MTU)
{
}

void BleOtaTx::sendControl(const uint8_t* data, size_t len) {
    if (!_controlChar || !isConnected()) {
        Serial.printf("[%s] Cannot send control - not connected\n", OTA_TAG);
        return;
    }
    
    _controlChar->setValue((uint8_t*)data, len);
    // _controlChar->indicate();
    _controlChar->notify();
}

void BleOtaTx::sendStatus(const uint8_t* data, size_t len) {
    if (!_statusChar || !isConnected()) {
        Serial.printf("[%s] Cannot send status - not connected\n", OTA_TAG);
        return;
    }
    
    _statusChar->setValue((uint8_t*)data, len);
    _statusChar->notify();
}

uint16_t BleOtaTx::negotiatedMtu() const {
    return _mtu;
}

bool BleOtaTx::isConnected() const {
    return _server && _server->getConnectedCount() > 0;
}

void BleOtaTx::updateMtu(uint16_t newMtu) {
    _mtu = newMtu;
    // Serial.printf("[%s] MTU updated to %d\n", OTA_TAG, _mtu);
}

// ═══════════════════════════════════════════════════════════════
// FIRMWARE SERVICE IMPLEMENTATION
// ═══════════════════════════════════════════════════════════════

FirmwareService::FirmwareService() 
    : _server(nullptr)
    , _service(nullptr)
    , _controlChar(nullptr)
    , _dataChar(nullptr)
    , _statusChar(nullptr)
{
}

FirmwareService::~FirmwareService() {
    // Protocol will handle cleanup in its destructor
}

void FirmwareService::init(NimBLEServer* server) {
    _server = server;
    
    // Create BLE service
    _service = _server->createService(OTA_SERVICE_UUID);
    
    // ═══════════════════════════════════════════════════════════════
    // Control characteristic (Write + Indicate)
    // Used for: HELLO, START, FINISH, ABORT commands
    // Responses: HELLO_OK, START_OK, COMPLETE_OK, ERROR, NACK
    // ═══════════════════════════════════════════════════════════════
    _controlChar = _service->createCharacteristic(
        OTA_CONTROL_CHAR_UUID,
        NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::INDICATE | NIMBLE_PROPERTY::NOTIFY
    );
    _controlChar->setCallbacks(new ControlCharCallbacks(this));

    // ═══════════════════════════════════════════════════════════════
    // Data characteristic (Write No Response)
    // Used for: Firmware data packets (seq + len + data)
    // ═══════════════════════════════════════════════════════════════
    _dataChar = _service->createCharacteristic(
        OTA_DATA_CHAR_UUID,
        NIMBLE_PROPERTY::WRITE_NR
    );
    _dataChar->setCallbacks(new DataCharCallbacks(this));
    
    // ═══════════════════════════════════════════════════════════════
    // Status characteristic (Notify)
    // Used for: ACK_WINDOW (cumulative ACK with credits)
    // ═══════════════════════════════════════════════════════════════
    _statusChar = _service->createCharacteristic(
        OTA_STATUS_CHAR_UUID,
        NIMBLE_PROPERTY::NOTIFY
    );

    // ═══════════════════════════════════════════════════════════════
    // Start the service
    // ═══════════════════════════════════════════════════════════════
    // _service->start();
    
    // ═══════════════════════════════════════════════════════════════
    // Create transmission interface
    // ═══════════════════════════════════════════════════════════════
    _tx = std::make_unique<BleOtaTx>(_controlChar, _statusChar, _server);
    
    // ═══════════════════════════════════════════════════════════════
    // Create protocol engine (Sliding Window OTA)
    // ═══════════════════════════════════════════════════════════════
    _protocol = std::make_unique<OtaProtocol>(_tx.get());
    _protocol->begin();

    // Debug
    Serial.printf("[%s] ✓ NimBLE OTA Service initialized\n", OTA_TAG);
}

// ═══════════════════════════════════════════════════════════════
// CONNECTION EVENT HANDLERS
// ═══════════════════════════════════════════════════════════════
// These are called by the main BLE server callbacks

void FirmwareService::onConnect() {
    // Serial.printf("[%s] Client connected\n", OTA_TAG);
    // Protocol doesn't need special connect handling
}

void FirmwareService::onDisconnect() {
    // Serial.printf("[%s] Client disconnected\n", OTA_TAG);
    if (_protocol) {
        _protocol->onConnectionLost();
    }
}

void FirmwareService::onMtuChanged(uint16_t newMtu) {
    // Serial.printf("[%s] MTU negotiation: %d bytes\n", OTA_TAG, newMtu);
    if (_tx) {
        _tx->updateMtu(newMtu);
    }
    if (_protocol) {
        _protocol->onMtuChanged(newMtu);
    }
}

// ═══════════════════════════════════════════════════════════════
// BLE WRITE HANDLERS
// ═══════════════════════════════════════════════════════════════
// These are called by the characteristic callbacks

void FirmwareService::handleControlWrite(const uint8_t* data, size_t len) {
    if (!_protocol || len == 0) {
        Serial.printf("[%s] Invalid control write\n", OTA_TAG);
        return;
    }
    
    // Extract command (first byte)
    uint8_t cmd = data[0];
    
    // Extract payload (rest of bytes, if any)
    const uint8_t* payload = (len > 1) ? data + 1 : nullptr;
    size_t payloadLen = (len > 1) ? len - 1 : 0;
    
    // Delegate to protocol
    _protocol->onControlCommand(cmd, payload, payloadLen);
}

void FirmwareService::handleDataWrite(uint8_t* data, size_t len) {
    if (!_protocol || len == 0) {
        Serial.printf("[%s] Invalid data write\n", OTA_TAG);
        return;
    }
    
    // Delegate to protocol
    // The protocol will handle:
    // 1. Parsing seq + len + data
    // 2. Checking sequence (for gaps)
    // 3. Pushing to queue (worker task writes to flash)
    _protocol->onDataSeq(data, len); 
}

// ═══════════════════════════════════════════════════════════════
// BLE CHARACTERISTIC CALLBACKS
// ═══════════════════════════════════════════════════════════════

ControlCharCallbacks::ControlCharCallbacks(FirmwareService* service) 
    : _service(service) 
{
}

void ControlCharCallbacks::onWrite(
    NimBLECharacteristic* ch,
    NimBLEConnInfo& connInfo
) {
    std::string value = ch->getValue();

    if (value.empty()) return;

    _service->handleControlWrite(
        reinterpret_cast<const uint8_t*>(value.data()),
        value.length()
    );
}

DataCharCallbacks::DataCharCallbacks(FirmwareService* service) 
    : _service(service) 
{
}

void DataCharCallbacks::onWrite(
    NimBLECharacteristic* ch,
    NimBLEConnInfo& connInfo
) {
    std::string value = ch->getValue();

    if (value.empty()) return;

    _service->handleDataWrite(
        reinterpret_cast<uint8_t*>(value.data()),
        value.length()
    );
}

// ═══════════════════════════════════════════════════════════════
// GLOBAL API FUNCTIONS
// ═══════════════════════════════════════════════════════════════
// These are called from main.cpp or BLE server callbacks

void initFirmwareService(NimBLEServer* server) {
    otaService.init(server);
}

void onOtaConnect() {
    otaService.onConnect();
}

void onOtaDisconnect() {
    otaService.onDisconnect();
}

void onOtaMtuChanged(uint16_t newMtu) {
    otaService.onMtuChanged(newMtu);
}

// ═══════════════════════════════════════════════════════════════
// NOTES ON SLIDING WINDOW SUPPORT
// ═══════════════════════════════════════════════════════════════
// 
// This service layer didn't need significant changes for Sliding Window!
// Why? Because it's just a "pipe" that passes data between BLE and OtaProtocol.
// 
// The Sliding Window logic is entirely in OtaProtocol.cpp:
// - Queue management for incoming packets
// - Worker task for flash writes
// - Cumulative ACK with dynamic credits
// - Time-based + count-based ACK triggers
// 
// This service just delivers those ACKs and NACKs through BLE.
// Beautiful separation of concerns! 🎯
//
// ═══════════════════════════════════════════════════════════════
