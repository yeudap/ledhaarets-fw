// AuthManager.cpp
// מימוש אימות האפליקציה — ראה תיעוד מלא ב-AuthManager.h

#include "AuthManager.h"
#include <string.h>

// instance גלובלי
AuthManager authManager;

// NVS keys (namespace "sec")
static const char* NVS_NAMESPACE  = "sec";
static const char* NVS_KEY_PIN    = "pin";
static const char* NVS_KEY_SLOTS  = "trusted";

// ============================================================================
// init — פתיחת ה-namespace וטעינת המצב השמור
// ============================================================================
void AuthManager::init()
{
    _prefs.begin(NVS_NAMESPACE, false);

    _pin = _prefs.getUShort(NVS_KEY_PIN, AUTH_DEFAULT_PIN);

    memset(_slots, 0, SLOTS_BLOB_SIZE);
    size_t stored = _prefs.getBytesLength(NVS_KEY_SLOTS);
    if (stored == SLOTS_BLOB_SIZE)
    {
        _prefs.getBytes(NVS_KEY_SLOTS, _slots, SLOTS_BLOB_SIZE);
    }
    else if (stored != 0)
    {
        // גודל לא צפוי (לא אמור לקרות) — מתעלמים ומתחילים נקי
        Serial.printf("[Auth] Unexpected slots blob size: %u, ignoring\n",
                      (unsigned)stored);
    }

    resetSession();

    int trustedCount = 0;
    for (int i = 0; i < AUTH_TRUSTED_SLOTS; i++)
        if (!_isAllZero(_slots + i * AUTH_CLIENT_ID_SIZE))
            trustedCount++;

    Serial.printf("[Auth] Initialized. trusted=%d/%d, pin=%s\n",
                  trustedCount, AUTH_TRUSTED_SLOTS,
                  _pin == AUTH_DEFAULT_PIN ? "default" : "custom");
}

// ============================================================================
// ניהול הסשן — אימות + לכידת ה-clientId שאומת
// ============================================================================
void AuthManager::grantSession(const uint8_t* clientId)
{
    _sessionAuthenticated = true;
    memcpy(_sessionClientId, clientId, AUTH_CLIENT_ID_SIZE);
}

void AuthManager::resetSession()
{
    _sessionAuthenticated = false;
    memset(_sessionClientId, 0, AUTH_CLIENT_ID_SIZE);
}

// ============================================================================
// isTrusted — השוואת clientId מול שלושת ה-slots
// ============================================================================
bool AuthManager::isTrusted(const uint8_t* clientId) const
{
    // clientId אפסים אינו חוקי — אחרת היה מתאים ל-slot ריק
    if (_isAllZero(clientId))
        return false;

    for (int i = 0; i < AUTH_TRUSTED_SLOTS; i++)
    {
        if (memcmp(_slots + i * AUTH_CLIENT_ID_SIZE,
                   clientId, AUTH_CLIENT_ID_SIZE) == 0)
            return true;
    }
    return false;
}

// ============================================================================
// registerSessionClient — מימוש AUTH_OP_REGISTER ("זכור אותי להבא")
// רושם את המזהה שנלכד ב-grantSession. אידמפוטנטי דרך addTrusted.
// ============================================================================
bool AuthManager::registerSessionClient()
{
    if (!_sessionAuthenticated || _isAllZero(_sessionClientId))
    {
        // רשת ביטחון — ה-handler אמור לחסום NOT_AUTHENTICATED לפני
        Serial.println("[Auth] registerSessionClient without valid session");
        return false;
    }

    addTrusted(_sessionClientId);
    return true;
}

// ============================================================================
// addTrusted — הוספה עם פינוי FIFO, כתיבת NVS פעם אחת ורק אם השתנה
// ============================================================================
void AuthManager::addTrusted(const uint8_t* clientId)
{
    if (_isAllZero(clientId))
        return;

    // כבר קיים — אין כתיבה (מניעת שחיקת flash + אידמפוטנטיות של REGISTER)
    if (isTrusted(clientId))
        return;

    // shift שמאלה: slot 0 (הישן ביותר) נזרק, החדש נכנס בסוף
    memmove(_slots,
            _slots + AUTH_CLIENT_ID_SIZE,
            (AUTH_TRUSTED_SLOTS - 1) * AUTH_CLIENT_ID_SIZE);
    memcpy(_slots + (AUTH_TRUSTED_SLOTS - 1) * AUTH_CLIENT_ID_SIZE,
           clientId, AUTH_CLIENT_ID_SIZE);

    _saveSlots();
    Serial.println("[Auth] New trusted client registered (FIFO)");
}

// ============================================================================
// setPin — שינוי PIN (כבר עבר ולידציית טווח ב-CoreService)
// ============================================================================
void AuthManager::setPin(uint16_t newPin)
{
    if (newPin == _pin)
        return; // אין שינוי — אין כתיבה

    _pin = newPin;
    _prefs.putUShort(NVS_KEY_PIN, _pin);
    Serial.println("[Auth] PIN updated");
}

// ============================================================================
// factoryReset — master revoke: PIN=1111 + ניקוי כל ה-slots
// (נקרא מקוד התפריט על BACK+OK למשך 10 שניות)
// ============================================================================
void AuthManager::factoryReset()
{
    _pin = AUTH_DEFAULT_PIN;
    _prefs.putUShort(NVS_KEY_PIN, _pin);

    memset(_slots, 0, SLOTS_BLOB_SIZE);
    _saveSlots();

    resetSession();
    Serial.println("[Auth] Factory reset: pin=1111, all slots cleared");
}

// ============================================================================
// Internals
// ============================================================================
bool AuthManager::_isAllZero(const uint8_t* id)
{
    for (int i = 0; i < AUTH_CLIENT_ID_SIZE; i++)
        if (id[i] != 0)
            return false;
    return true;
}

void AuthManager::_saveSlots()
{
    _prefs.putBytes(NVS_KEY_SLOTS, _slots, SLOTS_BLOB_SIZE);
}