#ifndef CORE_PROTOCOL_H
#define CORE_PROTOCOL_H

// =============================================================================
// CoreProtocol.h
// מפרט הפרוטוקול המאוחד של שירות ה-BLE המרכזי
//
// מבנה פקודה (CMD: Flutter → Watch):
//   Byte 0:   DOMAIN
//   Byte 1:   OPCODE
//   Byte 2-N: PAYLOAD (תלוי ב-opcode)
//
// מבנה תגובה (EVT: Watch → Flutter, via Notify):
//   Byte 0:   DOMAIN  (echo של ה-domain שנשלח)
//   Byte 1:   OPCODE  (echo של ה-opcode שנשלח)
//   Byte 2:   STATUS  (0x00=OK, 0x01=ERROR)
//   Byte 3-N: DATA    (תלוי ב-opcode, יכול להיות ריק)
//
// גרסת פרוטוקול: 1.1 (נוסף DOMAIN_AUTH)
// גרסת FW מינימלית: 7.0
// =============================================================================

// -----------------------------------------------------------------------------
// FW Version Gate
// שעון עם major version < MIN_FW_MAJOR_VERSION חייב לעבור OTA לפני שימוש
// -----------------------------------------------------------------------------
#define MIN_FW_MAJOR_VERSION  7

// -----------------------------------------------------------------------------
// Packet sizes
// -----------------------------------------------------------------------------
#define CORE_HEADER_SIZE      2   // DOMAIN + OPCODE
#define CORE_EVT_HEADER_SIZE  3   // DOMAIN + OPCODE + STATUS

// -----------------------------------------------------------------------------
// STATUS codes (Byte 2 ב-EVT)
// -----------------------------------------------------------------------------
#define CORE_STATUS_OK        0x00
#define CORE_STATUS_ERROR     0x01

// -----------------------------------------------------------------------------
// Error codes (Byte 3 ב-EVT כאשר STATUS == CORE_STATUS_ERROR)
// -----------------------------------------------------------------------------
#define CORE_ERR_UNKNOWN_DOMAIN   0x01  // domain לא מוכר
#define CORE_ERR_UNKNOWN_OPCODE   0x02  // opcode לא מוכר ב-domain זה
#define CORE_ERR_INVALID_PAYLOAD  0x03  // payload קצר מדי או לא תקין
#define CORE_ERR_INVALID_INDEX    0x04  // אינדקס מחוץ לטווח (TEXT)
#define CORE_ERR_INVALID_VALUE    0x05  // ערך לא חוקי
#define CORE_ERR_EMPTY            0x06  // רשומה ריקה (טקסט ריק)
#define CORE_ERR_WRITE_FAILED     0x07  // כתיבה לזיכרון נכשלה
#define CORE_ERR_TYPE_MISMATCH    0x08  // סוג נתון לא תואם ל-preference key
#define CORE_ERR_INVALID_KEY      0x09  // preference key לא קיים
#define CORE_ERR_INVALID_TIME     0x0A  // ערכי שעה לא חוקיים
#define CORE_ERR_AUTH_REQUIRED    0x0B  // AUTH: clientId לא מוכר — נדרש PIN (מענה ל-HELLO)
#define CORE_ERR_AUTH_FAILED      0x0C  // AUTH: PIN שגוי (מענה ל-PIN, אחרי השהיית 1s)
#define CORE_ERR_NOT_AUTHENTICATED 0x0D // פקודה בדומיין אחר לפני השלמת אימות
#define CORE_ERR_INTERNAL         0xFF  // שגיאה פנימית כללית

// =============================================================================
// DOMAIN codes
// =============================================================================
#define DOMAIN_NAV    0x01  // ניווט וכפתורים
#define DOMAIN_MENU   0x02  // ניהול תפריט
#define DOMAIN_TEXT   0x03  // ניהול טקסטים
#define DOMAIN_TIME   0x04  // סנכרון שעה
#define DOMAIN_PREF   0x05  // העדפות והגדרות (כולל מיקום)
#define DOMAIN_AUTH   0x06  // אימות אפליקציה (PIN + trusted clients)

// =============================================================================
// NAV domain (0x01) — ניווט וכפתורים
// =============================================================================

