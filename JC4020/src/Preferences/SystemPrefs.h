#ifndef SYSTEMPREFS_H
#define SYSTEMPREFS_H

#include "UserPreferences.h"

extern UserPreferences userPreferences;

// ---------------------------------------------------------------------------
// SystemPrefs: העדפות שכבת המערכת
// חלון (הפניות) לשדות הקיימים ב-userPreferences.
// ---------------------------------------------------------------------------
struct SystemPrefs
{
    UserPref<uint8_t> &bt;
};

inline constexpr SystemPrefs systemPrefs{
    userPreferences.bt};

#endif
