#include "SystemPrefs.h"

// מופע ה-Preferences של מרחב "clk" - שייך לרישום (userPreferences.prefs)
static constexpr Preferences &prefs = userPreferences.prefs;

// הגדרות ההעדפות - רשומות constexpr ב-flash (2a.4): מפתח, ברירת מחדל, טיפוס, אינדקס, Preferences, טווח.
// static_assert: ברירת המחדל בתוך הטווח (לא לרשומות PR_OPEN).

static constexpr PrefDef<uint8_t> btDef("bt", 1, PT_U8, 76, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(btDef), "bt");
                        // bluetooth on/off
                        // 0. off
                        // 1. on

SystemPrefs::SystemPrefs() :
                        bt(btDef)
{
}

SystemPrefs systemPrefs;
