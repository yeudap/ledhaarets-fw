// CoreService.cpp
// שירות BLE מרכזי מאוחד — מחליף את ButtonService, MenuService,
// TextService, TimeService ו-DataService.
// FirmwareService נשאר נפרד ללא שינוי.
//
// FW 7: נוסף DOMAIN_AUTH — כל דומיין אחר חסום עד להשלמת אימות (שער יחיד
// ב-onWrite). ראה AuthManager.h ו-CoreProtocol.h לתיעוד המלא.

#include "CoreService.h"
#include "AuthManager.h"
#include "TextCommandHandler.h"
#include "Logic.h"
#include "Preferences/UserPreferences.h"

extern bool            deviceConnected;
extern UserPreferences userPreferences;
extern Logic           bl;
extern State*          currentState;

// globals — כאן הגדרתם הרשמית (extern ב-Commands.h)
ButtonCommand currentButtonCommand = ButtonCommand::NONE;
MenuCommand   currentMenuCommand   = MenuCommand::NONE;

// הcharacteristic לשליחת EVT — מוגדר פה, משמש את coreSendEvt
static NimBLECharacteristic* evtCharacteristic = nullptr;

// Forward declarations
static uint8_t _mapTextError(uint8_t textErr);

// ============================================================================
// coreSendEvt — בניית תגובה אחידה ושליחתה כ-Notify
// ============================================================================
void coreSendEvt(uint8_t domain, uint8_t opcode, uint8_t status,
                 const uint8_t* data, size_t dataLen)
{
    if (!evtCharacteristic || !deviceConnected)
        return;

    // בניית הpacket: [DOMAIN][OPCODE][STATUS][DATA...]
    size_t totalLen = CORE_EVT_HEADER_SIZE + dataLen;
    uint8_t* buf = new uint8_t[totalLen];

    buf[0] = domain;
    buf[1] = opcode;
    buf[2] = status;

    if (data && dataLen > 0)
        memcpy(buf + CORE_EVT_HEADER_SIZE, data, dataLen);

    evtCharacteristic->setValue(buf, totalLen);
    evtCharacteristic->notify();

    delete[] buf;
}

// נוחות פנימית: שליחת EVT שגיאה בשורה אחת
static inline void sendErr(uint8_t domain, uint8_t opcode, uint8_t errCode)
{
    coreSendEvt(domain, opcode, CORE_STATUS_ERROR, &errCode, 1);
}

