// OtaProtocol.cpp
// Sliding Window OTA Protocol with Cumulative ACK and Dynamic Credits
// Queue-based architecture with worker task for flash writes
//
// ─── נקודות תצוגה על מסך השעון (4 הודעות סטטוס) ─────────────────
// כל התצוגה מרוכזת בחמש פונקציות hook בתחתית הקובץ (DISPLAY HOOKS):
//   _showUpdateStartScreen()      נקודה 1: מעבר למסך העדכון
//   _showUpdateProgress(percent)  נקודה 2: התקדמות (קיים — הועבר לפונקציה)
//   _showUpdateSuccessScreen()    נקודה 3: הושלם, מאתחל (קיים — הועבר)
//   _showUpdateCancelledScreen()  נקודה 4א: בוטל, חזרה למסך הראשי
//   _showUpdateFailedScreen(willRestart)
//                                 נקודה 4ב (false): נכשל, חזרה למסך הראשי
//                                 נקודה 4ג (true):  נכשל, מאתחל מכשיר
// הדגל _endScreenShown מונע ציור הודעת-סיום כפולה (למשל ABORT שמגיע
// אחרי שה-worker כבר הציג "נכשל").
// ──────────────────────────────────────────────────────────────────

#include "OtaProtocol.h"
#include <mbedtls/md5.h>
#include <Logic.h>
#include <esp_task_wdt.h>

extern Logic bl;

// ═══════════════════════════════════════════════════════════════
// CONSTRUCTOR / DESTRUCTOR
// ═══════════════════════════════════════════════════════════════

OtaProtocol::OtaProtocol(IOtaTx *tx)
    : _tx(tx), _state(OTA_IDLE), _totalSize(0), _receivedBytes(0), _nextExpectedSeq(0), _chunkSize(CHUNK_SIZE_DEFAULT), _windowSize(WINDOW_SIZE_DEFAULT), _currentCredits(0), _creditsEnabled(true), _packetQueue(nullptr), _workerTask(nullptr), _taskRunning(false), _lastAckTime(0), _packetsProcessedSinceAck(0), _lastNackedSeq(0xFFFF), _nackCount(0), _lastDataTime(0), _endScreenShown(false)
{
    memset(_expectedMd5, 0, sizeof(_expectedMd5));
}

OtaProtocol::~OtaProtocol()
{
    _stopWorkerTask();

    if (_packetQueue)
    {
        vQueueDelete(_packetQueue);
        _packetQueue = nullptr;
    }

    if (Update.isRunning())
    {
        Update.abort();
    }
}

// ═══════════════════════════════════════════════════════════════
// LIFECYCLE
// ═══════════════════════════════════════════════════════════════

void OtaProtocol::begin()
{
    _state = OTA_IDLE;

    // Serial.printf("[%s] ✓ Protocol engine initialized (Sliding Window mode)\n", OTA_TAG);
}

void OtaProtocol::onConnectionLost()
{
    Serial.printf("[%s] Connection lost - cleaning up\n", OTA_TAG);
    _resetToIdle();
}

void OtaProtocol::onMtuChanged(uint16_t newMtu)
{
    // Serial.printf("[%s] MTU changed to %d\n", OTA_TAG, newMtu);
    // Note: chunk size is locked after START
}

// ═══════════════════════════════════════════════════════════════
// PROTOCOL EVENT HANDLERS
// ═══════════════════════════════════════════════════════════════

void OtaProtocol::onControlCommand(uint8_t cmd, const uint8_t *payload, size_t len)
{
    Serial.printf("[%s] Control command: 0x%02X, state=%s\n",
                  OTA_TAG, cmd, getStateName(_state));

    switch (cmd)
    {
    case CMD_HELLO:
        _handleHello();
        break;
    case CMD_START:
        _handleStart(payload, len);
        break;
    case CMD_FINISH:
        _handleFinish();
        break;
    case CMD_CLIENT_FINAL_OK:
        _handleClientFinalOk();
        break;
    case CMD_ABORT:
        _handleAbort();
        break;
    default:
        Serial.printf("[%s] Unknown command: 0x%02X\n", OTA_TAG, cmd);
        break;
    }
}

