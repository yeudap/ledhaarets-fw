#ifndef PRESENTATIONPREFS_H
#define PRESENTATIONPREFS_H

#include "UserPreferences.h"

extern UserPreferences userPreferences;

// ---------------------------------------------------------------------------
// PresentationPrefs: העדפות שכבת ההצגה - שפה, פורמט שעה ותאריך, פריסה
// חלון (הפניות) לשדות הקיימים ב-userPreferences. הסדר לפי אינדקס BLE.
// ---------------------------------------------------------------------------

// פריסת המסך הראשי: בחירת הפריסה וגרסת כל אזור
struct LayoutPrefs
{
    UserPref<uint8_t> &middleLine;
    UserPref<uint8_t> &upLine;
    UserPref<uint8_t> &screen;
};

struct PresentationPrefs
{
    UserPref<uint8_t> &safa;
    UserPref<uint8_t> &dateFormat;
    UserPref<uint8_t> &clockFormat;
    UserPref<uint8_t> &timeFormat;
    UserPref<uint8_t> &leil;
    LayoutPrefs layout;
};

inline constexpr PresentationPrefs presentationPrefs{
    userPreferences.safa,
    userPreferences.dateFormat,
    userPreferences.clockFormat,
    userPreferences.timeFormat,
    userPreferences.leil,
    {userPreferences.middleLine,
     userPreferences.upLine,
     userPreferences.screen}};

#endif
