#ifndef COMMANDS_H
#define COMMANDS_H

// =============================================================================
// Commands.h
// הגדרת פקודות ה-state machine של השעון.
//
// הקובץ הזה מגדיר את הפקודות שה-state machine מבין — לא משנה מאיפה הן
// מגיעות: כפתור פיזי, BLE, או כל מקור אחר בעתיד.
//
// תלויות: ללא — קובץ זה אינו תלוי ב-BLE, ב-Arduino, או בשום דבר חיצוני.
//
// globals:
//   currentButtonCommand — מוגדר ב-CoreService.cpp
//   currentMenuCommand   — מוגדר ב-CoreService.cpp
//   מאופסים ב-main loop לאחר כל iteration
// =============================================================================

// -----------------------------------------------------------------------------
// ButtonCommand — פקודות כפתור פיזי / NAV BLE
// -----------------------------------------------------------------------------
enum class ButtonCommand {
    NONE = 0,
    MENU = 1,
    UP   = 2,
    DOWN = 3,
    OK   = 4,
};

// -----------------------------------------------------------------------------
// MenuCommand — פקודות תפריט / MENU BLE
// ערכים תואמים ל-MENU_CODE_* ב-CoreProtocol.h
// -----------------------------------------------------------------------------
enum class MenuCommand {
    NONE = 0,           // ללא פקודה

    // MainMenu (1-10)
    MAIN         = 1,   // תפריט ראשי
    CLOCK        = 2,   // שעה ותאריך
    DISPLAY1     = 3,   // תצוגה
    MINHAG       = 4,   // מנהגים
    MAKOM        = 5,   // מקום
    ZMANIM       = 6,   // זמנים
    CONTENT      = 7,   // תוכן
    RESET        = 8,   // איפוס
    ABOUT        = 9,   // אודות
    MAIN_SCREEN  = 10,  // מסך ראשי
    CONFIG       = 11,  // הגדרות

    // ClockMenu (20-29)
    SET_DATETIME = 20,  // כיוון שעה ותאריך
    TIME_FORMAT  = 21,  // מבנה שעה
    DATE_FORMAT  = 22,  // מבנה תאריך
    TIME_STRUCT  = 23,  // מבנה זמן
    NEXT_DAY     = 24,  // ליל מחר לאחר שקיעה
    DST          = 25,  // שעון קיץ

    // DisplayMenu (40-49)
    COLOR        = 40,  // צבע
    BRIGHTNESS   = 41,  // בהירות
    FONT         = 42,  // גופן

    // LocationMenu (50-59)
    SELECT_HORIZON = 50, // בחר אופק
    SET_LOCATION   = 51, // הגדר מקום
    WALLED_CITY    = 52, // עיר מוקפת חומה

    // TimeSettingsMenu (60-69)
    SELECT_METHOD      = 60, // בחר שיטה
    DESTRUCTION_YEARS  = 61, // שנים לחורבן
    EARLY_CANDLES      = 62, // הדלקת נרות מוקדמת
    SHOW_ALL_TIMES     = 63, // הצגת כל זמני היום

    // ContentMenu (70-79)
    EDIT         = 70,  // ערוך
    ENTER_TEXT   = 71,  // הזן טקסט
    CHANGE_RATE  = 72,  // קצב החלפה

    // CustomsMenu (80-89)
    DIASPORA     = 80,  // בני חו"ל
};

// -----------------------------------------------------------------------------
// Global declarations — מוגדרים ב-CoreService.cpp
// -----------------------------------------------------------------------------
extern ButtonCommand currentButtonCommand;
extern MenuCommand   currentMenuCommand;

#endif // COMMANDS_H