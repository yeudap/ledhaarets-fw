#include "PresentationPrefs.h"

// מופע ה-Preferences של מרחב "clk" - שייך לרישום (userPreferences.prefs)
static constexpr Preferences &prefs = userPreferences.prefs;

// הגדרות ההעדפות - רשומות constexpr ב-flash (2a.4): מפתח, ברירת מחדל, טיפוס, אינדקס, Preferences, טווח.
// static_assert: ברירת המחדל בתוך הטווח (לא לרשומות PR_OPEN).

// LayoutPrefs
static constexpr PrefDef<uint8_t> middleLineDef("middleLine", 1, PT_U8, 26, &prefs, PR_MIN_MAX, 1, 2);
static_assert(prefDefaultInRange(middleLineDef), "middleLine");
                        // 1. Clock only
                        // 2. Clock + Day

static constexpr PrefDef<uint8_t> upLineDef("upLine", 1, PT_U8, 27, &prefs, PR_MIN_MAX, 1, 3);
static_assert(prefDefaultInRange(upLineDef), "upLine");
                        // 1. Heb date + Loazi Date
                        // 2. Heb date + Day
                        // 3. Heb date only

static constexpr PrefDef<uint8_t> screenDef("screen", 2, PT_U8, 75, &prefs, PR_OPEN);
                        // main screens
                        // 1. digit only  - Big
                        // 2. yom + ivri + loazi + digit  - Small
                        // 3. yom + loazi + digit
                        // 4. yom + ivri + digit

// PresentationPrefs
static constexpr PrefDef<uint8_t> safaDef("safa", 1, PT_U8, 13, &prefs, PR_OPEN);
                        // 1. anglit
                        // 2. rusit

static constexpr PrefDef<uint8_t> dateFormatDef("dateFormat", 1, PT_U8, 22, &prefs, PR_MIN_MAX, 1, 8);
static_assert(prefDefaultInRange(dateFormatDef), "dateFormat");
                        // 1. 14/03/2023
                        // 2. 14/3/2023
                        // 3. 14/3/23
                        // 4. 14/3
                        // 5. 03/14/2023
                        // 6. 2023-03-14
                        // 7. 14.03.2023
                        // 8. without loazi date

static constexpr uint8_t clockFormatValues[] = {12, 24};
static constexpr PrefDef<uint8_t> clockFormatDef("clockFormat", 24, PT_U8, 23, &prefs, clockFormatValues);
static_assert(prefDefaultInRange(clockFormatDef), "clockFormat");
                        // 12
                        // 24

static constexpr PrefDef<uint8_t> timeFormatDef("timeFormat", 0, PT_U8, 24, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(timeFormatDef), "timeFormat");
                        // bool. leading zero in time

static constexpr PrefDef<uint8_t> leilDef("leil", 1, PT_U8, 43, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(leilDef), "leil");
                        // bool. after shkiaa
                        // 0. yom today
                        // 1. leil tomorrow

LayoutPrefs::LayoutPrefs() :
                        middleLine(middleLineDef),
                        upLine(upLineDef),
                        screen(screenDef)
{
}

PresentationPrefs::PresentationPrefs() :
                        safa(safaDef),
                        dateFormat(dateFormatDef),
                        clockFormat(clockFormatDef),
                        timeFormat(timeFormatDef),
                        leil(leilDef)
{
}

PresentationPrefs presentationPrefs;