// נוחות פנימית: שליחת EVT הצלחה ללא data
static inline void sendOk(uint8_t domain, uint8_t opcode)
{
    coreSendEvt(domain, opcode, CORE_STATUS_OK);
}
// ============================================================================
// טיפול ב-AUTH domain
// CMD HELLO:    [DOMAIN_AUTH][AUTH_OP_HELLO][clientId 16B]
// CMD PIN:      [DOMAIN_AUTH][AUTH_OP_PIN][pin u16 LE 2B][clientId 16B]
// CMD REGISTER: [DOMAIN_AUTH][AUTH_OP_REGISTER]  (payload ריק)
//
// אימות ≠ רישום: HELLO/PIN מאמתים את הסשן בלבד (ללא כתיבת NVS);
// REGISTER רושם את ה-clientId שאומת בסשן כ-trusted (FIFO + NVS).
// ============================================================================
static void handleAuth(const uint8_t* payload, size_t len)
{
    uint8_t opcode = payload[1];

    switch (opcode)
    {
        // ----------------------------------------------------------------
        // AUTH_OP_HELLO — הזדהות עם clientId
        // ----------------------------------------------------------------
        case AUTH_OP_HELLO:
        {
            if (len < AUTH_HELLO_MIN_SIZE)
            {
                sendErr(DOMAIN_AUTH, AUTH_OP_HELLO, CORE_ERR_INVALID_PAYLOAD);
                return;
            }

            const uint8_t* clientId = payload + CORE_HEADER_SIZE;

            if (authManager.isTrusted(clientId))
            {
                // לכידת המזהה נדרשת גם כאן — REGISTER אידמפוטנטי של לקוח
                // רשום עובר דרך אותו מסלול (וחיוני אם ה-slot יפונה בהמשך)
                authManager.grantSession(clientId);
                Serial.println("[Auth] HELLO: known client, session granted");
                sendOk(DOMAIN_AUTH, AUTH_OP_HELLO);
                
                State* lastState = currentState;
                currentState = &bl.updateState;
                messageScreen.printMessages(messageScreen.INFO, "& ", "...vhmehkptk rcjwn", 1000);
                currentState = lastState;
                currentState->showStateScreen();
            }
            else
            {
                Serial.println("[Auth] HELLO: unknown client, PIN required");
                sendErr(DOMAIN_AUTH, AUTH_OP_HELLO, CORE_ERR_AUTH_REQUIRED);
            }
            break;
        }

        // ----------------------------------------------------------------
        // AUTH_OP_PIN — אימות עם PIN. מדליק את דגל הסשן בלבד —
        // הרישום כ-trusted הוא צעד נפרד (AUTH_OP_REGISTER).
        // ----------------------------------------------------------------
        case AUTH_OP_PIN:
        {
            if (len < AUTH_PIN_MIN_SIZE)
            {
                sendErr(DOMAIN_AUTH, AUTH_OP_PIN, CORE_ERR_INVALID_PAYLOAD);
                return;
            }

            uint16_t pin = payload[2] | (payload[3] << 8); // little-endian
            const uint8_t* clientId = payload + 4;

            if (authManager.checkPin(pin))
            {
                authManager.grantSession(clientId); // אימות בלבד, ללא NVS
                Serial.println("[Auth] PIN correct, session granted");
                sendOk(DOMAIN_AUTH, AUTH_OP_PIN);
                State* lastState = currentState;
                currentState = &bl.updateState;
                messageScreen.printMessages(messageScreen.INFO, "& ", "...vhmehkptk rcjwn", 1000);
                currentState = lastState;
                currentState->showStateScreen();
            }
            else
            {
                // השהיית שנייה לפני המענה — מנטרל brute-force.
                // רץ בטאסק של NimBLE host; מקובל כאן כי יש חיבור יחיד
                // ולפני אימות ממילא אין תעבורה אחרת על השירות.
                Serial.println("[Auth] PIN incorrect, delaying response");
                delay(AUTH_FAIL_DELAY_MS);
                sendErr(DOMAIN_AUTH, AUTH_OP_PIN, CORE_ERR_AUTH_FAILED);
            }
            break;
        }

        // ----------------------------------------------------------------
        // AUTH_OP_REGISTER — "זכור אותי להבא": רישום ה-clientId שאומת
        // בסשן הנוכחי כ-trusted. payload ריק. אידמפוטנטי.
        // ----------------------------------------------------------------
        case AUTH_OP_REGISTER:
        {
            if (!authManager.isSessionAuthenticated())
            {
                sendErr(DOMAIN_AUTH, AUTH_OP_REGISTER, CORE_ERR_NOT_AUTHENTICATED);
                return;
            }

            if (!authManager.registerSessionClient())
            {
                // רשת ביטחון — סשן מאומת תמיד נושא clientId תקף
                sendErr(DOMAIN_AUTH, AUTH_OP_REGISTER, CORE_ERR_INTERNAL);
                return;
            }

            Serial.println("[Auth] REGISTER: session client stored as trusted");
            sendOk(DOMAIN_AUTH, AUTH_OP_REGISTER);
            break;
        }

        default:
            sendErr(DOMAIN_AUTH, opcode, CORE_ERR_UNKNOWN_OPCODE);
            break;
    }
}

// ============================================================================
// טיפול ב-NAV domain
// CMD: [DOMAIN_NAV][NAV_OP_PRESS][key 1B]
// ============================================================================
static void handleNav(const uint8_t* payload, size_t len)
{
    if (len < NAV_CMD_MIN_SIZE)
    {
        sendErr(DOMAIN_NAV, NAV_OP_PRESS, CORE_ERR_INVALID_PAYLOAD);
        return;
    }

    uint8_t opcode = payload[1];
    uint8_t key    = payload[2];

    if (opcode != NAV_OP_PRESS)
    {
        sendErr(DOMAIN_NAV, opcode, CORE_ERR_UNKNOWN_OPCODE);
        return;
    }

    switch (key)
    {
        case NAV_KEY_UP:   currentButtonCommand = ButtonCommand::UP;   break;
        case NAV_KEY_DOWN: currentButtonCommand = ButtonCommand::DOWN; break;
        case NAV_KEY_MENU: currentButtonCommand = ButtonCommand::MENU; break;
        case NAV_KEY_OK:   currentButtonCommand = ButtonCommand::OK;   break;
        default:
            sendErr(DOMAIN_NAV, NAV_OP_PRESS, CORE_ERR_INVALID_VALUE);
            return;
    }

    sendOk(DOMAIN_NAV, NAV_OP_PRESS);
}

