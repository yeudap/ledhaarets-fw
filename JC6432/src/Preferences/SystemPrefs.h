#ifndef SYSTEMPREFS_H
#define SYSTEMPREFS_H

#include "UserPreferences.h"

extern UserPreferences userPreferences;

// ---------------------------------------------------------------------------
// SystemPrefs: העדפות שכבת המערכת
// הבעלים של ההעדפות (UserPref) של השכבה; ההגדרות ב-SystemPrefs.cpp.
// ---------------------------------------------------------------------------
struct SystemPrefs
{
    UserPref<uint8_t> bt;
    SystemPrefs();
};

extern SystemPrefs systemPrefs;

#endif
