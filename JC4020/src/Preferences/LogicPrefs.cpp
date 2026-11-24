#include "LogicPrefs.h"

// מופע ה-Preferences של מרחב "clk" - שייך לרישום (userPreferences.prefs)
static constexpr Preferences &prefs = userPreferences.prefs;

// הגדרות ההעדפות - רשומות constexpr ב-flash (2a.4): מפתח, ברירת מחדל, טיפוס, אינדקס, Preferences, טווח.
// static_assert: ברירת המחדל בתוך הטווח (לא לרשומות PR_OPEN).

static constexpr PrefDef<uint8_t> netzClkDef("netzClk", 0, PT_U8, 44, &prefs, PR_MIN_MAX, 0, 60);
static_assert(prefDefaultInRange(netzClkDef), "netzClk");
                        // time left to Netz
                        // 0-60

static constexpr PrefDef<uint16_t> intervalDef("interval", 30, PT_U16, 46, &prefs, PR_MIN_MAX, 5, 600, 5);
static_assert(prefDefaultInRange(intervalDef), "interval");
                        // change rate in seconds
                        // 5-600 (multiple by 5)
                        // use in 4020 only

static constexpr PrefDef<uint16_t> intervalIluyDef("intervalIluy", 30, PT_U16, 47, &prefs, PR_MIN_MAX, 5, 600, 5);
static_assert(prefDefaultInRange(intervalIluyDef), "intervalIluy");
                        // change rate in seconds
                        // 5-600 (multiple by 5)
                        // use in 6416 only

static constexpr PrefDef<uint16_t> intervalTimeDef("intervalTime", 30, PT_U16, 48, &prefs, PR_MIN_MAX, 5, 600, 5);
static_assert(prefDefaultInRange(intervalTimeDef), "intervalTime");
                        // change rate in seconds
                        // 5-600 (multiple by 5)
                        // use in 6416 only

static constexpr PrefDef<uint16_t> intervalInfoDef("intervalInfo", 30, PT_U16, 49, &prefs, PR_MIN_MAX, 5, 600, 5);
static_assert(prefDefaultInRange(intervalInfoDef), "intervalInfo");
                        // change rate in seconds
                        // 5-600 (multiple by 5)
                        // use in 6416 only

static constexpr PrefDef<uint8_t> dafHayomiDef("dafHayomi", 1, PT_U8, 50, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(dafHayomiDef), "dafHayomi");
                        // bool. show Daf hayomi bavli

static constexpr PrefDef<uint8_t> yerushalmiDef("yerushalmi", 0, PT_U8, 51, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(yerushalmiDef), "yerushalmi");
                        // bool. show Daf hayomi yerushalmi

static constexpr PrefDef<uint8_t> yomDef("yom", 0, PT_U8, 52, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(yomDef), "yom");
                        // bool. show yom bashavua

static constexpr PrefDef<uint8_t> parashaDef("parasha", 0, PT_U8, 53, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(parashaDef), "parasha");
                        // bool. show parashat hashavua

static constexpr PrefDef<uint8_t> haftaraDef("haftara", 1, PT_U8, 54, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(haftaraDef), "haftara");
                        // bool. show haftarat hashavua

static constexpr PrefDef<uint8_t> ivriDef("ivri", 0, PT_U8, 55, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(ivriDef), "ivri");
                        // bool. show taarich ivri begadol

static constexpr PrefDef<uint8_t> loaziDef("loazi", 0, PT_U8, 56, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(loaziDef), "loazi");
                        // bool. show taarich loazi begadol

static constexpr PrefDef<uint8_t> tfilaDef("tfila", 1, PT_U8, 57, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(tfilaDef), "tfila");
                        // bool. show meida lamitpalel

static constexpr PrefDef<uint8_t> moadimDef("moadim", 1, PT_U8, 58, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(moadimDef), "moadim");
                        // bool. izcur hagim - moadim - zom - hodesh

static constexpr PrefDef<uint8_t> showIsraeliDef("showIsraeli", 1, PT_U8, 59, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(showIsraeliDef), "showIsraeli");
                        // bool. izcur moadim israelim

static constexpr PrefDef<uint8_t> TAZDef("TAZ", 1, PT_U8, 60, &prefs, PR_MIN_MAX, 0, 2);
static_assert(prefDefaultInRange(TAZDef), "TAZ");
                        // zman talit - alot hashachar - zriha
                        // 0. off
                        // 1. on time
                        // 2. all day

static constexpr PrefDef<uint8_t> SOZTDef("SOZT", 1, PT_U8, 61, &prefs, PR_MIN_MAX, 0, 2);
static_assert(prefDefaultInRange(SOZTDef), "SOZT");
                        // sof zman tfila
                        // 0. off
                        // 1. on time
                        // 2. all day