// ============================================================================
// טיפול ב-MENU domain
// CMD: [DOMAIN_MENU][MENU_OP_GOTO][menu_code 1B]
// ============================================================================
static void handleMenu(const uint8_t* payload, size_t len)
{
    if (len < MENU_CMD_MIN_SIZE)
    {
        sendErr(DOMAIN_MENU, MENU_OP_GOTO, CORE_ERR_INVALID_PAYLOAD);
        return;
    }

    uint8_t opcode    = payload[1];
    uint8_t menuCode  = payload[2];

    if (opcode != MENU_OP_GOTO)
    {
        sendErr(DOMAIN_MENU, opcode, CORE_ERR_UNKNOWN_OPCODE);
        return;
    }

    // המרה מ-menu_code ל-MenuCommand — תואם לEnum הקיים
    MenuCommand cmd = static_cast<MenuCommand>(menuCode);

    // בדיקת תקינות: NONE (0) לא חוקי, וכל ערך שאינו מוגדר ב-enum
    if (cmd == MenuCommand::NONE)
    {
        sendErr(DOMAIN_MENU, MENU_OP_GOTO, CORE_ERR_INVALID_VALUE);
        return;
    }

    currentMenuCommand = cmd;

    sendOk(DOMAIN_MENU, MENU_OP_GOTO);
}

