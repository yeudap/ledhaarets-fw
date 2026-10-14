#ifndef FIRMWAREPROTOCOL_H
#define FIRMWAREPROTOCOL_H

#include <Arduino.h>
    
// ═══════════════════════════════════════════════════════════════
// LOGGING & DEBUG
// ═══════════════════════════════════════════════════════════════
#define OTA_TAG "OTA"

// Optional: Debug macros (uncomment if needed)
// #define OTA_DEBUG_ENABLED
#ifdef OTA_DEBUG_ENABLED
    #define OTA_DEBUG(...) Serial.printf("[%s] DEBUG: ", OTA_TAG); Serial.printf(__VA_ARGS__); Serial.println()
#else
    #define OTA_DEBUG(...)
#endif

// ═══════════════════════════════════════════════════════════════
// OPCODES - Client to Server (via control write)
// ═══════════════════════════════════════════════════════════════
#define CMD_HELLO           0x01    // Hello handshake
#define CMD_START           0x02    // Start OTA with size + MD5
#define CMD_FINISH          0x03    // Finish and verify
#define CMD_CLIENT_FINAL_OK 0x04    // Client confirms COMPLETE_OK received
#define CMD_ABORT           0x05    // Abort OTA

// Alternative names (for compatibility)
#define OP_HELLO            CMD_HELLO
#define OP_START            CMD_START
#define OP_FINISH           CMD_FINISH
#define OP_ABORT            CMD_ABORT

// ═══════════════════════════════════════════════════════════════
// STATUS CODES - Server to Client (via control notify)
// ═══════════════════════════════════════════════════════════════
#define STATUS_HELLO_OK     0x81    // Hello response with MTU
#define STATUS_START_OK     0x82    // Start confirmed with MTU + chunk size + window size
#define STATUS_COMPLETE_OK  0x83    // Verification and commit successful
#define STATUS_VERIFY_FAIL  0x84    // Verification failed
#define STATUS_ABORT_OK     0x85    // Abort acknowledged
#define STATUS_ACK_WINDOW   0x91    // NEW: Cumulative ACK with credits (status channel)
#define STATUS_UPDATE       0x93    // Progress update (status channel, deprecated)
#define STATUS_NACK         0x94    // NACK with expectedSeq (control channel)
#define STATUS_ERROR        0xE0    // Error response with error code

// ═══════════════════════════════════════════════════════════════
// ERROR CODES (for STATUS_ERROR)
// ═══════════════════════════════════════════════════════════════
#define ERR_BEGIN_FAILED        0x01    // Update.begin() failed
#define ERR_OTA_WRITE_FAILED    0x02    // Update.write() failed
#define ERR_SIZE_MISMATCH       0x03    // Received != expected size
#define ERR_MD5_MISMATCH        0x04    // MD5 verification failed
#define ERR_COMMIT_FAILED       0x05    // Update.end() commit failed
#define ERR_TIMEOUT             0x06    // Inactivity timeout
#define ERR_BAD_STATE           0x07    // Command in wrong state
#define ERR_INVALID_PARAMS      0x08    // Invalid parameters
#define ERR_SEQ_GAP             0x09    // Sequence gap (deprecated - now sends NACK)
#define ERR_WRITE_SHORT         0x0A    // Write operation incomplete
#define ERR_UNSUPPORTED_OTA     0x0B    // Unsupported command or operation

// ═══════════════════════════════════════════════════════════════
// OTA STATE MACHINE
// ═══════════════════════════════════════════════════════════════
enum OtaState : uint8_t {
    OTA_IDLE = 0,           // Ready for HELLO
    OTA_START = 1,          // HELLO received, ready for START
    OTA_IN_PROGRESS = 2,    // Data transfer in progress
    OTA_VERIFYING = 4,      // Verifying MD5 and committing
    OTA_DONE = 5,           // Successfully completed
    OTA_ERROR = 6           // Error state
};

// ═══════════════════════════════════════════════════════════════
// PROTOCOL CONFIGURATION - SLIDING WINDOW WITH CUMULATIVE ACK
// ═══════════════════════════════════════════════════════════════
#define DEFAULT_MTU                 517     // Requested MTU
#define CHUNK_SIZE_DEFAULT          244     // Default chunk size
#define SAFE_CHUNK_SIZE             244     // Fixed safe chunk size (never exceed)