// Opcodes
#define NAV_OP_PRESS  0x01  // לחיצת כפתור
                            // CMD:  [DOMAIN][NAV_OP_PRESS][key 1B]
                            // EVT:  [DOMAIN][NAV_OP_PRESS][STATUS]

// Key codes (payload של NAV_OP_PRESS)
#define NAV_KEY_UP    0x01
#define NAV_KEY_DOWN  0x02
#define NAV_KEY_MENU  0x03
#define NAV_KEY_OK    0x04

// =============================================================================
// MENU domain (0x02) — ניהול תפריט
// =============================================================================

// Opcodes
#define MENU_OP_GOTO  0x01  // מעבר לפריט תפריט
                            // CMD:  [DOMAIN][MENU_OP_GOTO][menu_code 1B]
                            // EVT:  [DOMAIN][MENU_OP_GOTO][STATUS]

// Menu codes (payload של MENU_OP_GOTO) — תואם ל-MenuCommand enum הקיים
// MainMenu (1-10)
#define MENU_CODE_MAIN         1
#define MENU_CODE_CLOCK        2
#define MENU_CODE_DISPLAY      3
#define MENU_CODE_MINHAG       4
#define MENU_CODE_MAKOM        5
#define MENU_CODE_ZMANIM       6
#define MENU_CODE_CONTENT      7
#define MENU_CODE_RESET        8
#define MENU_CODE_ABOUT        9
#define MENU_CODE_MAIN_SCREEN  10
#define MENU_CODE_CONFIG       11

// ClockMenu (20-29)
#define MENU_CODE_SET_DATETIME  20
#define MENU_CODE_TIME_FORMAT   21
#define MENU_CODE_DATE_FORMAT   22
#define MENU_CODE_TIME_STRUCT   23
#define MENU_CODE_NEXT_DAY      24
#define MENU_CODE_DST           25

// DisplayMenu (40-49)
#define MENU_CODE_COLOR       40
#define MENU_CODE_BRIGHTNESS  41
#define MENU_CODE_FONT        42

// LocationMenu (50-59)
#define MENU_CODE_SELECT_HORIZON  50
#define MENU_CODE_SET_LOCATION    51
#define MENU_CODE_WALLED_CITY     52

// TimeSettingsMenu (60-69)
#define MENU_CODE_SELECT_METHOD      60
#define MENU_CODE_DESTRUCTION_YEARS  61
#define MENU_CODE_EARLY_CANDLES      62
#define MENU_CODE_SHOW_ALL_TIMES     63

// ContentMenu (70-79)
#define MENU_CODE_EDIT        70
#define MENU_CODE_ENTER_TEXT  71
#define MENU_CODE_CHANGE_RATE 72

// CustomsMenu (80-89)
#define MENU_CODE_DIASPORA  80

// =============================================================================
// TEXT domain (0x03) — ניהול טקסטים
// =============================================================================

// Opcodes
#define TEXT_OP_READ     0x01  // קריאת טקסט לפי אינדקס
                               // CMD:  [DOMAIN][TEXT_OP_READ][index 1B]
                               // EVT:  [DOMAIN][TEXT_OP_READ][STATUS][string bytes]

#define TEXT_OP_WRITE    0x02  // כתיבת טקסט לפי אינדקס
                               // CMD:  [DOMAIN][TEXT_OP_WRITE][index 1B][string bytes]
                               // EVT:  [DOMAIN][TEXT_OP_WRITE][STATUS][index 1B]

#define TEXT_OP_DELETE   0x03  // מחיקת טקסט לפי אינדקס (דוחס את הרשימה)
                               // CMD:  [DOMAIN][TEXT_OP_DELETE][index 1B]
                               // EVT:  [DOMAIN][TEXT_OP_DELETE][STATUS][index 1B]

#define TEXT_OP_DISPLAY  0x04  // הגדרת הטקסט הפעיל על המסך
                               // CMD:  [DOMAIN][TEXT_OP_DISPLAY][index 1B]
                               // EVT:  [DOMAIN][TEXT_OP_DISPLAY][STATUS][index 1B]

#define TEXT_OP_SWAP     0x05  // החלפת מיקום בין שני טקסטים
                               // CMD:  [DOMAIN][TEXT_OP_SWAP][idx_a 1B][idx_b 1B]
                               // EVT:  [DOMAIN][TEXT_OP_SWAP][STATUS][idx_a 1B][idx_b 1B]