// ============================================================================
// טיפול ב-TEXT domain
// ============================================================================
static void handleText(const uint8_t* payload, size_t len)
{
    if (len < 2)
    {
        sendErr(DOMAIN_TEXT, 0x00, CORE_ERR_INVALID_PAYLOAD);
        return;
    }

    uint8_t opcode = payload[1];

    switch (opcode)
    {
        // ----------------------------------------------------------------
        // TEXT_OP_READ — [DOMAIN][TEXT_OP_READ][index]
        // ----------------------------------------------------------------
        case TEXT_OP_READ:
        {
            if (len < TEXT_READ_MIN_SIZE)
            {
                sendErr(DOMAIN_TEXT, TEXT_OP_READ, CORE_ERR_INVALID_PAYLOAD);
                return;
            }

            uint8_t index = payload[2];
            CommandResult result = handleReadText(index);

            if (result.success)
            {
                size_t textLen = result.text.length();
                uint8_t* data = new uint8_t[textLen];
                memcpy(data, result.text.c_str(), textLen);
                coreSendEvt(DOMAIN_TEXT, TEXT_OP_READ, CORE_STATUS_OK, data, textLen);
                delete[] data;
            }
            else
            {
                uint8_t errCode = _mapTextError(result.errorCode);
                sendErr(DOMAIN_TEXT, TEXT_OP_READ, errCode);
            }
            break;
        }

        // ----------------------------------------------------------------
        // TEXT_OP_WRITE — [DOMAIN][TEXT_OP_WRITE][index][string bytes]
        // ----------------------------------------------------------------
        case TEXT_OP_WRITE:
        {
            if (len < TEXT_WRITE_MIN_SIZE)
            {
                sendErr(DOMAIN_TEXT, TEXT_OP_WRITE, CORE_ERR_INVALID_PAYLOAD);
                return;
            }

            uint8_t index = payload[2];
            // הטקסט מתחיל מ-byte 3, אורכו len-3
            String text = String(reinterpret_cast<const char*>(payload + 3),
                                 len - 3);
            CommandResult result = handleWriteText(index, text);

            if (result.success)
            {
                uint8_t data = result.data1;
                coreSendEvt(DOMAIN_TEXT, TEXT_OP_WRITE, CORE_STATUS_OK, &data, 1);
            }
            else
            {
                sendErr(DOMAIN_TEXT, TEXT_OP_WRITE, _mapTextError(result.errorCode));
            }
            break;
        }

        // ----------------------------------------------------------------
        // TEXT_OP_DELETE — [DOMAIN][TEXT_OP_DELETE][index]
        // ----------------------------------------------------------------
        case TEXT_OP_DELETE:
        {
            if (len < TEXT_READ_MIN_SIZE) // אותו גודל מינימלי כמו READ
            {
                sendErr(DOMAIN_TEXT, TEXT_OP_DELETE, CORE_ERR_INVALID_PAYLOAD);
                return;
            }

            uint8_t index = payload[2];
            CommandResult result = handleDeleteText(index);

            if (result.success)
            {
                uint8_t data = result.data1;
                coreSendEvt(DOMAIN_TEXT, TEXT_OP_DELETE, CORE_STATUS_OK, &data, 1);
            }
            else
            {
                sendErr(DOMAIN_TEXT, TEXT_OP_DELETE, _mapTextError(result.errorCode));
            }
            break;
        }

        // ----------------------------------------------------------------
        // TEXT_OP_DISPLAY — [DOMAIN][TEXT_OP_DISPLAY][index]
        // ----------------------------------------------------------------
        case TEXT_OP_DISPLAY:
        {
            if (len < TEXT_READ_MIN_SIZE)
            {
                sendErr(DOMAIN_TEXT, TEXT_OP_DISPLAY, CORE_ERR_INVALID_PAYLOAD);
                return;
            }

            uint8_t index = payload[2];
            CommandResult result = handleDisplayText(index);

            if (result.success)
            {
                uint8_t data = result.data1;
                coreSendEvt(DOMAIN_TEXT, TEXT_OP_DISPLAY, CORE_STATUS_OK, &data, 1);
            }
            else
            {
                sendErr(DOMAIN_TEXT, TEXT_OP_DISPLAY, _mapTextError(result.errorCode));
            }
            break;
        }

        // ----------------------------------------------------------------
        // TEXT_OP_SWAP — [DOMAIN][TEXT_OP_SWAP][idx_a][idx_b]
        // ----------------------------------------------------------------
        case TEXT_OP_SWAP:
        {
            if (len < TEXT_SWAP_MIN_SIZE)
            {
                sendErr(DOMAIN_TEXT, TEXT_OP_SWAP, CORE_ERR_INVALID_PAYLOAD);
                return;
            }

            uint8_t idxA = payload[2];
            uint8_t idxB = payload[3];
            CommandResult result = handleSwapTexts(idxA, idxB);

            if (result.success)
            {
                uint8_t data[2] = { result.data1, result.data2 };
                coreSendEvt(DOMAIN_TEXT, TEXT_OP_SWAP, CORE_STATUS_OK, data, 2);
            }
            else
            {
                sendErr(DOMAIN_TEXT, TEXT_OP_SWAP, _mapTextError(result.errorCode));
            }
            break;
        }

        // ----------------------------------------------------------------
        // TEXT_OP_COUNT — [DOMAIN][TEXT_OP_COUNT]
        // ----------------------------------------------------------------
        case TEXT_OP_COUNT:
        {
            CommandResult result = handleGetTextCount();

            if (result.success)
            {
                uint8_t data = result.data1;
                coreSendEvt(DOMAIN_TEXT, TEXT_OP_COUNT, CORE_STATUS_OK, &data, 1);
            }
            else
            {
                sendErr(DOMAIN_TEXT, TEXT_OP_COUNT, CORE_ERR_INTERNAL);
            }
            break;
        }

        default:
            sendErr(DOMAIN_TEXT, opcode, CORE_ERR_UNKNOWN_OPCODE);
            break;
    }
}

// המרת קודי שגיאה מ-TextCommandHandler לקודי CORE_ERR
static uint8_t _mapTextError(uint8_t textErr)
{
    switch (textErr)
    {
        case ERR_INVALID_INDEX:  return CORE_ERR_INVALID_INDEX;
        case ERR_INVALID_FORMAT: return CORE_ERR_INVALID_VALUE;
        case ERR_EMPTY_TEXT:     return CORE_ERR_EMPTY;
        case ERR_WRITE_FAILED:   return CORE_ERR_WRITE_FAILED;
        case ERR_MEMORY:         return CORE_ERR_INTERNAL;
        default:                 return CORE_ERR_INTERNAL;
    }
}