// === Sliding Window Configuration ===
#define WINDOW_SIZE_DEFAULT         32      // Default window size (conservative)
#define WINDOW_SIZE_MAX             32      // Maximum window size
#define QUEUE_LENGTH                48      // Queue size for incoming packets

// === ACK Timing Configuration ===
#define ACK_INTERVAL_MS             50      // Send ACK every 80ms (time-based)
#define ACK_INTERVAL_PACKETS        32      // Or after processing N packets (whichever comes first)

// === Dynamic Credits Configuration ===
// Watermarks for dynamic credit calculation based on queue occupancy
#define QUEUE_HIGH_WATERMARK        36      // Queue 75% full → credits = 0
#define QUEUE_LOW_WATERMARK         12       // Queue 25% full → credits = full

// === Timeouts ===
#define PROGRESS_INTERVAL_MS        1000    // Progress update interval (1 second, legacy)

// === Task Configuration ===
#define OTA_TASK_STACK_SIZE         4096    // Stack size for OTA worker task
#define OTA_TASK_PRIORITY           10      // Task priority (high)
#define OTA_TASK_CORE               1       // Run on core 1 (app core)

// ═══════════════════════════════════════════════════════════════
// HELPER MACROS
// ═══════════════════════════════════════════════════════════════

// Calculate chunk size from MTU
// MTU - 3 (ATT overhead) - 4 (packet header: seq + len) = data size
#define CALCULATE_CHUNK_SIZE(mtu) ((mtu) - 3 - 4)

// Safe chunk size calculation (never exceed SAFE_CHUNK_SIZE)
#define SAFE_CALCULATE_CHUNK_SIZE(mtu) \
    (((mtu) - 3 - 4) > SAFE_CHUNK_SIZE ? SAFE_CHUNK_SIZE : ((mtu) - 3 - 4))

// Validate MD5 hex character
#define IS_VALID_MD5_CHAR(c) (((c) >= '0' && (c) <= '9') || \
                             ((c) >= 'a' && (c) <= 'f') || \
                             ((c) >= 'A' && (c) <= 'F'))

// ═══════════════════════════════════════════════════════════════
// PROTOCOL STRUCTURES (Little-Endian)
// ═══════════════════════════════════════════════════════════════

// --- Data Packet (Client → Server via data write) ---
// Format: seq(2) + len(2) + data[len]
// Total: 4 + len bytes
typedef struct __attribute__((packed)) {
    uint16_t seq;           // Sequence number (starts from 0)
    uint16_t len;           // Data length
    uint8_t data[];         // Actual data (variable length)
} DataSeqPacket;

// --- HELLO_OK Response (Server → Client via control) ---
// Format: opcode(1) + mtu(2)
// Total: 3 bytes
typedef struct __attribute__((packed)) {
    uint16_t mtu;           // Negotiated MTU
} HelloOkPacket;

// --- START_OK Response (Server → Client via control) ---
// Format: opcode(1) + mtu(2) + chunk_size(2) + window_size(1) + initial_credits(1)
// Total: 7 bytes
typedef struct __attribute__((packed)) {
    uint16_t mtu;           // Negotiated MTU
    uint16_t chunk_size;    // Chunk size to be used (locked until end)
    uint8_t  window_size;   // Window size for this session
    uint8_t  initial_credits; // Initial credits available
} StartOkPacket;

// --- START Command Payload (Client → Server via control) ---
// Format: opcode(1) + total_size(4) + md5_hex[32]
// Total: 37 bytes
typedef struct __attribute__((packed)) {
    uint32_t total_size;    // Total firmware size
    uint8_t  md5_hex[32];   // MD5 hash as ASCII hex (no null terminator)
} StartCommandPayload;

// --- ACK Window Packet (Server → Client via status notify) - NEW ---
// Format: opcode(1) + next_expected_seq(2) + credits(1) + received_bytes(4)
// Total: 8 bytes
// This is sent periodically (time-based OR count-based, whichever comes first)
typedef struct __attribute__((packed)) {
    uint16_t next_expected_seq; // Everything before this was received OK (cumulative ACK)
    uint8_t  credits;           // How many packets client can send now
    uint32_t received_bytes;    // Total bytes written to flash (for progress sync)
} AckWindowPacket;

