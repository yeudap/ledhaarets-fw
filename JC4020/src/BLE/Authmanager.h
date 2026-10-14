#ifndef AUTH_MANAGER_H
#define AUTH_MANAGER_H

// =============================================================================
// AuthManager — אימות אפליקציה (DOMAIN_AUTH)
//
// הפרדה לוגית בין שני מושגים:
//   אימות  — "מי אתה, מותר לך בסשן הזה": HELLO/PIN מדליקים את דגל הסשן בלבד.
//            הצלחת PIN אינה כותבת דבר ל-NVS.
//   רישום — "זכור אותי להבא": REGISTER מוסיף את ה-clientId של הסשן הנוכחי
//            ל-slots (FIFO + NVS). האפליקציה קוראת לו רק עבור מכשיר שמור.
//
// מנהל ארבעה דברים:
//   1. דגל האימות של הסשן הנוכחי (per-connection, מתאפס בכל connect/disconnect)
//   2. ה-clientId של הסשן — נלכד בהצלחת אימות בלבד, משמש את REGISTER,
//      מתאפס יחד עם הסשן. כך לא ניתן לרשום מזהה שלא אומת.
//   3. ה-PIN — נשמר ב-NVS ב-namespace נפרד ("sec"), ברירת מחדל 1111
//   4. שלושה slots של clientId מהימנים (16B כל אחד) — פינוי FIFO
//
// עקרונות:
//   - כתיבת NVS של ה-slots רק ב-REGISTER מפורש של לקוח *חדש* (מניעת שחיקת flash)
//   - clientId שכולו אפסים לעולם אינו מהימן (מגן מהתאמה מקרית ל-slot ריק)
//   - factoryReset() הוא ה-master revoke: PIN=1111 + ניקוי כל ה-slots.
//     החיווט לקומבינציית הכפתורים (BACK+OK למשך 10s) נעשה בקוד התפריט.
// =============================================================================

#include <Arduino.h>
#include <Preferences.h>
#include "CoreProtocol.h"

class AuthManager
{
public:
    // פתיחת ה-namespace וטעינת PIN + slots. לקרוא פעם אחת ב-init של ה-Bluetooth.
    void init();

    // ---- מצב הסשן -----------------------------------------------------------
    bool isSessionAuthenticated() const { return _sessionAuthenticated; }

    // הענקת אימות לסשן + לכידת ה-clientId שאומת (לשימוש REGISTER).
    // נקרא משני מסלולי ההצלחה בלבד: HELLO-מוכר ו-PIN-נכון.
    void grantSession(const uint8_t* clientId);

    // איפוס מלא של הסשן: דגל + clientId. נקרא בכל connect/disconnect.
    void resetSession();

    // ---- בדיקות -------------------------------------------------------------
    // האם ה-clientId (16B) נמצא באחד ה-slots. clientId אפסים → תמיד false.
    bool isTrusted(const uint8_t* clientId) const;

    // השוואת PIN מול הערך השמור
    bool checkPin(uint16_t pin) const { return pin == _pin; }

    // ---- עדכונים ------------------------------------------------------------
    // רישום ה-clientId של הסשן הנוכחי כ-trusted (מימוש AUTH_OP_REGISTER).
    // אידמפוטנטי: לקוח שכבר רשום → true ללא כתיבה. דורש סשן מאומת עם
    // clientId תקף (לא-אפסים); אחרת false — רשת ביטחון, לא אמור לקרות
    // כי ה-handler חוסם NOT_AUTHENTICATED לפני.
    bool registerSessionClient();

    // שינוי PIN (מנותב מ-PREF_OP_WRITE עם המפתח AUTH_PIN_PREF_KEY)
    void setPin(uint16_t newPin);

    // איפוס פיזי מהמכשיר: PIN=1111 + ניקוי כל ה-slots
    void factoryReset();

private:
    static constexpr size_t SLOTS_BLOB_SIZE =
        AUTH_TRUSTED_SLOTS * AUTH_CLIENT_ID_SIZE; // 48B

    Preferences _prefs;                       // namespace "sec"
    uint16_t    _pin = AUTH_DEFAULT_PIN;
    uint8_t     _slots[SLOTS_BLOB_SIZE] = {0};
    bool        _sessionAuthenticated = false;
    uint8_t     _sessionClientId[AUTH_CLIENT_ID_SIZE] = {0}; // אפסים = אין

    // הוספת clientId ל-slots עם פינוי FIFO. אם כבר קיים — אין כתיבה.
    // פרטי — הכניסה היחידה היא registerSessionClient (מזהה מאומת בלבד).
    void addTrusted(const uint8_t* clientId);

    static bool _isAllZero(const uint8_t* id);
    void        _saveSlots();
};

// instance גלובלי — מוגדר ב-AuthManager.cpp
extern AuthManager authManager;

#endif // AUTH_MANAGER_H