// ============================================================================
// טיפול ב-TIME domain
// ============================================================================
static void handleTime(const uint8_t* payload, size_t len)
{
    if (len < 2)
    {
        sendErr(DOMAIN_TIME, 0x00, CORE_ERR_INVALID_PAYLOAD);
        return;
    }

    uint8_t opcode = payload[1];

    switch (opcode)
    {
        // ----------------------------------------------------------------
        // TIME_OP_SET — [DOMAIN][TIME_OP_SET][7B time data]
        // ----------------------------------------------------------------
        case TIME_OP_SET:
        {
            if (len < TIME_SET_MIN_SIZE)
            {
                sendErr(DOMAIN_TIME, TIME_OP_SET, CORE_ERR_INVALID_PAYLOAD);
                return;
            }

            const uint8_t* timeBytes = payload + 2;
            uint16_t year   = (timeBytes[0] << 8) | timeBytes[1];
            uint8_t  month  = timeBytes[2];
            uint8_t  day    = timeBytes[3];
            uint8_t  hour   = timeBytes[4];
            uint8_t  minute = timeBytes[5];
            uint8_t  second = timeBytes[6];

            DateTime newTime(year, month, day, hour, minute, second);

            if (!newTime.isValid())
            {
                sendErr(DOMAIN_TIME, TIME_OP_SET, CORE_ERR_INVALID_TIME);
                return;
            }

            // שמירת ה-state הנוכחי, כיוון השעה, חזרה ל-state
            State* lastState = currentState;
            currentState = &bl.clockSetTime;
            bl.setClockState.validateAndSetTime(newTime);
            bl.setClockState.commonMenuAction();
            currentState = lastState;
            currentState->showStateScreen();

            sendOk(DOMAIN_TIME, TIME_OP_SET);
            break;
        }

        // ----------------------------------------------------------------
        // TIME_OP_GET — [DOMAIN][TIME_OP_GET]
        // EVT data: [7B time data]
        // ----------------------------------------------------------------
        case TIME_OP_GET:
        {

            uint8_t timeBytes[TIME_DATA_SIZE];
            timeBytes[0] = (now.year() >> 8) & 0xFF;
            timeBytes[1] =  now.year()       & 0xFF;
            timeBytes[2] =  now.month();
            timeBytes[3] =  now.day();
            timeBytes[4] =  now.hour();
            timeBytes[5] =  now.minute();
            timeBytes[6] =  now.second();

            coreSendEvt(DOMAIN_TIME, TIME_OP_GET, CORE_STATUS_OK,
                        timeBytes, TIME_DATA_SIZE);
            break;
        }

        default:
            sendErr(DOMAIN_TIME, opcode, CORE_ERR_UNKNOWN_OPCODE);
            break;
    }
}

