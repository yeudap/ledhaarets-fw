#include "SystemPrefs.h"

// מופע ה-Preferences של מרחב "clk" - שייך לרישום (userPreferences.prefs)
static Preferences &prefs = userPreferences.prefs;

SystemPrefs::SystemPrefs() :
                        bt("bt", 1, PT_U8, 76, &prefs)
                        // bluetooth on/off
                        // 0. off
                        // 1. on
{
}

SystemPrefs systemPrefs;
