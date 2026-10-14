#ifndef FIRMWARESERVICE_H
#define FIRMWARESERVICE_H

#include "OtaProtocol.h"
#include "FirmwareProtocol.h"
#include "Bluetooth.h"
#include <memory> 

// ═══════════════════════════════════════════════════════════════
// PROTOCOL UUIDs
// ═══════════════════════════════════════════════════════════════
#define OTA_SERVICE_UUID "85564e0a-542d-4967-9d09-892b864a1b46"
#define OTA_CONTROL_CHAR_UUID "3cf9ef55-5c41-4801-b2d7-cd34c3c06f14"  // Write + Indicate
#define OTA_DATA_CHAR_UUID "4cdc8cf8-55a7-4cd7-882d-aee962c537c1"     // Write No Response
#define OTA_STATUS_CHAR_UUID "b8423123-1aa6-452a-93dc-4eea995fcb9e"   // Notify

// Forward declarations
class ControlCharCallbacks;
class DataCharCallbacks;
class BleOtaTx;

// ═══════════════════════════════════════════════════════════════
// BLE-TO-PROTOCOL TRANSMISSION IMPLEMENTATION
// ═══════════════════════════════════════════════════════════════
// This class implements the IOtaTx interface for BLE transport
class BleOtaTx : public IOtaTx {
private:
    NimBLECharacteristic* _controlChar;
    NimBLECharacteristic* _statusChar;
    NimBLEServer* _server;
    uint16_t _mtu;

public:
    BleOtaTx(NimBLECharacteristic* controlChar, NimBLECharacteristic* statusChar, NimBLEServer* server);
    
    // IOtaTx interface implementation
    void sendControl(const uint8_t* data, size_t len) override;
    void sendStatus(const uint8_t* data, size_t len) override;
    uint16_t negotiatedMtu() const override;
    bool isConnected() const override;
    
    // BLE-specific methods
    void updateMtu(uint16_t newMtu);
};

// ═══════════════════════════════════════════════════════════════
// FIRMWARE SERVICE (BLE-ONLY)
// ═══════════════════════════════════════════════════════════════
// This service handles BLE communication and delegates protocol logic to OtaProtocol
// No significant changes for Sliding Window - still just a pipe!
class FirmwareService {
private:
    // BLE Components
    NimBLEServer* _server;
    NimBLEService* _service;
    NimBLECharacteristic* _controlChar;
    NimBLECharacteristic* _dataChar;
    NimBLECharacteristic* _statusChar;
    
    // Protocol engine (handles all OTA logic including Sliding Window)
    std::unique_ptr<OtaProtocol> _protocol;
    
    // Transmission interface
    std::unique_ptr<BleOtaTx> _tx;
    
public:
    // Constructor/Destructor
    FirmwareService();
    ~FirmwareService();
    
    // Lifecycle
    void init(NimBLEServer* server);

    // Access to protocol (for debugging/monitoring)
    OtaProtocol* getProtocol() const { return _protocol.get(); }
    
    // Connection events
    void onConnect();
    void onDisconnect();
    void onMtuChanged(uint16_t newMtu);
    
    // Internal handlers (called by BLE callbacks)
    void handleControlWrite(const uint8_t* data, size_t len);
    void handleDataWrite(uint8_t* data, size_t len);
};

// ═══════════════════════════════════════════════════════════════
// BLE CHARACTERISTIC CALLBACKS
// ═══════════════════════════════════════════════════════════════
class ControlCharCallbacks : public NimBLECharacteristicCallbacks {
private:
    FirmwareService* _service;

public:
    ControlCharCallbacks(FirmwareService* service);
    void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override;
};

class DataCharCallbacks : public NimBLECharacteristicCallbacks {
private:
    FirmwareService* _service;

public:
    DataCharCallbacks(FirmwareService* service);
    void onWrite(NimBLECharacteristic* pCharacteristic, NimBLEConnInfo& connInfo) override;
};

// ═══════════════════════════════════════════════════════════════
// GLOBAL API FUNCTIONS
// ═══════════════════════════════════════════════════════════════
void initFirmwareService(NimBLEServer* server);
void onOtaConnect();
void onOtaDisconnect();
void onOtaMtuChanged(uint16_t newMtu);

// Global instance
extern FirmwareService otaService;

#endif // FIRMWARESERVICE_H