// ============================================================================
// טיפול ב-PREF domain
// ============================================================================
static void handlePref(const uint8_t* payload, size_t len)
{
    if (len < 2)
    {
        sendErr(DOMAIN_PREF, 0x00, CORE_ERR_INVALID_PAYLOAD);
        return;
    }

    uint8_t opcode = payload[1];

    switch (opcode)
    {
        // ----------------------------------------------------------------
        // PREF_OP_READ — [DOMAIN][PREF_OP_READ][key string, null-terminated]
        // EVT data: [pref_type 1B][value bytes]
        // ----------------------------------------------------------------
        case PREF_OP_READ:
        {
            if (len < PREF_READ_MIN_SIZE)
            {
                sendErr(DOMAIN_PREF, PREF_OP_READ, CORE_ERR_INVALID_PAYLOAD);
                return;
            }

            // ה-key מתחיל מ-byte 2
            const char* key = reinterpret_cast<const char*>(payload + 2);

            // חיפוש ב-simplePrefs בלבד (textPrefs נגישים דרך TEXT domain).
            // הערה: המפתח "pin" אינו ב-simplePrefs במכוון — ה-PIN write-only.
            for (auto pref : userPreferences.simplePrefs)
            {
                if (strcmp(pref->getKey(), key) == 0)
                {
                    auto valueBytes = pref->toBytes();

                    // EVT data: [type][value bytes]
                    size_t dataLen = 1 + valueBytes.size();
                    uint8_t* data = new uint8_t[dataLen];
                    data[0] = static_cast<uint8_t>(pref->getType());
                    memcpy(data + 1, valueBytes.data(), valueBytes.size());

                    coreSendEvt(DOMAIN_PREF, PREF_OP_READ, CORE_STATUS_OK,
                                data, dataLen);
                    delete[] data;
                    return;
                }
            }

            sendErr(DOMAIN_PREF, PREF_OP_READ, CORE_ERR_INVALID_KEY);
            break;
        }

        // ----------------------------------------------------------------
        // PREF_OP_WRITE — [DOMAIN][PREF_OP_WRITE][type 1B][key\0][value bytes]
        // ----------------------------------------------------------------
        case PREF_OP_WRITE:
        {
            if (len < PREF_WRITE_MIN_SIZE)
            {
                sendErr(DOMAIN_PREF, PREF_OP_WRITE, CORE_ERR_INVALID_PAYLOAD);
                return;
            }

            PreferenceType type = static_cast<PreferenceType>(payload[2]);

            // מציאת ה-key (מתחיל מ-byte 3, מסתיים ב-null)
            const char* keyStart = reinterpret_cast<const char*>(payload + 3);
            const char* keyEnd   = reinterpret_cast<const char*>(
                                       memchr(keyStart, '\0', len - 3));
            if (!keyEnd)
            {
                sendErr(DOMAIN_PREF, PREF_OP_WRITE, CORE_ERR_INVALID_KEY);
                return;
            }

            const uint8_t* valueStart = reinterpret_cast<const uint8_t*>(keyEnd + 1);
            size_t valueLen = len - (valueStart - payload);

            // ------------------------------------------------------------
            // Special-case: שינוי PIN — מנותב ל-AuthManager (namespace "sec")
            // ולא ל-userPreferences. ה-PIN אינו קריא ואינו מסונכרן.
            // מגיעים לכאן רק אחרי אימות (השער ב-onWrite חוסם לפני).
            // ------------------------------------------------------------
            if (strcmp(keyStart, AUTH_PIN_PREF_KEY) == 0)
            {
                if (payload[2] != PREF_TYPE_U16)
                {
                    sendErr(DOMAIN_PREF, PREF_OP_WRITE, CORE_ERR_TYPE_MISMATCH);
                    return;
                }
                if (valueLen < 2)
                {
                    sendErr(DOMAIN_PREF, PREF_OP_WRITE, CORE_ERR_INVALID_PAYLOAD);
                    return;
                }

                uint16_t newPin = valueStart[0] | (valueStart[1] << 8); // LE

                if (newPin > AUTH_MAX_PIN)
                {
                    sendErr(DOMAIN_PREF, PREF_OP_WRITE, CORE_ERR_INVALID_VALUE);
                    return;
                }

                authManager.setPin(newPin);
                sendOk(DOMAIN_PREF, PREF_OP_WRITE);
                return;
            }

            // חיפוש וכתיבה ב-allPrefs
            for (auto pref : userPreferences.allPrefs)
            {
                if (strcmp(pref->getKey(), keyStart) == 0)
                {
                    if (pref->getType() != type)
                    {
                        sendErr(DOMAIN_PREF, PREF_OP_WRITE, CORE_ERR_TYPE_MISMATCH);
                        return;
                    }

                    if (!pref->fromBytes(valueStart, valueLen))
                    {
                        sendErr(DOMAIN_PREF, PREF_OP_WRITE, CORE_ERR_INVALID_VALUE);
                        return;
                    }

                    pref->save();
                    sendOk(DOMAIN_PREF, PREF_OP_WRITE);
                    // טיפול מיוחד: עדכון ofek (index 6) מצריך פעולה נוספת
                    if (pref->getIndex() == 6)
                    {
                        State* lastState = currentState;
                        currentState = &bl.updateState;
                        bl.setCustomState.printUpdateSucc();
                        currentState = lastState;
                        currentState->showStateScreen();
                    }
                    return;
                }
            }

            sendErr(DOMAIN_PREF, PREF_OP_WRITE, CORE_ERR_INVALID_KEY);
            break;
        }

        // ----------------------------------------------------------------
        // PREF_OP_SYNC — [DOMAIN][PREF_OP_SYNC]
        // EVT data: [count 1B][index, index, ...]
        // ----------------------------------------------------------------
        case PREF_OP_SYNC:
        {
            std::vector<uint8_t> indices;

            for (auto pref : userPreferences.simplePrefs)
            {
                if (pref->hasChangedFromDefault())
                    indices.push_back(pref->getIndex());
            }

            // EVT data: [count][index0][index1]...
            size_t dataLen = 1 + indices.size();
            uint8_t* data = new uint8_t[dataLen];
            data[0] = static_cast<uint8_t>(indices.size());
            memcpy(data + 1, indices.data(), indices.size());

            coreSendEvt(DOMAIN_PREF, PREF_OP_SYNC, CORE_STATUS_OK,
                        data, dataLen);
            delete[] data;
            break;
        }

        default:
            sendErr(DOMAIN_PREF, opcode, CORE_ERR_UNKNOWN_OPCODE);
            break;
    }
}

