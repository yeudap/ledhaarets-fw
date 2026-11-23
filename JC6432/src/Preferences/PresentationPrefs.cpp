#include "PresentationPrefs.h"

// מופע ה-Preferences של מרחב "clk" - שייך לרישום (userPreferences.prefs)
static Preferences &prefs = userPreferences.prefs;

LayoutPrefs::LayoutPrefs() :
                        middleLine("middleLine", 1, PT_U8, 26, &prefs),
                        // 1. Clock only
                        // 2. Clock + Day

                        upLine("upLine", 1, PT_U8, 27, &prefs),
                        // 1. Heb date + Loazi Date
                        // 2. Heb date + Day
                        // 3. Heb date only

                        screen("screen", 2, PT_U8, 75, &prefs)
                        // main screens
                        // 1. digit only  - Big
                        // 2. yom + ivri + loazi + digit  - Small
                        // 3. yom + loazi + digit
                        // 4. yom + ivri + digit
{
}

PresentationPrefs::PresentationPrefs() :
                        safa("safa", 1, PT_U8, 13, &prefs),
                        // 1. anglit
                        // 2. rusit

                        dateFormat("dateFormat", 1, PT_U8, 22, &prefs),
                        // 1. 14/03/2023
                        // 2. 14/3/2023
                        // 3. 14/3/23
                        // 4. 14/3
                        // 5. 03/14/2023
                        // 6. 2023-03-14
                        // 7. 14.03.2023
                        // 8. without loazi date

                        clockFormat("clockFormat", 24, PT_U8, 23, &prefs),
                        // 12
                        // 24

                        timeFormat("timeFormat", 0, PT_U8, 24, &prefs),
                        // bool. leading zero in time

                        leil("leil", 1, PT_U8, 43, &prefs)
                        // bool. after shkiaa
                        // 0. yom today
                        // 1. leil tomorrow
{
}

PresentationPrefs presentationPrefs;
