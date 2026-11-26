
#include "UserPreferences.h"
#include <States/States.h>
#include "DataPrefs.h"
#include "LogicPrefs.h"
#include "PresentationPrefs.h"
#include "DisplayPrefs.h"
#include "SystemPrefs.h"
#include "UserContent.h"

// מופע ה-Preferences של מרחב "clk" - שייך לרישום (userPreferences.prefs)
static constexpr Preferences &prefs = userPreferences.prefs;

// הגדרת forceWrite - רשומה constexpr ב-flash (2a.4)
static constexpr PrefDef<uint8_t> forceWriteDef("forceWrite", 0, PT_U8, 1, &prefs, PR_OPEN);

UserPreferences::UserPreferences() : forceWrite(forceWriteDef)
{
}

// טבלת הבדיקות הרשומות (2b.1). מאותחלת לאפס לפני כל בנאי, ונמלאת באתחול (registerCheck).
UserPreferences::CheckEntry UserPreferences::checks[UserPreferences::maxChecks];
size_t UserPreferences::numChecks = 0;

// כל ההעדפות לגישה גנרית - מערך קבוע ב-flash (2a.4). הסדר: textPrefs ואחריהן simplePrefs (allPrefs).
UserPrefBase *const UserPreferences::allPrefsArray[numTextPrefs + numSimplePrefs] = {
        // textPrefs
        &userContent.textInput1,
        &userContent.textInput2,
        &userContent.textInput3,
        &userContent.textInput4,
        &userContent.textInput5,
        &userContent.textInput6,
        &userContent.textInput7,
        &userContent.textInput8,
        &userContent.textInput9,
        &userContent.textInput10,
        &userContent.textInput11,
        &userContent.textInput12,
        &userContent.textInput13,
        &userContent.textInput14,
        &userContent.textInput15,
        &userContent.textInput16,
        &userContent.textInput17,
        &userContent.textInput18,
        &userContent.textInput19,
        &userContent.textInput20,
        &userContent.textInput21,
        &userContent.textInput22,
        &userContent.textInput23,
        &userContent.textInput24,
        &userContent.textInput25,
        &userContent.textInput26,
        &userContent.textInput27,
        &userContent.textInput28,
        &userContent.textInput29,
        &userContent.textInput30,
        &userContent.textInput31,
        &userContent.textInput32,
        &userContent.textInput33,
        &userContent.textInput34,
        &userContent.textInput35,
        &userContent.textInput36,
        &userContent.textInput37,
        &userContent.textInput38,
        &userContent.textInput39,
        &userContent.textInput40,
        &userContent.textInput41,
        &userContent.textInput42,
        &userContent.textInput43,
        &userContent.textInput44,
        &userContent.textInput45,
        &userContent.textInput46,
        &userContent.textInput47,
        &userContent.textInput48,
        &userContent.textInput49,
        &userContent.textInput50,

        // simplePrefs
        &userPreferences.forceWrite,
        &dataPrefs.place.textPlace,
        &dataPrefs.zmanim.shita,
        &dataPrefs.minhagim.eda,
        &dataPrefs.place.ofek,
        &dataPrefs.place.nz_lat,
        &dataPrefs.place.nz_long,
        &dataPrefs.place.nz_height,
        &dataPrefs.place.nz_hul,
        &dataPrefs.place.timezone,
        &displayPrefs.brightness,
        &presentationPrefs.safa,
        &displayPrefs.colorUp,
        &displayPrefs.colorMiddle,
        &displayPrefs.colorBottomT,
        &displayPrefs.colorBottomV,
        &displayPrefs.colorYom,
        &displayPrefs.colorParasha,
        &displayPrefs.colorIluy,
        &displayPrefs.clockFont,
        &presentationPrefs.dateFormat,
        &presentationPrefs.clockFormat,
        &presentationPrefs.timeFormat,
        &dataPrefs.place.shaonKaiz,
        &presentationPrefs.layout.middleLine,
        &presentationPrefs.layout.upLine,
        &dataPrefs.zmanim.zriha,
        &dataPrefs.zmanim.shkiaa,
        &dataPrefs.zmanim.rabenuTam,
        &dataPrefs.zmanim.zeetCochavim,
        &dataPrefs.zmanim.zmanTalit,
        &dataPrefs.zmanim.alotHashahar,
        &dataPrefs.zmanim.minhaG,
        &dataPrefs.zmanim.sozksMga,
        &dataPrefs.zmanim.soztMga,
        &dataPrefs.zmanim.plagMinha,
        &dataPrefs.zmanim.zeetShabat,
        &dataPrefs.zmanim.zeetZom,
        &dataPrefs.zmanim.nerot,
        &dataPrefs.place.homa,
        &dataPrefs.minhagim.hul,
        &presentationPrefs.leil,
        &logicPrefs.netzClk,
        &dataPrefs.minhagim.shanim,
        &logicPrefs.interval,
        &logicPrefs.intervalIluy,
        &logicPrefs.intervalTime,
        &logicPrefs.intervalInfo,
        &logicPrefs.dafHayomi,
        &logicPrefs.yerushalmi,
        &logicPrefs.yom,
        &logicPrefs.parasha,
        &logicPrefs.haftara,
        &logicPrefs.ivri,
        &logicPrefs.loazi,
        &logicPrefs.tfila,
        &logicPrefs.moadim,
        &logicPrefs.showIsraeli,
        &logicPrefs.TAZ,
        &logicPrefs.SOZT,
        &logicPrefs.SOZKS,
        &logicPrefs.HMM,
        &logicPrefs.PSZ,
        &logicPrefs.HAZ,
        &logicPrefs.ZSZ1,
        &logicPrefs.ZSZ,
        &logicPrefs.ZYS,
        &logicPrefs.ZPS,
        &logicPrefs.IPO,
        &logicPrefs.IORT,
        &displayPrefs.colorIvri,
        &displayPrefs.colorLoazi,
        &displayPrefs.colorParashaV,
        &presentationPrefs.layout.screen,
        &systemPrefs.bt,
};

UserPreferences userPreferences;

void setupPreferences()
{
    userPreferences.init("clk");
    userPreferences.loadAll();
}