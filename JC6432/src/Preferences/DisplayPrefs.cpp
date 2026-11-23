#include "DisplayPrefs.h"

// מופע ה-Preferences של מרחב "clk" - שייך לרישום (userPreferences.prefs)
static Preferences &prefs = userPreferences.prefs;

DisplayPrefs::DisplayPrefs() :
                        brightness("brightness", 200, PT_U8, 12, &prefs),
                        // 1-12  multiple by 20. (valid values 0-255)

                        colorUp("colorUp", 1, PT_U8, 14, &prefs),
                        // 1-7

                        colorMiddle("colorMiddle", 6, PT_U8, 15, &prefs),
                        // 1-7

                        colorBottomT("colorBottomT", 1, PT_U8, 16, &prefs),
                        // 1-7

                        colorBottomV("colorBottomV", 6, PT_U8, 17, &prefs),
                        // 1-7

                        colorYom("colorYom", 1, PT_U8, 18, &prefs),
                        // 1-7

                        colorParasha("colorParasha", 1, PT_U8, 19, &prefs),
                        // 1-7

                        colorIluy("colorIluy", 6, PT_U8, 20, &prefs),
                        // 1-7

                        clockFont("clockFont", 0, PT_U8, 21, &prefs),
                        // 0-4

                        colorIvri("colorIvri", 6, PT_U8, 72, &prefs),
                        // 1-7

                        colorLoazi("colorLoazi", 1, PT_U8, 73, &prefs),
                        // 1-7

                        colorParashaV("colorParashaV", 6, PT_U8, 74, &prefs)
                        // 1-7
{
}

DisplayPrefs displayPrefs;