void OtaProtocol::onDataSeq(uint8_t *data, size_t len)
{
    // Only accept data in IN_PROGRESS state
    if (_state != OTA_IN_PROGRESS)
    {
        return; // Silently ignore data in wrong state
    }

    if (len < 4)
    {
        Serial.printf("[%s] Packet too short: %d bytes\n", OTA_TAG, len);
        return;
    }

    // Parse packet: seq(2) + len(2) + data[len]
    uint16_t seq = data[0] | (data[1] << 8);
    uint16_t dataLen = data[2] | (data[3] << 8);

    if (len < 4 + dataLen)
    {
        Serial.printf("[%s] Incomplete packet: expected %d, got %d\n",
                      OTA_TAG, 4 + dataLen, len);
        return;
    }

    // Validate dataLen against chunk size
    if (dataLen == 0 || dataLen > _chunkSize)
    {
        Serial.printf("[%s] Invalid dataLen=%d (chunkSize=%d)\n",
                      OTA_TAG, dataLen, _chunkSize);
        _sendError(ERR_INVALID_PARAMS);
        return;
    }
    _lastDataTime = millis();

    // Push to queue (BLE callback just queues, doesn't write to flash)
    _pushToQueue(seq, data + 4, dataLen);
}

// ═══════════════════════════════════════════════════════════════
// COMMAND HANDLERS
// ═══════════════════════════════════════════════════════════════

void OtaProtocol::_handleHello()
{
    // During active transfer, do NOT abort on duplicate HELLO.
    // With NimBLE/app retries this can happen even after START_OK.
    if (_state == OTA_IN_PROGRESS || _state == OTA_VERIFYING)
    {
        Serial.printf("[%s] ⚠️ HELLO ignored during active transfer (state=%s)\n",
                      OTA_TAG, getStateName(_state));
        return;
    }

    // In START / DONE / ERROR, allow HELLO to reset handshake state.
    if (_state != OTA_IDLE)
    {
        Serial.printf("[%s] HELLO received in state=%s - resetting handshake\n",
                      OTA_TAG, getStateName(_state));

        _stopWorkerTask();

        if (Update.isRunning())
        {
            Update.abort();
        }

        _resetToIdle();
    }

    uint16_t mtu = _tx->negotiatedMtu();
    uint8_t response[3] = {
        STATUS_HELLO_OK,
        (uint8_t)(mtu & 0xFF),
        (uint8_t)((mtu >> 8) & 0xFF)
    };

    _tx->sendControl(response, sizeof(response));
    Serial.printf("[%s] ✓ HELLO_OK sent (MTU: %d)\n", OTA_TAG, mtu);

    _state = OTA_START;
}

void OtaProtocol::_handleStart(const uint8_t *payload, size_t len)
{
    if (_state != OTA_START)
    {
        Serial.printf("[%s] START in wrong state: %s\n", OTA_TAG, getStateName(_state));
        _sendError(ERR_BAD_STATE);
        return;
    }

    if (len < 36)
    {
        Serial.printf("[%s] START payload too short: %d\n", OTA_TAG, len);
        _sendError(ERR_INVALID_PARAMS);
        _state = OTA_IDLE;
        return;
    }

    // Parse: total_size(4) + md5_hex[32]
    _totalSize = payload[0] | (payload[1] << 8) | (payload[2] << 16) | (payload[3] << 24);
    memcpy(_expectedMd5, payload + 4, 32);
    _expectedMd5[32] = 0;

    Serial.printf("[%s] START received:\n", OTA_TAG);
    Serial.printf("[%s]   Size: %d bytes (%.2f MB)\n", OTA_TAG, _totalSize,
                  _totalSize / (1024.0 * 1024.0));
    Serial.printf("[%s]   MD5:  %.32s\n", OTA_TAG, _expectedMd5);

    // Validate size
    if (_totalSize == 0 || _totalSize > 16 * 1024 * 1024)
    {
        Serial.printf("[%s] Invalid size: %d\n", OTA_TAG, _totalSize);
        _sendError(ERR_SIZE_MISMATCH);
        _resetToIdle();
        return;
    }

    // Begin OTA
    if (!Update.begin(_totalSize))
    {
        Serial.printf("[%s] Update.begin() failed\n", OTA_TAG);
        _sendError(ERR_BEGIN_FAILED);
        _resetToIdle();
        return;
    }
    Update.setMD5((const char *)_expectedMd5);

    // Calculate chunk size
    uint16_t mtu = _tx->negotiatedMtu();
    _chunkSize = SAFE_CALCULATE_CHUNK_SIZE(mtu);

    // Set window size
    _windowSize = WINDOW_SIZE_DEFAULT;

    // Reset state
    _receivedBytes = 0;
    _nextExpectedSeq = 0;
    _lastNackedSeq = 0xFFFF;
    _nackCount = 0;
    _packetsProcessedSinceAck = 0;
    _lastAckTime = millis();
    _creditsEnabled = true;

    // Reset statistics
    _sessionStartMs = millis();
    _totalWriteTimeUs = 0;
    _maxWriteTimeUs = 0;
    _totalPacketsWritten = 0;

    // Calculate initial credits (full window available)
    _currentCredits = _windowSize;

    // Start worker task
    _startWorkerTask();

    if (!_taskRunning)
    {
        Serial.printf("[%s] Failed to start worker task\n", OTA_TAG);
        _sendError(ERR_BEGIN_FAILED);
        _resetToIdle();
        return;
    }

    // אתחול ל-millis() (ולא 0) כדי שה-timeout ב-worker יכסה גם את
    // המקרה שבו אף חבילת דאטה ראשונה לא מגיעה אחרי START_OK
    _lastDataTime = millis();

    _state = OTA_IN_PROGRESS;

    // ═══════════════════════════════════════════════════════════
    // נקודה 1: התחלת עדכון — כל הולידציות עברו, ה-worker רץ.
    // הציור כאן (לפני START_OK) כדי שיסתיים לפני שהאפליקציה
    // מתחילה להציף דאטה וה-BLE stack נכנס לעומס.
    // ═══════════════════════════════════════════════════════════
    _endScreenShown = false;
    bl.updateState.showUpdateStartScreen();

    // Send START_OK with window size and initial credits
    uint8_t response[7] = {
        STATUS_START_OK,
        (uint8_t)(mtu & 0xFF),
        (uint8_t)((mtu >> 8) & 0xFF),
        (uint8_t)(_chunkSize & 0xFF),
        (uint8_t)((_chunkSize >> 8) & 0xFF),
        _windowSize,
        _currentCredits};

    _tx->sendControl(response, sizeof(response));

    Serial.printf("[%s] ✓ START_OK sent:\n", OTA_TAG);
    Serial.printf("[%s]   MTU: %d, Chunk: %d\n", OTA_TAG, mtu, _chunkSize);
    Serial.printf("[%s]   Window: %d, Initial Credits: %d\n",
                  OTA_TAG, _windowSize, _currentCredits);
}

