#include "LogicPrefs.h"

// מופע ה-Preferences של מרחב "clk" - שייך לרישום (userPreferences.prefs)
static Preferences &prefs = userPreferences.prefs;

LogicPrefs::LogicPrefs() :
                        netzClk("netzClk", 0, PT_U8, 44, &prefs),
                        // time left to Netz
                        // 0-60

                        interval("interval", 30, PT_U16, 46, &prefs),
                        // change rate in seconds
                        // 5-600 (multiple by 5)
                        // use in 4020 only

                        intervalIluy("intervalIluy", 30, PT_U16, 47, &prefs),
                        // change rate in seconds
                        // 5-600 (multiple by 5)
                        // use in 6416 only

                        intervalTime("intervalTime", 30, PT_U16, 48, &prefs),
                        // change rate in seconds
                        // 5-600 (multiple by 5)
                        // use in 6416 only

                        intervalInfo("intervalInfo", 30, PT_U16, 49, &prefs),
                        // change rate in seconds
                        // 5-600 (multiple by 5)
                        // use in 6416 only

                        dafHayomi("dafHayomi", 1, PT_U8, 50, &prefs),
                        // bool. show Daf hayomi bavli

                        yerushalmi("yerushalmi", 0, PT_U8, 51, &prefs),
                        // bool. show Daf hayomi yerushalmi

                        yom("yom", 0, PT_U8, 52, &prefs),
                        // bool. show yom bashavua

                        parasha("parasha", 0, PT_U8, 53, &prefs),
                        // bool. show parashat hashavua

                        haftara("haftara", 1, PT_U8, 54, &prefs),
                        // bool. show haftarat hashavua

                        ivri("ivri", 0, PT_U8, 55, &prefs),
                        // bool. show taarich ivri begadol

                        loazi("loazi", 0, PT_U8, 56, &prefs),
                        // bool. show taarich loazi begadol

                        tfila("tfila", 1, PT_U8, 57, &prefs),
                        // bool. show meida lamitpalel

                        moadim("moadim", 1, PT_U8, 58, &prefs),
                        // bool. izcur hagim - moadim - zom - hodesh

                        showIsraeli("showIsraeli", 1, PT_U8, 59, &prefs),
                        // bool. izcur moadim israelim

                        TAZ("TAZ", 1, PT_U8, 60, &prefs),
                        // zman talit - alot hashachar - zriha
                        // 0. off
                        // 1. on time
                        // 2. all day

                        SOZT("SOZT", 1, PT_U8, 61, &prefs),
                        // sof zman tfila
                        // 0. off
                        // 1. on time
                        // 2. all day

                        SOZKS("SOZKS", 1, PT_U8, 62, &prefs),
                        // sof zman kriat shma
                        // 0. off
                        // 1. on time
                        // 2. all day

                        HMM("HMM", 1, PT_U8, 63, &prefs),
                        // chazot - mincha gdola - mincha ktana
                        // 0. off
                        // 1. on time
                        // 2. all day

                        PSZ("PSZ", 1, PT_U8, 64, &prefs),
                        // plag hamincha - shkiaa - zeet hacochavim
                        // 0. off
                        // 1. on time
                        // 2. all day

                        HAZ("HAZ", 1, PT_U8, 65, &prefs),
                        // hazot - alot hashachar - zriha
                        // 0. off
                        // 1. on time
                        // 2. all day

                        ZSZ1("ZSZ1", 1, PT_U8, 66, &prefs),
                        // zriha - shkiaa - zeet hacochavim
                        // shishi shabat
                        // 0. off
                        // 1. on time
                        // 2. all day

                        ZSZ("ZSZ", 1, PT_U8, 67, &prefs),
                        // zriha - shkiaa - zeet hacochavim
                        // yom hol
                        // 0. off
                        // 1. on time
                        // 2. all day

                        ZYS("ZYS", 3, PT_U8, 68, &prefs),
                        // zriha - yom - shkiaa
                        // 0. off
                        // 1. yom hol
                        // 2. shishi shabat
                        // 3. all week

                        ZPS("ZPS", 0, PT_U8, 69, &prefs),
                        // zriha - parasha - shkiaa
                        // 0. off
                        // 1. yom hol
                        // 2. shishi shabat
                        // 3. all week

                        IPO("IPO", 2, PT_U8, 70, &prefs),
                        // knisat shabat - parasha - zeet shabat
                        // 0. off
                        // 1. yom hol
                        // 2. shishi shabat
                        // 3. all week

                        IORT("IORT", 2, PT_U8, 71, &prefs)
                        // knisat shabat - zeet shabat - rabenu tam
                        // 0. off
                        // 1. yom hol
                        // 2. shishi shabat
                        // 3. all week
{
}

LogicPrefs logicPrefs;