#define TEXT_OP_COUNT    0x06  // בקשת מספר הטקסטים הלא-ריקים
                               // CMD:  [DOMAIN][TEXT_OP_COUNT]
                               // EVT:  [DOMAIN][TEXT_OP_COUNT][STATUS][count 1B]

// =============================================================================
// TIME domain (0x04) — סנכרון שעה
// =============================================================================

// Opcodes
#define TIME_OP_SET  0x01  // כתיבת שעה לשעון
                           // CMD:  [DOMAIN][TIME_OP_SET][7B time data]
                           // EVT:  [DOMAIN][TIME_OP_SET][STATUS]

#define TIME_OP_GET  0x02  // קריאת השעה הנוכחית מהשעון
                           // CMD:  [DOMAIN][TIME_OP_GET]
                           // EVT:  [DOMAIN][TIME_OP_GET][STATUS][7B time data]

// Time data format (7 bytes) — זהה ל-TimeData struct הקיים:
//   Byte 0: year  MSB
//   Byte 1: year  LSB
//   Byte 2: month  (1-12)
//   Byte 3: day    (1-31)
//   Byte 4: hour   (0-23)
//   Byte 5: minute (0-59)
//   Byte 6: second (0-59)
#define TIME_DATA_SIZE  7

// =============================================================================
// PREF domain (0x05) — העדפות והגדרות (כולל מיקום)
// =============================================================================

// Opcodes
#define PREF_OP_READ   0x01  // קריאת העדפה לפי מפתח
                             // CMD:  [DOMAIN][PREF_OP_READ][key string, null-terminated]
                             // EVT:  [DOMAIN][PREF_OP_READ][STATUS][pref_type 1B][value bytes]

#define PREF_OP_WRITE  0x02  // כתיבת העדפה לפי מפתח
                             // CMD:  [DOMAIN][PREF_OP_WRITE][pref_type 1B][key\0][value bytes]
                             // EVT:  [DOMAIN][PREF_OP_WRITE][STATUS]

#define PREF_OP_SYNC   0x03  // בקשת רשימת indices של העדפות שהשתנו מברירת המחדל
                             // CMD:  [DOMAIN][PREF_OP_SYNC]
                             // EVT:  [DOMAIN][PREF_OP_SYNC][STATUS][count 1B][index, index, ...]

// Preference type codes — תואם ל-PreferenceType enum בשני הצדדים
#define PREF_TYPE_I8    0x00  // int8_t   (1 byte,  signed)
#define PREF_TYPE_U8    0x01  // uint8_t  (1 byte,  unsigned)
#define PREF_TYPE_I16   0x02  // int16_t  (2 bytes, little-endian, signed)
#define PREF_TYPE_U16   0x03  // uint16_t (2 bytes, little-endian, unsigned)
#define PREF_TYPE_STR   0x08  // String   (N bytes UTF-8, ללא null terminator בערך עצמו)
#define PREF_TYPE_BLOB  0x09  // double   (8 bytes, little-endian IEEE 754)

// =============================================================================
// AUTH domain (0x06) — אימות אפליקציה (Application Authentication)
//
// זרימה: אחרי discovery האפליקציה שולחת HELLO עם clientId (16B, UUID v4 גולמי).
// clientId מוכר (אחד מ-3 ה-slots) → OK. לא מוכר → ERROR + AUTH_REQUIRED,
// והאפליקציה שולחת PIN. PIN נכון → OK + הוספת ה-clientId ל-slots (FIFO).
// PIN שגוי → השהיית שנייה אחת ואז ERROR + AUTH_FAILED.
// כל פקודה בדומיין אחר לפני אימות → ERROR + NOT_AUTHENTICATED.
//
// שינוי PIN: דרך PREF_OP_WRITE עם המפתח AUTH_PIN_PREF_KEY (טיפוס U16) —
// מטופל ב-special-case שמנתב ל-AuthManager (namespace נפרד ב-NVS), כך
// שה-PIN אינו קריא דרך PREF_OP_READ ואינו מדווח ב-PREF_OP_SYNC.
// =============================================================================

