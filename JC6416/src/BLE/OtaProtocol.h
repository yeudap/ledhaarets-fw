// OtaProtocol.h
// Sliding Window OTA Protocol with Cumulative ACK and Dynamic Credits
#ifndef OTA_PROTOCOL_H
#define OTA_PROTOCOL_H

#include "FirmwareProtocol.h"  // ← Uses shared protocol definitions
#include <Update.h>
#include <Arduino.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/queue.h>

// ═══════════════════════════════════════════════════════════════
// QUEUE PACKET STRUCTURE
// ═══════════════════════════════════════════════════════════════
// This is what we store in the FreeRTOS queue
// BLE callback pushes these, worker task pops and writes to flash
struct QueuedPacket {
    uint16_t seq;           // Sequence number
    uint16_t len;           // Data length
    uint8_t* data;          // Pointer to heap-allocated data
    
    // Constructor
    QueuedPacket() : seq(0), len(0), data(nullptr) {}
    
    // Helper to allocate and copy data
    bool allocate(uint16_t dataLen) {
        if (data) free(data);  // Clean up old allocation
        data = (uint8_t*)malloc(dataLen);
        if (!data) {
            Serial.printf("[%s] ✗ Failed to allocate %d bytes for packet\n", OTA_TAG, dataLen);
            return false;
        }
        len = dataLen;
        return true;
    }
    
    // Helper to free data
    void release() {
        if (data) {
            free(data);
            data = nullptr;
        }
        len = 0;
    }
};

// ═══════════════════════════════════════════════════════════════
// SLIDING WINDOW OTA PROTOCOL ENGINE
// ═══════════════════════════════════════════════════════════════
// Event-driven protocol handler with:
// - Sliding window (W packets in flight)
// - Cumulative ACK (time-based OR count-based)
// - Dynamic credits based on queue occupancy
// - Queue + Worker Task for flash writes
// - NACK recovery with credits=0
class OtaProtocol {
private:
    // Transmission interface
    IOtaTx* _tx;  // ← IOtaTx defined in FirmwareProtocol.h
    
    // Protocol state
    OtaState _state;
    uint32_t _totalSize;
    uint32_t _receivedBytes;        // Bytes successfully written to flash
    uint16_t _nextExpectedSeq;      // Next sequence we expect to receive
    uint16_t _chunkSize;
    uint8_t  _windowSize;           // Window size for this session
    uint32_t _finishCompletedTime;  // Track when COMPLETE_OK was sent

    // Credits and flow control
    uint8_t _currentCredits;        // Current credits available to client
    bool _creditsEnabled;           // False during NACK recovery
    
    // Queue and task for async flash writes
    QueueHandle_t _packetQueue;     // FreeRTOS queue for incoming packets
    TaskHandle_t _workerTask;       // Worker task handle
    volatile bool _taskRunning;     // Task lifecycle flag
    
    // ACK timing - dual trigger (time OR count, whichever comes first)
    uint32_t _lastAckTime;          // Last time we sent ACK
    uint16_t _packetsProcessedSinceAck; // Packets written since last ACK
    uint32_t _lastDataTime;      // Last time we received any DATA packet

    // NACK recovery
    uint16_t _lastNackedSeq;        // Last sequence number we sent NACK for
    uint8_t _nackCount;             // Number of NACKs sent for current recovery
    static const uint8_t MAX_NACK_RETRIES = 5;
    
    // MD5 verification
    uint8_t _expectedMd5[33];       // 32 hex chars + null terminator;
    
    // Statistics and performance tracking
    uint32_t _sessionStartMs;       // Session start time (millis)
    uint64_t _totalWriteTimeUs;     // Total time spent in Update.write (microseconds)
    uint32_t _maxWriteTimeUs;       // Maximum single write time (microseconds)
    uint32_t _totalPacketsWritten;  // Total packets written to flash

    bool _endScreenShown;                    // הודעת-סיום כבר צוירה בסשן הנוכחי
    void _fatalRestart(uint8_t errorCode);   // כשל ללא התאוששות → הודעה + אתחול

public:
    // Constructor/Destructor
    explicit OtaProtocol(IOtaTx* tx);
    ~OtaProtocol();
    
    // Lifecycle
    void begin();
    
    // Connection events
    void onConnectionLost();
    void onMtuChanged(uint16_t newMtu);
    
    // Protocol events (called by service layer)
    void onControlCommand(uint8_t cmd, const uint8_t* payload, size_t len);
    void onDataSeq(uint8_t* data, size_t len);  // BLE callback → queue push
    
    // Status/debugging
    void printStatus();
    OtaState getCurrentState() const { return _state; }
    uint32_t getProgress() const { return _receivedBytes; }
    uint32_t getTotalSize() const { return _totalSize; }
    uint16_t getQueuedPackets() const;
    uint8_t getCurrentCredits() const { return _currentCredits; }

private:
    // Command handlers
    void _handleHello();
    void _handleStart(const uint8_t* payload, size_t len);
    void _handleFinish();
    void _handleAbort();
    void _handleClientFinalOk();
    
    // Queue management
    bool _pushToQueue(uint16_t seq, uint8_t* data, uint16_t len);
    
    // Worker task (static wrapper + instance method)
    static void _workerTaskWrapper(void* param);
    void _workerTaskLoop();
    
    // ACK and credits management
    void _sendAckWindow();
    uint8_t _calculateCredits();    // Dynamic credits based on queue occupancy
    void _checkAckTiming();         // Time-based OR count-based trigger
    
    // NACK and error handling
    void _sendNack(uint16_t expectedSeq);
    void _sendError(uint8_t errorCode);
    
    // Verification
    bool _verifyMd5();
    
    // State management
    void _resetToIdle();
    void _startWorkerTask();
    void _stopWorkerTask();
};

#endif // OTA_PROTOCOL_H
