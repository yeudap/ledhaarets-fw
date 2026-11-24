#include "DataPrefs.h"

// מופע ה-Preferences של מרחב "clk" - שייך לרישום (userPreferences.prefs)
static constexpr Preferences &prefs = userPreferences.prefs;

// הגדרות ההעדפות - רשומות constexpr ב-flash (2a.4): מפתח, ברירת מחדל, טיפוס, אינדקס, Preferences, טווח.
// static_assert: ברירת המחדל בתוך הטווח (לא לרשומות PR_OPEN).

// PlacePrefs
static constexpr PrefDef<String> textPlaceDef("textPlace", "whaht otwun", PT_STR, 2, &prefs, PR_MIN_MAX, 0, 19);
static_assert(prefDefaultInRange(textPlaceDef), "textPlace");
                        // input custom place name

static constexpr PrefDef<uint8_t> ofekDef("ofek", 1, PT_U8, 6, &prefs, PR_MIN_MAX, 0, 94);
static_assert(prefDefaultInRange(ofekDef), "ofek");
                        // ofek index

static constexpr PrefDef<double_t> nz_latDef("nz_lat", 32.07, PT_BLOB, 7, &prefs, PR_OPEN);
                        // latitude (valid values -90 to 90)

static constexpr PrefDef<double_t> nz_longDef("nz_long", 34.78, PT_BLOB, 8, &prefs, PR_OPEN);
                        // longitude (valid values -180 to 180)

static constexpr PrefDef<int16_t> nz_heightDef("nz_height", 20, PT_I16, 9, &prefs, PR_OPEN);
                        // city height

static constexpr PrefDef<uint8_t> nz_hulDef("nz_hul", 0, PT_U8, 10, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(nz_hulDef), "nz_hul");
                        // place country
                        // 0. israel
                        // 1. hul

static constexpr PrefDef<int8_t> timezoneDef("timezone", 2, PT_I8, 11, &prefs, PR_OPEN);
                        // timezone (valid values -12 to 14)

static constexpr PrefDef<uint8_t> shaonKaizDef("shaonKaiz", 1, PT_U8, 25, &prefs, PR_MIN_MAX, 1, 3);
static_assert(prefDefaultInRange(shaonKaizDef), "shaonKaiz");
                        // 1. automatic
                        // 2. shaon kaiz, timeZone +1
                        // 3. shaon horef, timeZone

static constexpr PrefDef<uint8_t> homaDef("homa", 0, PT_U8, 41, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(homaDef), "homa");
                        // 0. lo mukefet homa
                        // 1. mukefet homa

// ZmanimPrefs
static constexpr PrefDef<uint8_t> shitaDef("shita", 0, PT_U8, 4, &prefs, PR_MIN_MAX, 0, 9);
static_assert(prefDefaultInRange(shitaDef), "shita");
                        // 0. Or hachayim
                        // 1. Kise rachamim
                        // 2. chazon shamaim
                        // 3. harav melamed
                        // 4. habad
                        // 5. habad - harav singawi
                        // 6. harav tucazinsky
                        // 7. ben ish chai

static constexpr PrefDef<uint8_t> zrihaDef("zriha", 2, PT_U8, 28, &prefs, PR_MIN_MAX, 1, 3);
static_assert(prefDefaultInRange(zrihaDef), "zriha");
                        // 1. Mishorit
                        // 2. Haniret
                        // 3. Astronomit

static constexpr PrefDef<uint8_t> shkiaaDef("shkiaa", 3, PT_U8, 29, &prefs, PR_MIN_MAX, 1, 3);
static_assert(prefDefaultInRange(shkiaaDef), "shkiaa");
                        // 1. Mishorit
                        // 2. Haniret
                        // 3. Astronomit

static constexpr PrefDef<uint8_t> rabenuTamDef("rabenuTam", 20, PT_U8, 30, &prefs, PR_MIN_MAX, 1, 28);
static_assert(prefDefaultInRange(rabenuTamDef), "rabenuTam");
                        // hishuv rabenu tam

static constexpr PrefDef<uint8_t> zeetCochavimDef("zeetCochavim", 8, PT_U8, 31, &prefs, PR_MIN_MAX, 1, 28);
static_assert(prefDefaultInRange(zeetCochavimDef), "zeetCochavim");
                        // hishuv zeet cochavim

static constexpr PrefDef<uint8_t> zmanTalitDef("zmanTalit", 4, PT_U8, 32, &prefs, PR_MIN_MAX, 1, 10);
static_assert(prefDefaultInRange(zmanTalitDef), "zmanTalit");
                        // hishuv zman talit utfilin