// Opcodes
#define AUTH_OP_HELLO  0x01  // הזדהות עם clientId
                             // CMD:  [DOMAIN][AUTH_OP_HELLO][clientId 16B]        (18B)
                             // EVT:  [DOMAIN][AUTH_OP_HELLO][STATUS]
                             //       ERROR + AUTH_REQUIRED אם ה-clientId לא מוכר

#define AUTH_OP_PIN    0x02  // אימות עם PIN
                             // CMD:  [DOMAIN][AUTH_OP_PIN][pin u16 LE 2B][clientId 16B] (20B)
                             // EVT:  [DOMAIN][AUTH_OP_PIN][STATUS]
                             //       ERROR + AUTH_FAILED אם ה-PIN שגוי (אחרי 1s)
                             
#define AUTH_OP_REGISTER 0x03  // רישום הלקוח המאומת כ-trusted ("זכור אותי להבא")
                               // CMD:  [DOMAIN][AUTH_OP_REGISTER]                 (2B)
                               //       payload ריק — נרשם ה-clientId שאומת בסשן
                               // EVT:  [DOMAIN][AUTH_OP_REGISTER][STATUS]
                               //       אידמפוטנטי; NOT_AUTHENTICATED לפני אימות
// 0x04 שמור ל-UNREGISTER עתידי — לא ממומש בכוונה

// Sizes
#define AUTH_CLIENT_ID_SIZE  16
#define AUTH_HELLO_MIN_SIZE  (CORE_HEADER_SIZE + AUTH_CLIENT_ID_SIZE)      // 18
#define AUTH_PIN_MIN_SIZE    (CORE_HEADER_SIZE + 2 + AUTH_CLIENT_ID_SIZE)  // 20

// Behavior constants
#define AUTH_TRUSTED_SLOTS   3      // מספר טלפונים מהימנים (פינוי FIFO)
#define AUTH_DEFAULT_PIN     1111   // ברירת מחדל + ערך אחרי איפוס פיזי
#define AUTH_MAX_PIN         9999   // טווח: 0000–9999
#define AUTH_FAIL_DELAY_MS   1000   // השהיה לפני מענה AUTH_FAILED (anti brute-force)

// PIN כ-preference: המפתח שהאפליקציה כותבת אליו דרך PREF_OP_WRITE
#define AUTH_PIN_PREF_KEY    "pin"

// =============================================================================
// Payload size minimums — לבדיקת תקינות ב-onWrite
// =============================================================================
#define NAV_CMD_MIN_SIZE    3   // DOMAIN + OPCODE + key
#define MENU_CMD_MIN_SIZE   3   // DOMAIN + OPCODE + menu_code
#define TEXT_READ_MIN_SIZE  3   // DOMAIN + OPCODE + index
#define TEXT_WRITE_MIN_SIZE 4   // DOMAIN + OPCODE + index + (לפחות byte אחד של טקסט)
#define TEXT_SWAP_MIN_SIZE  4   // DOMAIN + OPCODE + idx_a + idx_b
#define TIME_SET_MIN_SIZE   9   // DOMAIN + OPCODE + 7 bytes
#define PREF_READ_MIN_SIZE  4   // DOMAIN + OPCODE + key (לפחות תו אחד + null)
#define PREF_WRITE_MIN_SIZE 5   // DOMAIN + OPCODE + type + key\0 + value

// ============================================================================
// SYS domain — פעולות מערכת
// ============================================================================
#define DOMAIN_SYS              0x07

#define SYS_OP_RESTART          0x01
#define SYS_OP_SET_NAME         0x02
#define SYS_OP_GET_MAC          0x03

// קודי שגיאה ייעודיים (ב-DATA[0], לפי הדפוס הקיים)
#define SYS_ERR_NAME_TOO_LONG   0x01

// שם המכשיר
#define SYS_NAME_MAX_BYTES      24            // מרווח ביטחון מתחת ל-29 של scan response
#define SYS_DEFAULT_DEVICE_NAME "LedHaarets"  // ברירת מחדל כשלא הוגדר שם
#define SYS_NAME_NVS_NAMESPACE  "sys"
#define SYS_NAME_NVS_KEY        "dev_name"


#endif // CORE_PROTOCOL_H