// ============================================================================
// שם המכשיר — אחסון ב-NVS והחלה על הפרסום
// ============================================================================
String loadDeviceName()
{
    Preferences prefs;
    prefs.begin(SYS_NAME_NVS_NAMESPACE, true); // read-only
    String name = prefs.getString(SYS_NAME_NVS_KEY, SYS_DEFAULT_DEVICE_NAME);
    prefs.end();
    return name;
}

static void saveDeviceName(const String& name)
{
    Preferences prefs;
    prefs.begin(SYS_NAME_NVS_NAMESPACE, false);
    if (name.isEmpty())
        prefs.remove(SYS_NAME_NVS_KEY);        // איפוס לברירת המחדל
    else
        prefs.putString(SYS_NAME_NVS_KEY, name);
    prefs.end();
}

// החלת השם על GAP ועל ה-scan response. בזמן חיבור הפרסום מושהה ממילא,
// והנתונים המעודכנים ישודרו ב-startAdvertising שאחרי הניתוק.
void applyDeviceName(const String& name)
{
    NimBLEDevice::setDeviceName(name.c_str());

    NimBLEAdvertisementData scanResp;
    scanResp.setName(name.c_str());
    NimBLEDevice::getAdvertising()->setScanResponseData(scanResp);
}

// ============================================================================
// ריסטארט נדחה — ה-ACK יוצא קודם, הריסטארט אחרי השהיה קצרה
// ============================================================================
static void restartTimerCallback(void* arg)
{
    esp_restart();
}

static void scheduleRestart()
{
    const esp_timer_create_args_t timerArgs = {
        .callback = &restartTimerCallback,
        .arg = nullptr,
        .dispatch_method = ESP_TIMER_TASK,
        .name = "sys_restart"
    };
    esp_timer_handle_t timer;
    esp_timer_create(&timerArgs, &timer);
    esp_timer_start_once(timer, 500 * 1000); // 500ms — מספיק ל-notify לצאת
}

// ============================================================================
// טיפול ב-SYS domain
// ============================================================================
static void handleSys(const uint8_t* payload, size_t len)
{
    uint8_t opcode = payload[1];

    switch (opcode)
    {
        // ----------------------------------------------------------------
        // SYS_OP_RESTART — [DOMAIN][SYS_OP_RESTART]
        // ACK נשלח מיד; הריסטארט מתוזמן ל-500ms כדי שה-notify יספיק לצאת.
        // ----------------------------------------------------------------
        case SYS_OP_RESTART:
        {
            Serial.println("[Sys] Restart requested");
            sendOk(DOMAIN_SYS, SYS_OP_RESTART);
            currentState = &bl.updateState;
            messageScreen.printMessages(messageScreen.INFO, "! ck oha", "hbsh  kujwt  gmcn  iugav");
            scheduleRestart();
            break;
        }

        // ----------------------------------------------------------------
        // SYS_OP_SET_NAME — [DOMAIN][SYS_OP_SET_NAME][UTF-8 bytes]
        // payload ריק = איפוס לברירת המחדל. ולידציה לפי בייטים, לא תווים.
        // ----------------------------------------------------------------
        case SYS_OP_SET_NAME:
        {
            size_t nameLen = len - CORE_HEADER_SIZE;

            if (nameLen > SYS_NAME_MAX_BYTES)
            {
                sendErr(DOMAIN_SYS, SYS_OP_SET_NAME, CORE_ERR_INVALID_VALUE);
                return;
            }

            String newName;
            if (nameLen == 0)
            {
                newName = SYS_DEFAULT_DEVICE_NAME;
                saveDeviceName("");            // מחיקת המפתח מ-NVS
            }
            else
            {
                newName = String(reinterpret_cast<const char*>(payload + CORE_HEADER_SIZE), nameLen);
                saveDeviceName(newName);
            }

            applyDeviceName(newName);

            Serial.printf("[Sys] Device name set to: %s\n", newName.c_str());
            sendOk(DOMAIN_SYS, SYS_OP_SET_NAME);
            break;
        }

        // ----------------------------------------------------------------
        // SYS_OP_GET_MAC — [DOMAIN][SYS_OP_GET_MAC]
        // EVT data: [6 בייט MAC]. נדרש בעיקר ל-iOS, שבו device.id הוא
        // UUID פנימי של CoreBluetooth ולא כתובת החומרה.
        // ----------------------------------------------------------------
        case SYS_OP_GET_MAC:
        {
            uint8_t mac[6];
            esp_efuse_mac_get_default(mac);
            coreSendEvt(DOMAIN_SYS, SYS_OP_GET_MAC, CORE_STATUS_OK, mac, 6);
            break;
        }

        default:
            sendErr(DOMAIN_SYS, opcode, CORE_ERR_UNKNOWN_OPCODE);
            break;
    }
}