static constexpr PrefDef<uint8_t> alotHashaharDef("alotHashahar", 4, PT_U8, 33, &prefs, PR_MIN_MAX, 1, 7);
static_assert(prefDefaultInRange(alotHashaharDef), "alotHashahar");
                        // hishuv zman alot hashahar

static constexpr PrefDef<uint8_t> minhaGDef("minhaG", 3, PT_U8, 34, &prefs, PR_MIN_MAX, 1, 3);
static_assert(prefDefaultInRange(minhaGDef), "minhaG");
                        // hishuv zman minha gdola

static constexpr PrefDef<uint8_t> sozksMgaDef("sozksMga", 1, PT_U8, 35, &prefs, PR_MIN_MAX, 1, 4);
static_assert(prefDefaultInRange(sozksMgaDef), "sozksMga");
                        // hishuv zman sof zman kriat shma mga

static constexpr PrefDef<uint8_t> soztMgaDef("soztMga", 1, PT_U8, 36, &prefs, PR_MIN_MAX, 1, 4);
static_assert(prefDefaultInRange(soztMgaDef), "soztMga");
                        // hishuv zman sof zman tfila mga

static constexpr PrefDef<uint8_t> plagMinhaDef("plagMinha", 1, PT_U8, 37, &prefs, PR_MIN_MAX, 1, 3);
static_assert(prefDefaultInRange(plagMinhaDef), "plagMinha");
                        // hishuv zman plag haminha

static constexpr PrefDef<uint8_t> zeetShabatDef("zeetShabat", 16, PT_U8, 38, &prefs, PR_MIN_MAX, 1, 28);
static_assert(prefDefaultInRange(zeetShabatDef), "zeetShabat");
                        // hishuv zman zeet shabat

static constexpr PrefDef<uint8_t> zeetZomDef("zeetZom", 14, PT_U8, 39, &prefs, PR_MIN_MAX, 1, 28);
static_assert(prefDefaultInRange(zeetZomDef), "zeetZom");
                        // hishuv zman zeet zom derabanan

static constexpr PrefDef<uint8_t> nerotDef("nerot", 22, PT_U8, 40, &prefs, PR_OPEN);
                        // hadlakat nerot mukdemet
                        // 0 - 40

// MinhagimPrefs
static constexpr PrefDef<uint8_t> edaDef("eda", 1, PT_U8, 5, &prefs, PR_MIN_MAX, 1, 6);
static_assert(prefDefaultInRange(edaDef), "eda");
                        // 1. Edot hamizrah
                        // 2. sfarad
                        // 3. ashkenaz
                        // 4. teiman - baladi
                        // 5. teiman - shami
                        // 6. chabad

static constexpr PrefDef<uint8_t> hulDef("hul", 0, PT_U8, 42, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(hulDef), "hul");
                        // 0. israel
                        // 1. bnei hul

static constexpr PrefDef<uint8_t> shanimDef("shanim", 0, PT_U8, 45, &prefs, PR_MIN_MAX, 0, 1);
static_assert(prefDefaultInRange(shanimDef), "shanim");
                        // time from hurban bayit sheni
                        // 0. 1954
                        // 1. 1955

PlacePrefs::PlacePrefs() :
                        textPlace(textPlaceDef),
                        ofek(ofekDef),
                        nz_lat(nz_latDef),
                        nz_long(nz_longDef),
                        nz_height(nz_heightDef),
                        nz_hul(nz_hulDef),
                        timezone(timezoneDef),
                        shaonKaiz(shaonKaizDef),
                        homa(homaDef)
{
}

ZmanimPrefs::ZmanimPrefs() :
                        shita(shitaDef),
                        zriha(zrihaDef),
                        shkiaa(shkiaaDef),
                        rabenuTam(rabenuTamDef),
                        zeetCochavim(zeetCochavimDef),
                        zmanTalit(zmanTalitDef),
                        alotHashahar(alotHashaharDef),
                        minhaG(minhaGDef),
                        sozksMga(sozksMgaDef),
                        soztMga(soztMgaDef),
                        plagMinha(plagMinhaDef),
                        zeetShabat(zeetShabatDef),
                        zeetZom(zeetZomDef),
                        nerot(nerotDef)
{
}

MinhagimPrefs::MinhagimPrefs() :
                        eda(edaDef),
                        hul(hulDef),
                        shanim(shanimDef)
{
}

DataPrefs dataPrefs;
