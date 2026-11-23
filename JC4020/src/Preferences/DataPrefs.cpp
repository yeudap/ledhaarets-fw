#include "DataPrefs.h"

// מופע ה-Preferences של מרחב "clk" - שייך לרישום (userPreferences.prefs)
static Preferences &prefs = userPreferences.prefs;

PlacePrefs::PlacePrefs() :
                        textPlace("textPlace", "whaht otwun", PT_STR, 2, &prefs),
                        // input custom place name

                        ofek("ofek", 1, PT_U8, 6, &prefs),
                        // ofek index

                        nz_lat("nz_lat", 32.07, PT_BLOB, 7, &prefs),
                        // latitude (valid values -90 to 90)

                        nz_long("nz_long", 34.78, PT_BLOB, 8, &prefs),
                        // longitude (valid values -180 to 180)

                        nz_height("nz_height", 20, PT_I16, 9, &prefs),
                        // city height

                        nz_hul("nz_hul", 0, PT_U8, 10, &prefs),
                        // place country
                        // 0. israel
                        // 1. hul

                        timezone("timezone", 2, PT_I8, 11, &prefs),
                        // timezone (valid values -12 to 14)

                        shaonKaiz("shaonKaiz", 1, PT_U8, 25, &prefs),
                        // 1. automatic
                        // 2. shaon kaiz, timeZone +1
                        // 3. shaon horef, timeZone

                        homa("homa", 0, PT_U8, 41, &prefs)
                        // 0. lo mukefet homa
                        // 1. mukefet homa
{
}

ZmanimPrefs::ZmanimPrefs() :
                        shita("shita", 0, PT_U8, 4, &prefs),
                        // 0. Or hachayim
                        // 1. Kise rachamim
                        // 2. chazon shamaim
                        // 3. harav melamed
                        // 4. habad
                        // 5. habad - harav singawi
                        // 6. harav tucazinsky
                        // 7. ben ish chai

                        zriha("zriha", 2, PT_U8, 28, &prefs),
                        // 1. Mishorit
                        // 2. Haniret
                        // 3. Astronomit

                        shkiaa("shkiaa", 3, PT_U8, 29, &prefs),
                        // 1. Mishorit
                        // 2. Haniret
                        // 3. Astronomit

                        rabenuTam("rabenuTam", 20, PT_U8, 30, &prefs),
                        // hishuv rabenu tam

                        zeetCochavim("zeetCochavim", 8, PT_U8, 31, &prefs),
                        // hishuv zeet cochavim

                        zmanTalit("zmanTalit", 4, PT_U8, 32, &prefs),
                        // hishuv zman talit utfilin

                        alotHashahar("alotHashahar", 4, PT_U8, 33, &prefs),
                        // hishuv zman alot hashahar

                        minhaG("minhaG", 3, PT_U8, 34, &prefs),
                        // hishuv zman minha gdola

                        sozksMga("sozksMga", 1, PT_U8, 35, &prefs),
                        // hishuv zman sof zman kriat shma mga

                        soztMga("soztMga", 1, PT_U8, 36, &prefs),
                        // hishuv zman sof zman tfila mga

                        plagMinha("plagMinha", 1, PT_U8, 37, &prefs),
                        // hishuv zman plag haminha

                        zeetShabat("zeetShabat", 16, PT_U8, 38, &prefs),
                        // hishuv zman zeet shabat

                        zeetZom("zeetZom", 14, PT_U8, 39, &prefs),
                        // hishuv zman zeet zom derabanan

                        nerot("nerot", 22, PT_U8, 40, &prefs)
                        // hadlakat nerot mukdemet
                        // 0 - 40
{
}

MinhagimPrefs::MinhagimPrefs() :
                        eda("eda", 1, PT_U8, 5, &prefs),
                        // 1. Edot hamizrah
                        // 2. sfarad
                        // 3. ashkenaz
                        // 4. teiman - baladi
                        // 5. teiman - shami
                        // 6. chabad

                        hul("hul", 0, PT_U8, 42, &prefs),
                        // 0. israel
                        // 1. bnei hul

                        shanim("shanim", 0, PT_U8, 45, &prefs)
                        // time from hurban bayit sheni
                        // 0. 1954
                        // 1. 1955
{
}

DataPrefs dataPrefs;