// ============================================================================
// CoreCallbacks — onWrite: שער אימות + ניתוב לפי DOMAIN
// ============================================================================
class CoreCallbacks : public NimBLECharacteristicCallbacks
{
    void onWrite(NimBLECharacteristic* pCharacteristic,
                 NimBLEConnInfo& connInfo) override
    {
        std::string raw = pCharacteristic->getValue();
        const uint8_t* payload = reinterpret_cast<const uint8_t*>(raw.data());
        size_t len = raw.length();

        if (len < CORE_HEADER_SIZE)
        {
            Serial.println("[Core] Packet too short");
            return;
        }

        uint8_t domain = payload[0];

        Serial.printf("[Core] domain=0x%02X opcode=0x%02X len=%u\n",
                      domain, payload[1], len);

        // --------------------------------------------------------------
        // שער אימות יחיד: כל דומיין ≠ AUTH חסום עד להשלמת אימות.
        // ה-EVT מהדהד את ה-domain+opcode המקוריים כדי שה-completer
        // בצד האפליקציה יתפוס את התשובה.
        // --------------------------------------------------------------
        if (domain != DOMAIN_AUTH && !authManager.isSessionAuthenticated())
        {
            Serial.printf("[Core] Blocked (not authenticated): domain=0x%02X\n",
                          domain);
            sendErr(domain, payload[1], CORE_ERR_NOT_AUTHENTICATED);
            return;
        }

        switch (domain)
        {
            case DOMAIN_AUTH: handleAuth(payload, len); break;
            case DOMAIN_NAV:  handleNav(payload, len);  break;
            case DOMAIN_MENU: handleMenu(payload, len); break;
            case DOMAIN_TEXT: handleText(payload, len); break;
            case DOMAIN_TIME: handleTime(payload, len); break;
            case DOMAIN_PREF: handlePref(payload, len); break;
            case DOMAIN_SYS:  handleSys(payload, len);  break;            
            default:
                Serial.printf("[Core] Unknown domain: 0x%02X\n", domain);
                sendErr(domain, payload[1], CORE_ERR_UNKNOWN_DOMAIN);
                break;
        }
    }
};

// ============================================================================
// initCoreService — נקרא מ-Bluetooth.cpp
// ============================================================================
void initCoreService(NimBLEServer* pServer)
{
    NimBLEService* coreService = pServer->createService(CORE_SERVICE_UUID);

    // CMD characteristic — Flutter כותב פקודות
    NimBLECharacteristic* cmdChar = coreService->createCharacteristic(
        CORE_CMD_CHAR_UUID,
        NIMBLE_PROPERTY::WRITE);
    cmdChar->setCallbacks(new CoreCallbacks());

    // EVT characteristic — השעון שולח תגובות כ-Notify
    evtCharacteristic = coreService->createCharacteristic(
        CORE_EVT_CHAR_UUID,
        NIMBLE_PROPERTY::NOTIFY);

    Serial.println("[Core] Core Service initialized");
}