void OtaProtocol::_handleFinish()
{
    if (_state != OTA_IN_PROGRESS)
    {
        Serial.printf("[%s] FINISH in wrong state: %s\n", OTA_TAG, getStateName(_state));
        _sendError(ERR_BAD_STATE);
        return;
    }

    Serial.printf("[%s] FINISH received - verifying...\n", OTA_TAG);
    _state = OTA_VERIFYING;
    _sendAckWindow();

    // Wait for worker task to finish processing queue
    uint32_t waitStart = millis();
    while (uxQueueMessagesWaiting(_packetQueue) > 0 && millis() - waitStart < 2000)
    {
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    // Stop worker task
    _stopWorkerTask();

    // Extra safety: ensure we don't think we're done too early
    if (_receivedBytes < _totalSize)
    {
        Serial.printf("[%s] ✗ FINISH received but data is incomplete: %d/%d bytes\n",
                      OTA_TAG, _receivedBytes, _totalSize);
        _sendError(ERR_SIZE_MISMATCH);
        _resetToIdle();
        return;
    }

    // Verify size
    if (_receivedBytes != _totalSize)
    {
        Serial.printf("[%s] ✗ Size mismatch: expected %d, got %d\n",
                      OTA_TAG, _totalSize, _receivedBytes);
        _sendError(ERR_SIZE_MISMATCH);
        _resetToIdle();
        return;
    }

    // Verify MD5 and commit
    if (!_verifyMd5())
    {
        Serial.printf("[%s] ✗ MD5 verification failed\n", OTA_TAG);
        uint8_t response[2] = {STATUS_VERIFY_FAIL, ERR_MD5_MISMATCH};
        _tx->sendControl(response, sizeof(response));
        _resetToIdle();
        return;
    }

    // Success!
    Serial.printf("[%s] ✓ Verification successful - committing update\n", OTA_TAG);

    uint8_t response[1] = {STATUS_COMPLETE_OK};
    _tx->sendControl(response, sizeof(response));

    // Move to DONE state and WAIT for CLIENT_FINAL_OK
    _state = OTA_DONE;
    Serial.printf("[%s] ✓✓✓ OTA verification complete - waiting for CLIENT_FINAL_OK...\n", OTA_TAG);

    _finishCompletedTime = millis();

    // Create watchdog task that will restart if CLIENT_FINAL_OK doesn't arrive
    xTaskCreate([](void* param) {
        OtaProtocol* self = static_cast<OtaProtocol*>(param);
        vTaskDelay(pdMS_TO_TICKS(10000));  // Wait 10 seconds

        if (self->_state == OTA_DONE) {
            Serial.printf("[%s] ⚠️ CLIENT_FINAL_OK timeout - restarting anyway\n", OTA_TAG);

            // ═══════════════════════════════════════════════════
            // נקודה 3 (מסלול ה-watchdog): העדכון נצרב בהצלחה אך
            // האפליקציה לא אישרה (נותקה/נסגרה) — מציגים הצלחה
            // כדי שהמשתמש לא יראה מסך קפוא ואתחול-הפתעה.
            // ═══════════════════════════════════════════════════
            if (!self->_endScreenShown) {
                self->_endScreenShown = true;
                bl.updateState.showUpdateSuccessScreen();
            }

            vTaskDelay(pdMS_TO_TICKS(100));
            ESP.restart();
        }
        vTaskDelete(nullptr);
    }, "OTA_Watchdog", 2048, this, 1, nullptr);
}

void OtaProtocol::_handleClientFinalOk()
{
    if (_state != OTA_DONE)
    {
        Serial.printf("[%s] ⚠️ CLIENT_FINAL_OK in wrong state: %s (ignoring)\n",
                      OTA_TAG, getStateName(_state));
        return;
    }

    Serial.printf("[%s] ✓ CLIENT_FINAL_OK received - both sides agree!\n", OTA_TAG);
    Serial.printf("[%s] 🎉 OTA handshake complete - restarting in 1 second...\n", OTA_TAG);

    // ═══════════════════════════════════════════════════════════
    // נקודה 3 (המסלול הרגיל): עדכון הושלם, מאתחל מכשיר
    // ═══════════════════════════════════════════════════════════
    _endScreenShown = true;
    bl.updateState.showUpdateSuccessScreen();

    // Small delay for any final BLE operations
    vTaskDelay(pdMS_TO_TICKS(100));

    // NOW it's safe to restart!
    ESP.restart();
}

void OtaProtocol::_handleAbort()
{
    Serial.printf("[%s] ABORT received (state=%s)\n", OTA_TAG, getStateName(_state));

    // ═══════════════════════════════════════════════════════════
    // ABORT אחרי DONE: העדכון כבר נצרב (Update.end עבר) — אין מה
    // לבטל. בלי זה השעון היה נשאר על הגרסה הישנה עד כיבוי-הדלקה
    // ואז "מפתיע" עם גרסה חדשה. מתייחסים כהצלחה ומאתחלים.
    // ═══════════════════════════════════════════════════════════
    if (_state == OTA_DONE)
    {
        Serial.printf("[%s] ABORT after commit - treating as success, restarting\n", OTA_TAG);

        uint8_t response[1] = {STATUS_ABORT_OK};
        _tx->sendControl(response, sizeof(response));

        if (!_endScreenShown)
        {
            _endScreenShown = true;
            bl.updateState.showUpdateSuccessScreen();
        }

        vTaskDelay(pdMS_TO_TICKS(100));
        ESP.restart();
        return;
    }

    const bool wasActive = (_state == OTA_IN_PROGRESS || _state == OTA_VERIFYING);

    _stopWorkerTask();

    if (Update.isRunning())
    {
        Update.abort();
    }

    uint8_t response[1] = {STATUS_ABORT_OK};
    _tx->sendControl(response, sizeof(response));

    // ═══════════════════════════════════════════════════════════
    // נקודה 4א: ביטול ע"י המשתמש באמצע עדכון פעיל.
    // כלום לא נצרב — השעון ממשיך בגרסה הנוכחית, בלי אתחול.
    // (במצבי IDLE/START המסך הראשי עוד מוצג — אין מה לצייר.)
    // ═══════════════════════════════════════════════════════════
    if (wasActive && !_endScreenShown)
    {
        _endScreenShown = true;
        bl.updateState.showUpdateCancelledScreen();
    }

    _resetToIdle();
}

// ═══════════════════════════════════════════════════════════════
// QUEUE MANAGEMENT
// ═══════════════════════════════════════════════════════════════

bool OtaProtocol::_pushToQueue(uint16_t seq, uint8_t *data, uint16_t len)
{
    if (!_packetQueue || !_taskRunning)
    {
        Serial.printf("[%s] Cannot push - queue not ready\n", OTA_TAG);
        return false;
    }

    // ═══════════════════════════════════════════════════════════════
    // SEQUENCE HANDLING (Improved - allow duplicates, NACK only on forward gap)
    // ═══════════════════════════════════════════════════════════════

    // 1. Duplicate or old packet (seq < expected)
    if (seq < _nextExpectedSeq)
    {
        Serial.printf("[%s] Duplicate/old packet: seq=%d < nextExpected=%d (dropping)\n",
                      OTA_TAG, seq, _nextExpectedSeq);
        return false; // silently drop
    }

    // 2. Forward GAP → missing packets → send NACK
    if (seq > _nextExpectedSeq)
    {
        Serial.printf("[%s] ⚠ GAP detected: expected seq=%d, got seq=%d\n",
                      OTA_TAG, _nextExpectedSeq, seq);

        bool shouldSendNack = (_lastNackedSeq != _nextExpectedSeq);

        if (shouldSendNack)
        {
            // Enter recovery mode
            _creditsEnabled = false;
            _currentCredits = 0;
            _sendNack(_nextExpectedSeq);
        }
        else
        {
            Serial.printf("[%s]   (NACK already sent for seq=%d, dropping packet)\n",
                          OTA_TAG, _nextExpectedSeq);
        }

        return false;
    }

    // 3. seq == nextExpectedSeq → OK, continue to queue push

    // Allocate packet buffer
    QueuedPacket packet;
    if (!packet.allocate(len))
    {
        Serial.printf("[%s] ✗ Failed to allocate packet buffer (%d bytes)\n",
                      OTA_TAG, len);

        // כשל הקצאת זיכרון = מצב שהתהליך לא יכול להתאושש ממנו נקי —
        // מסלול האתחול (נקודה 4ג). כלום לא נצרב, האתחול בטוח.
        _fatalRestart(ERR_BEGIN_FAILED);
        return false; // לא מגיעים לכאן — _fatalRestart מאתחל
    }

    packet.seq = seq;
    memcpy(packet.data, data, len);

    // Non-blocking queue push
    if (xQueueSend(_packetQueue, &packet, 0) != pdTRUE)
    {
        Serial.printf("[%s] ⚠ Queue full! Dropping packet seq=%d\n", OTA_TAG, seq);
        packet.release();
        return false;
    }

    // Advance sequence counter
    _nextExpectedSeq++;

    return true;
}

uint16_t OtaProtocol::getQueuedPackets() const
{
    if (!_packetQueue)
        return 0;
    return uxQueueMessagesWaiting(_packetQueue);
}

// ═══════════════════════════════════════════════════════════════
// WORKER TASK
// ═══════════════════════════════════════════════════════════════

void OtaProtocol::_startWorkerTask()
{
    if (_taskRunning)
    {
        Serial.printf("[%s] Worker task already running\n", OTA_TAG);
        return;
    }

    // Create queue
    _packetQueue = xQueueCreate(QUEUE_LENGTH, sizeof(QueuedPacket));
    if (!_packetQueue)
    {
        Serial.printf("[%s] ✗ Failed to create queue\n", OTA_TAG);
        return;
    }

    // Create task
    BaseType_t result = xTaskCreatePinnedToCore(
        _workerTaskWrapper,
        "OTA_Worker",
        OTA_TASK_STACK_SIZE,
        this,
        OTA_TASK_PRIORITY,
        &_workerTask,
        OTA_TASK_CORE);

    if (result != pdPASS)
    {
        Serial.printf("[%s] ✗ Failed to create worker task\n", OTA_TAG);
        vQueueDelete(_packetQueue);
        _packetQueue = nullptr;
        return;
    }

    _taskRunning = true;
    Serial.printf("[%s] ✓ Worker task started\n", OTA_TAG);
}

void OtaProtocol::_stopWorkerTask()
{
    if (!_taskRunning)
    {
        return;
    }
    Serial.printf("[MEM] Before stopWorker: heap=%u\n", esp_get_free_heap_size());

    _taskRunning = false; // מסמן ל-worker לצאת מהלולאה

    // נותן קצת זמן למשימה לסיים ולהרוג את עצמה
    vTaskDelay(pdMS_TO_TICKS(100));

    // בשלב הזה ה-worker כבר עשה vTaskDelete על עצמו
    _workerTask = nullptr;

    // עכשיו אפשר לנקות את ה-queue בצורה בטוחה
    if (_packetQueue)
    {
        QueuedPacket packet;
        while (xQueueReceive(_packetQueue, &packet, 0) == pdTRUE)
        {
            packet.release();
        }

        vQueueDelete(_packetQueue);
        _packetQueue = nullptr;
    }
    Serial.printf("[MEM] After stopWorker: heap=%u\n", esp_get_free_heap_size());
    Serial.printf("[%s] ✓ Worker task stopped (queue destroyed)\n", OTA_TAG);
}

void OtaProtocol::_workerTaskWrapper(void *param)
{
    OtaProtocol *self = static_cast<OtaProtocol *>(param);
    self->_workerTaskLoop();
}

void OtaProtocol::_workerTaskLoop()
{
    Serial.printf("[%s] Worker task loop started\n", OTA_TAG);
    Serial.printf("[MEM] Worker start heap=%u\n", esp_get_free_heap_size());

    QueuedPacket packet;
    uint32_t lastSnapshot = millis();

    while (_taskRunning)
    {
        // Wait for packet (blocking with timeout)
        if (xQueueReceive(_packetQueue, &packet, pdMS_TO_TICKS(100)) == pdTRUE)
        {

            // ═══════════════════════════════════════════════════════════
            // Measure flash write time
            // ═══════════════════════════════════════════════════════════
            uint32_t t0 = micros();
            size_t written = Update.write(packet.data, packet.len);
            uint32_t dt = micros() - t0;

            // Accumulate statistics
            _totalWriteTimeUs += dt;
            _totalPacketsWritten++;
            if (dt > _maxWriteTimeUs)
            {
                _maxWriteTimeUs = dt;
            }

            // Check write success
            if (written != packet.len)
            {
                Serial.printf("[%s] ✗ Write failed: seq=%d, expected=%d, written=%d\n",
                              OTA_TAG, packet.seq, packet.len, written);
                packet.release();

                // נקודה 4ג: כשל כתיבת flash — התהליך לא יכול להסתיים,
                // אתחול הוא ההתאוששות הנקייה (כלום לא נצרב)
                _fatalRestart(ERR_OTA_WRITE_FAILED);
            }

            // Check for overflow vs totalSize
            if (_totalSize > 0 && (_receivedBytes + written) > _totalSize)
            {
                Serial.printf("[%s] ✗ Overflow: receivedBytes(%d) + written(%d) > totalSize(%d)\n",
                              OTA_TAG, _receivedBytes, written, _totalSize);
                packet.release();

                // נקודה 4ג: הצפת גודל — מצב פרוטוקול לא תקין, אתחול
                _fatalRestart(ERR_SIZE_MISMATCH);
            }

            // Update progress
            _receivedBytes += written;
            _packetsProcessedSinceAck++;

            // Log coarse progress every 100 packets (less noisy than before)
            if (packet.seq % 500 == 0)
            {
                uint8_t percent = _totalSize > 0 ? (_receivedBytes * 100) / _totalSize : 0;
                Serial.printf("[%s] Progress: seq=%d, %d/%d bytes (%%%d)\n", OTA_TAG, packet.seq, _receivedBytes, _totalSize, percent);
                // Serial.printf("[MEM] Worker loop heap=%u  HW=%u\n",
                //                 esp_get_free_heap_size(),
                //                 uxTaskGetStackHighWaterMark(nullptr));
            }

            // Free packet data
            packet.release();

            // Check if we should send ACK
            _checkAckTiming();

            // ═══════════════════════════════════════════════════════════
            // Periodic snapshot
            // ═══════════════════════════════════════════════════════════
            uint32_t now = millis();
            if (now - lastSnapshot >= 500)
            {
                lastSnapshot = now;

                // נקודה 2: עדכון התקדמות על המסך
                uint8_t percent = _totalSize > 0 ? (_receivedBytes * 100) / _totalSize : 0;
                bl.updateState.showUpdateProgress(percent);
            }

            // Feed watchdog
            esp_task_wdt_reset();
        }
        uint32_t now = millis();
        if (_lastDataTime > 0 && (now - _lastDataTime) > 5000)
        { // למשל 5 שניות
            Serial.printf("[%s] ✗ Inactivity timeout (> 5000 ms)\n", OTA_TAG);

            // נקודה 4ג: חוסר פעילות — האפליקציה נעלמה באמצע העברה.
            // אתחול מנקה הכל (worker, queue, מצב הפרוטוקול) בבת אחת.
            _fatalRestart(ERR_TIMEOUT);
        }
    }
    Serial.printf("[%s] Worker task loop exiting\n", OTA_TAG);
    vTaskDelete(nullptr); // מוחק את המשימה הנוכחית, לא חוזר
}

// ═══════════════════════════════════════════════════════════════
// ACK AND CREDITS MANAGEMENT
// ═══════════════════════════════════════════════════════════════

void OtaProtocol::_checkAckTiming()
{
    uint32_t now = millis();

    // Check both conditions: time-based OR count-based
    bool timeExpired = (now - _lastAckTime) >= ACK_INTERVAL_MS;
    bool countReached = _packetsProcessedSinceAck >= ACK_INTERVAL_PACKETS;

    if (timeExpired || countReached)
    {
        _sendAckWindow();
    }
}

void OtaProtocol::_sendAckWindow()
{
    if (!_tx->isConnected())
        return;

    // Calculate dynamic credits
    _currentCredits = _calculateCredits();

    // Build ACK packet using struct
    uint8_t buffer[1 + sizeof(AckWindowPacket)];
    buffer[0] = STATUS_ACK_WINDOW;

    AckWindowPacket *ack = (AckWindowPacket *)(buffer + 1);
    ack->next_expected_seq = _nextExpectedSeq;
    ack->credits = _currentCredits;
    ack->received_bytes = _receivedBytes;

    _tx->sendStatus(buffer, sizeof(buffer));

    // ═══════════════════════════════════════════════════════════════
    // Log one compact line per ACK_WINDOW
    // ═══════════════════════════════════════════════════════════════
    uint16_t q = getQueuedPackets();
    // Serial.printf("[%s] ✉️  ACK_WINDOW: nextSeq=%d, credits=%d, recvBytes=%d, queue=%d/%d\n",
    //               OTA_TAG, _nextExpectedSeq, _currentCredits, _receivedBytes, q, QUEUE_LENGTH);

    // Reset counters
    _lastAckTime = millis();
    _packetsProcessedSinceAck = 0;

    // If this is recovery from NACK, re-enable credits
    if (!_creditsEnabled && _currentCredits > 0)
    {
        _creditsEnabled = true;
        _nackCount = 0;
        _lastNackedSeq = 0xFFFF;
        Serial.printf("[%s] ✓ Recovery complete - credits re-enabled\n", OTA_TAG);
    }
}

uint8_t OtaProtocol::_calculateCredits()
{
    if (!_creditsEnabled)
    {
        return 0; // During NACK recovery, no credits
    }

    uint16_t queueSize = getQueuedPackets();

    // Dynamic credits based on queue occupancy
    if (queueSize >= QUEUE_HIGH_WATERMARK)
    {
        // Queue almost full → no more credits
        return 0;
    }
    else if (queueSize <= QUEUE_LOW_WATERMARK)
    {
        // Queue almost empty → full window
        return _windowSize;
    }
    else
    {
        // Queue in middle → proportional credits
        // Linear interpolation between LOW and HIGH watermarks
        uint16_t range = QUEUE_HIGH_WATERMARK - QUEUE_LOW_WATERMARK;
        uint16_t offset = queueSize - QUEUE_LOW_WATERMARK;
        uint8_t credits = _windowSize - ((_windowSize * offset) / range);
        return credits;
    }
}

// ═══════════════════════════════════════════════════════════════
// NACK AND ERROR HANDLING
// ═══════════════════════════════════════════════════════════════

void OtaProtocol::_sendNack(uint16_t expectedSeq)
{
    // Build NACK packet
    uint8_t nack[7];
    nack[0] = STATUS_NACK;

    // Pack expectedSeq (little-endian, 2 bytes)
    nack[1] = (uint8_t)(expectedSeq & 0xFF);
    nack[2] = (uint8_t)((expectedSeq >> 8) & 0xFF);

    // Pack receivedBytes (little-endian, 4 bytes)
    nack[3] = (uint8_t)(_receivedBytes & 0xFF);
    nack[4] = (uint8_t)((_receivedBytes >> 8) & 0xFF);
    nack[5] = (uint8_t)((_receivedBytes >> 16) & 0xFF);
    nack[6] = (uint8_t)((_receivedBytes >> 24) & 0xFF);

    _tx->sendControl(nack, sizeof(nack));

    _lastNackedSeq = expectedSeq;
    _nackCount++;

    Serial.printf("[%s] ⚠ NACK sent: expectedSeq=%d, receivedBytes=%d (attempt=%d)\n",
                  OTA_TAG, expectedSeq, _receivedBytes, _nackCount);

    // Check if too many NACKs
    if (_nackCount > MAX_NACK_RETRIES)
    {
        Serial.printf("[%s] ✗✗✗ Too many NACK retries (%d), aborting OTA\n", OTA_TAG, _nackCount);
        _sendError(ERR_TIMEOUT);
        _resetToIdle();
    }
}

void OtaProtocol::_sendError(uint8_t errorCode)
{
    const char *errorName = getErrorString(errorCode);

    uint8_t error[2] = {STATUS_ERROR, errorCode};
    _tx->sendControl(error, sizeof(error));

    Serial.printf("[%s] ERROR sent: 0x%02X (%s)\n", OTA_TAG, errorCode, errorName);
}

// ═══════════════════════════════════════════════════════════════
// FATAL RESTART (נקודה 4ג)
// כשלים שהתהליך לא יכול להתאושש מהם נקי (כשל flash, הצפה, timeout,
// כשל הקצאה): שולחים ERROR לאפליקציה, מציגים הודעה, ומאתחלים.
// האתחול בטוח — כלום לא נצרב עד Update.end(), השעון עולה עם אותה
// גרסה בדיוק, וכל המצב (worker, queue, פרוטוקול) מתאפס בבת אחת.
// לא חוזרת לעולם.
// ═══════════════════════════════════════════════════════════════

void OtaProtocol::_fatalRestart(uint8_t errorCode)
{
    Serial.printf("[%s] ✗✗✗ Fatal error 0x%02X (%s) - restarting device\n",
                  OTA_TAG, errorCode, getErrorString(errorCode));

    // מודיעים לאפליקציה לפני האתחול (best-effort; אם מנותקים — נבלע)
    _sendError(errorCode);

    if (!_endScreenShown)
    {
        _endScreenShown = true;
        bl.updateState.showUpdateFailedScreen(true); // willRestart = true
    }

    // השהיה: מספיק זמן שההודעה תיראה ושה-ERROR יספיק לצאת ב-BLE
    vTaskDelay(pdMS_TO_TICKS(1500));

    ESP.restart();
}

// ═══════════════════════════════════════════════════════════════
// VERIFICATION
// ═══════════════════════════════════════════════════════════════

bool OtaProtocol::_verifyMd5()
{
    if (!Update.end())
    {
        Serial.printf("[%s] Update.end() failed: %s\n", OTA_TAG, Update.errorString());
        return false;
    }

    Serial.printf("[%s] ✓ MD5 verified and committed\n", OTA_TAG);
    return true;
}

// ═══════════════════════════════════════════════════════════════
// STATE MANAGEMENT
// ═══════════════════════════════════════════════════════════════

void OtaProtocol::_resetToIdle()
{
    // ═══════════════════════════════════════════════════════════
    // נקודה 4ב: עדכון פעיל שהסתיים שלא דרך מסלול ההצלחה ושלא הוצגה
    // עליו הודעה עדיין — כשל MD5, דאטה חסרה, ניתוק, סערת NACK.
    // כלום לא נצרב, השעון ממשיך בגרסה הנוכחית — בלי אתחול.
    // ה-guard על המצב מוציא אוטומטית: כשלי START (מצב OTA_START),
    // איפוס handshake מ-HELLO, וקריאות מה-destructor.
    // הדגל מונע ציור כפול (למשל ABORT שכבר הציג "בוטל" ומיד קורא לכאן).
    // ═══════════════════════════════════════════════════════════
    if ((_state == OTA_IN_PROGRESS || _state == OTA_VERIFYING) && !_endScreenShown)
    {
        _endScreenShown = true;
        bl.updateState.showUpdateFailedScreen(false); // willRestart = false — חזרה למסך הראשי
    }

    Serial.printf("[%s] Resetting state to IDLE\n", OTA_TAG);

    _stopWorkerTask();

    if (Update.isRunning())
    {
        Update.abort();
    }

    _state = OTA_IDLE;
    _totalSize = 0;
    _receivedBytes = 0;
    _nextExpectedSeq = 0;
    _currentCredits = 0;
    _creditsEnabled = true;
    _lastNackedSeq = 0xFFFF;
    _nackCount = 0;
    _packetsProcessedSinceAck = 0;
    _lastDataTime = 0;
    memset(_expectedMd5, 0, sizeof(_expectedMd5));
}


// ═══════════════════════════════════════════════════════════════
// DEBUG / MONITORING
// ═══════════════════════════════════════════════════════════════

void OtaProtocol::printStatus()
{
    Serial.printf("[%s] ════════════ Status Report ════════════\n", OTA_TAG);
    Serial.printf("[%s] State: %s\n", OTA_TAG, getStateName(_state));
    Serial.printf("[%s] Progress: %d/%d bytes (%.1f%%)\n",
                  OTA_TAG, _receivedBytes, _totalSize,
                  _totalSize > 0 ? (_receivedBytes * 100.0f) / _totalSize : 0);
    Serial.printf("[%s] Next expected sequence: %d\n", OTA_TAG, _nextExpectedSeq);
    Serial.printf("[%s] Chunk size: %d bytes\n", OTA_TAG, _chunkSize);
    Serial.printf("[%s] Window size: %d\n", OTA_TAG, _windowSize);
    Serial.printf("[%s] Current credits: %d (enabled: %s)\n",
                  OTA_TAG, _currentCredits, _creditsEnabled ? "yes" : "no");
    Serial.printf("[%s] Queue: %d/%d packets\n", OTA_TAG, getQueuedPackets(), QUEUE_LENGTH);
    Serial.printf("[%s] NACK stats: last=%d, count=%d\n",
                  OTA_TAG, _lastNackedSeq, _nackCount);
    Serial.printf("[%s] Worker task: %s\n", OTA_TAG, _taskRunning ? "running" : "stopped");
    Serial.printf("[%s] ═══════════════════════════════════════\n", OTA_TAG);
}