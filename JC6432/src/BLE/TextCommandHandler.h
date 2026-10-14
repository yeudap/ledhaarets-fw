#ifndef TEXT_COMMAND_HANDLER_H
#define TEXT_COMMAND_HANDLER_H

#include <Arduino.h>

// קודי פקודה
#define READ_TEXT 0xF8          // קריאת טקסט
#define WRITE_TEXT 0xF7         // כתיבת טקסט
#define DELETE_TEXT 0xFD        // מחיקת טקסט
#define DISPLAY_TEXT 0xFC       // הצגת טקסט על המסך
#define SWAP_TEXTS 0xFB         // החלפת מיקום בין טקסטים
#define GET_TEXT_COUNT 0xF9     // בקשת מספר הטקסטים

// קודי תשובה
#define SUCCESS_CODE 0x00            // הצלחה
#define ERROR_CODE 0xFF         // שגיאה כללית

// קודי שגיאה ספציפיים
#define ERR_INVALID_INDEX 0x01  // אינדקס לא חוקי
#define ERR_INVALID_LENGTH 0x02 // אורך פקודה לא תקין
#define ERR_INVALID_FORMAT 0x03 // פורמט טקסט לא חוקי
#define ERR_EMPTY_TEXT 0x04     // טקסט ריק במיקום המבוקש
#define ERR_MEMORY 0x05         // שגיאת זיכרון
#define ERR_UNSUPPORTED 0x06    // פעולה לא נתמכת
#define ERR_WRITE_FAILED 0x07   // שגיאת כתיבה
#define ERR_READ_FAILED 0x08    // שגיאת קריאה
#define ERR_DELETE_FAILED 0x09  // שגיאת מחיקה

// מבנה להחזרת תוצאה מהפעולות
struct CommandResult {
    bool success;
    uint8_t responseCode;
    uint8_t errorCode;
    String text;
    uint8_t data1;
    uint8_t data2;
    
    // קונסטרקטור להצלחה
    static CommandResult ok(uint8_t data = 0) {
        CommandResult result;
        result.success = true;
        result.responseCode = SUCCESS_CODE;
        result.errorCode = 0;
        result.data1 = data;
        result.data2 = 0;
        return result;
    }
    
    // קונסטרקטור להצלחה עם שני ערכי מידע
    static CommandResult okWithData(uint8_t data1, uint8_t data2) {
        CommandResult result;
        result.success = true;
        result.responseCode = SUCCESS_CODE;
        result.errorCode = 0;
        result.data1 = data1;
        result.data2 = data2;
        return result;
    }
    
    // קונסטרקטור להצלחה עם טקסט
    static CommandResult okWithText(const String& text) {
        CommandResult result;
        result.success = true;
        result.responseCode = SUCCESS_CODE;
        result.errorCode = 0;
        result.text = text;
        return result;
    }

    // קונסטרקטור לשגיאה
    static CommandResult error(uint8_t specificError) {
        CommandResult result;
        result.success = false;
        result.responseCode = ERROR_CODE;
        result.errorCode = specificError;
        return result;
    }
};

// פונקציות לטיפול בפקודות
CommandResult handleReadText(uint8_t index);
CommandResult handleWriteText(uint8_t index, const String& text);
CommandResult handleDeleteText(uint8_t index);
CommandResult handleDisplayText(uint8_t index);
CommandResult handleSwapTexts(uint8_t index1, uint8_t index2);
CommandResult handleGetTextCount();
String getTextByIndex(uint8_t index);


#endif