static constexpr PrefDef<uint8_t> SOZKSDef("SOZKS", 1, PT_U8, 62, &prefs, PR_MIN_MAX, 0, 2);
static_assert(prefDefaultInRange(SOZKSDef), "SOZKS");
                        // sof zman kriat shma
                        // 0. off
                        // 1. on time
                        // 2. all day

static constexpr PrefDef<uint8_t> HMMDef("HMM", 1, PT_U8, 63, &prefs, PR_MIN_MAX, 0, 2);
static_assert(prefDefaultInRange(HMMDef), "HMM");
                        // chazot - mincha gdola - mincha ktana
                        // 0. off
                        // 1. on time
                        // 2. all day

static constexpr PrefDef<uint8_t> PSZDef("PSZ", 1, PT_U8, 64, &prefs, PR_MIN_MAX, 0, 2);
static_assert(prefDefaultInRange(PSZDef), "PSZ");
                        // plag hamincha - shkiaa - zeet hacochavim
                        // 0. off
                        // 1. on time
                        // 2. all day

static constexpr PrefDef<uint8_t> HAZDef("HAZ", 1, PT_U8, 65, &prefs, PR_MIN_MAX, 0, 2);
static_assert(prefDefaultInRange(HAZDef), "HAZ");
                        // hazot - alot hashachar - zriha
                        // 0. off
                        // 1. on time
                        // 2. all day

static constexpr PrefDef<uint8_t> ZSZ1Def("ZSZ1", 1, PT_U8, 66, &prefs, PR_MIN_MAX, 0, 2);
static_assert(prefDefaultInRange(ZSZ1Def), "ZSZ1");
                        // zriha - shkiaa - zeet hacochavim
                        // shishi shabat
                        // 0. off
                        // 1. on time
                        // 2. all day

static constexpr PrefDef<uint8_t> ZSZDef("ZSZ", 1, PT_U8, 67, &prefs, PR_MIN_MAX, 0, 2);
static_assert(prefDefaultInRange(ZSZDef), "ZSZ");
                        // zriha - shkiaa - zeet hacochavim
                        // yom hol
                        // 0. off
                        // 1. on time
                        // 2. all day

static constexpr PrefDef<uint8_t> ZYSDef("ZYS", 3, PT_U8, 68, &prefs, PR_MIN_MAX, 0, 3);
static_assert(prefDefaultInRange(ZYSDef), "ZYS");
                        // zriha - yom - shkiaa
                        // 0. off
                        // 1. yom hol
                        // 2. shishi shabat
                        // 3. all week

static constexpr PrefDef<uint8_t> ZPSDef("ZPS", 0, PT_U8, 69, &prefs, PR_MIN_MAX, 0, 3);
static_assert(prefDefaultInRange(ZPSDef), "ZPS");
                        // zriha - parasha - shkiaa
                        // 0. off
                        // 1. yom hol
                        // 2. shishi shabat
                        // 3. all week

static constexpr PrefDef<uint8_t> IPODef("IPO", 2, PT_U8, 70, &prefs, PR_MIN_MAX, 0, 3);
static_assert(prefDefaultInRange(IPODef), "IPO");
                        // knisat shabat - parasha - zeet shabat
                        // 0. off
                        // 1. yom hol
                        // 2. shishi shabat
                        // 3. all week

static constexpr PrefDef<uint8_t> IORTDef("IORT", 2, PT_U8, 71, &prefs, PR_MIN_MAX, 0, 3);
static_assert(prefDefaultInRange(IORTDef), "IORT");
                        // knisat shabat - zeet shabat - rabenu tam
                        // 0. off
                        // 1. yom hol
                        // 2. shishi shabat
                        // 3. all week

LogicPrefs::LogicPrefs() :
                        netzClk(netzClkDef),
                        interval(intervalDef),
                        intervalIluy(intervalIluyDef),
                        intervalTime(intervalTimeDef),
                        intervalInfo(intervalInfoDef),
                        dafHayomi(dafHayomiDef),
                        yerushalmi(yerushalmiDef),
                        yom(yomDef),
                        parasha(parashaDef),
                        haftara(haftaraDef),
                        ivri(ivriDef),
                        loazi(loaziDef),
                        tfila(tfilaDef),
                        moadim(moadimDef),
                        showIsraeli(showIsraeliDef),
                        TAZ(TAZDef),
                        SOZT(SOZTDef),
                        SOZKS(SOZKSDef),
                        HMM(HMMDef),
                        PSZ(PSZDef),
                        HAZ(HAZDef),
                        ZSZ1(ZSZ1Def),
                        ZSZ(ZSZDef),
                        ZYS(ZYSDef),
                        ZPS(ZPSDef),
                        IPO(IPODef),
                        IORT(IORTDef)
{
}

LogicPrefs logicPrefs;
