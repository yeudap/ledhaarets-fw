#include "DisplayPrefs.h"

// מופע ה-Preferences של מרחב "clk" - שייך לרישום (userPreferences.prefs)
static constexpr Preferences &prefs = userPreferences.prefs;

// הגדרות ההעדפות - רשומות constexpr ב-flash (2a.4): מפתח, ברירת מחדל, טיפוס, אינדקס, Preferences, טווח.
// static_assert: ברירת המחדל בתוך הטווח (לא לרשומות PR_OPEN).

static constexpr PrefDef<uint8_t> brightnessDef("brightness", 200, PT_U8, 12, &prefs, PR_OPEN);
                        // 1-12  multiple by 20. (valid values 0-255)

static constexpr PrefDef<uint8_t> colorUpDef("colorUp", 1, PT_U8, 14, &prefs, PR_MIN_MAX, 0, 6);
static_assert(prefDefaultInRange(colorUpDef), "colorUp");
                        // 1-7

static constexpr PrefDef<uint8_t> colorMiddleDef("colorMiddle", 6, PT_U8, 15, &prefs, PR_MIN_MAX, 0, 6);
static_assert(prefDefaultInRange(colorMiddleDef), "colorMiddle");
                        // 1-7

static constexpr PrefDef<uint8_t> colorBottomTDef("colorBottomT", 1, PT_U8, 16, &prefs, PR_MIN_MAX, 0, 6);
static_assert(prefDefaultInRange(colorBottomTDef), "colorBottomT");
                        // 1-7

static constexpr PrefDef<uint8_t> colorBottomVDef("colorBottomV", 6, PT_U8, 17, &prefs, PR_MIN_MAX, 0, 6);
static_assert(prefDefaultInRange(colorBottomVDef), "colorBottomV");
                        // 1-7

static constexpr PrefDef<uint8_t> colorYomDef("colorYom", 1, PT_U8, 18, &prefs, PR_MIN_MAX, 0, 6);
static_assert(prefDefaultInRange(colorYomDef), "colorYom");
                        // 1-7

static constexpr PrefDef<uint8_t> colorParashaDef("colorParasha", 1, PT_U8, 19, &prefs, PR_MIN_MAX, 0, 6);
static_assert(prefDefaultInRange(colorParashaDef), "colorParasha");
                        // 1-7

static constexpr PrefDef<uint8_t> colorIluyDef("colorIluy", 6, PT_U8, 20, &prefs, PR_MIN_MAX, 0, 6);
static_assert(prefDefaultInRange(colorIluyDef), "colorIluy");
                        // 1-7

static constexpr PrefDef<uint8_t> clockFontDef("clockFont", 0, PT_U8, 21, &prefs, PR_MIN_MAX, 0, 2);
static_assert(prefDefaultInRange(clockFontDef), "clockFont");
                        // 0-4

static constexpr PrefDef<uint8_t> colorIvriDef("colorIvri", 6, PT_U8, 72, &prefs, PR_MIN_MAX, 0, 6);
static_assert(prefDefaultInRange(colorIvriDef), "colorIvri");
                        // 1-7

static constexpr PrefDef<uint8_t> colorLoaziDef("colorLoazi", 1, PT_U8, 73, &prefs, PR_MIN_MAX, 0, 6);
static_assert(prefDefaultInRange(colorLoaziDef), "colorLoazi");
                        // 1-7

static constexpr PrefDef<uint8_t> colorParashaVDef("colorParashaV", 6, PT_U8, 74, &prefs, PR_MIN_MAX, 0, 6);
static_assert(prefDefaultInRange(colorParashaVDef), "colorParashaV");
                        // 1-7

DisplayPrefs::DisplayPrefs() :
                        brightness(brightnessDef),
                        colorUp(colorUpDef),
                        colorMiddle(colorMiddleDef),
                        colorBottomT(colorBottomTDef),
                        colorBottomV(colorBottomVDef),
                        colorYom(colorYomDef),
                        colorParasha(colorParashaDef),
                        colorIluy(colorIluyDef),
                        clockFont(clockFontDef),
                        colorIvri(colorIvriDef),
                        colorLoazi(colorLoaziDef),
                        colorParashaV(colorParashaVDef)
{
}

DisplayPrefs displayPrefs;