// --- Progress Update (Server → Client via status notify) ---
// Format: opcode(1) + received_bytes(4) + total_size(4) + percent(1)
// Total: 10 bytes
// NOTE: This is DEPRECATED in favor of AckWindowPacket, kept for compatibility
typedef struct __attribute__((packed)) {
    uint32_t received_bytes;    // Bytes received so far
    uint32_t total_size;        // Total firmware size
    uint8_t  percent;          // Progress 0-100%
} ProgressUpdatePacket;

// --- NACK Packet (Server → Client via control) ---
// Format: opcode(1) + expectedSeq(2) + receivedBytes(4)
// Total: 7 bytes
typedef struct __attribute__((packed)) {
    uint16_t expectedSeq;       // Sequence number server is waiting for
    uint32_t receivedBytes;     // Bytes received so far (for progress sync)
} NackPacket;

// --- Error Packet (Server → Client via control) ---
// Format: opcode(1) + errorCode(1)
// Total: 2 bytes
typedef struct __attribute__((packed)) {
    uint8_t errorCode;          // Error code from ERR_* defines
} ErrorPacket;

// ═══════════════════════════════════════════════════════════════
// CHANNEL RESPONSIBILITIES
// ═══════════════════════════════════════════════════════════════
//
// Control Channel (write + notify):
//   Client → Server: HELLO, START, FINISH, ABORT
//   Server → Client: HELLO_OK, START_OK, COMPLETE_OK, VERIFY_FAIL, 
//                     ABORT_OK, NACK, ERROR
//
// Status Channel (notify only):
//   Server → Client: ACK_WINDOW (periodic cumulative ACK with credits)
//                     PROGRESS_UPDATE (deprecated, for compatibility)
//
// Data Channel (write no response):
//   Client → Server: DATA packets (seq + len + data)
//
// ═══════════════════════════════════════════════════════════════

// ═══════════════════════════════════════════════════════════════
// HELPER FUNCTIONS
// ═══════════════════════════════════════════════════════════════

// Get error message string for logging
inline const char* getErrorString(uint8_t errorCode) {
    switch (errorCode) {
        case ERR_BEGIN_FAILED:      return "BEGIN_FAILED";
        case ERR_OTA_WRITE_FAILED:  return "WRITE_FAILED";
        case ERR_SIZE_MISMATCH:     return "SIZE_MISMATCH";
        case ERR_MD5_MISMATCH:      return "MD5_MISMATCH";
        case ERR_COMMIT_FAILED:     return "COMMIT_FAILED";
        case ERR_TIMEOUT:           return "TIMEOUT";
        case ERR_BAD_STATE:         return "BAD_STATE";
        case ERR_INVALID_PARAMS:    return "INVALID_PARAMS";
        case ERR_SEQ_GAP:           return "SEQ_GAP (deprecated)";
        case ERR_WRITE_SHORT:       return "WRITE_SHORT";
        case ERR_UNSUPPORTED_OTA:   return "UNSUPPORTED_OTA";
        default:                    return "UNKNOWN";
    }
}

// Get state name string for logging
inline const char* getStateName(OtaState state) {
    switch (state) {
        case OTA_IDLE:          return "IDLE";
        case OTA_START:         return "START";
        case OTA_IN_PROGRESS:   return "IN_PROGRESS";
        case OTA_VERIFYING:     return "VERIFYING";
        case OTA_DONE:          return "DONE";
        case OTA_ERROR:         return "ERROR";
        default:                return "UNKNOWN";
    }
}

// Abstract interface for sending responses to client
class IOtaTx {
public:
    virtual ~IOtaTx() = default;
    
    // Send control indication (commands, responses, errors)
    virtual void sendControl(const uint8_t* data, size_t len) = 0;
    
    // Send status notification (ACK_WINDOW, STATUS_UPDATE - for UI)
    virtual void sendStatus(const uint8_t* data, size_t len) = 0;
    
    // Connection info
    virtual uint16_t negotiatedMtu() const = 0;
    virtual bool isConnected() const = 0;
};

#endif // FIRMWAREPROTOCOL_H
