#ifndef DISPLAYPREFS_H
#define DISPLAYPREFS_H

#include "UserPreferences.h"

extern UserPreferences userPreferences;

// ---------------------------------------------------------------------------
// DisplayPrefs: העדפות שכבת התצוגה - בהירות, צבעים, גופן
// חלון (הפניות) לשדות הקיימים ב-userPreferences. הסדר לפי אינדקס BLE.
// ---------------------------------------------------------------------------
struct DisplayPrefs
{
    UserPref<uint8_t> &brightness;
    UserPref<uint8_t> &colorUp;
    UserPref<uint8_t> &colorMiddle;
    UserPref<uint8_t> &colorBottomT;
    UserPref<uint8_t> &colorBottomV;
    UserPref<uint8_t> &colorYom;
    UserPref<uint8_t> &colorParasha;
    UserPref<uint8_t> &colorIluy;
    UserPref<uint8_t> &clockFont;
    UserPref<uint8_t> &colorIvri;
    UserPref<uint8_t> &colorLoazi;
    UserPref<uint8_t> &colorParashaV;
};

inline constexpr DisplayPrefs displayPrefs{
    userPreferences.brightness,
    userPreferences.colorUp,
    userPreferences.colorMiddle,
    userPreferences.colorBottomT,
    userPreferences.colorBottomV,
    userPreferences.colorYom,
    userPreferences.colorParasha,
    userPreferences.colorIluy,
    userPreferences.clockFont,
    userPreferences.colorIvri,
    userPreferences.colorLoazi,
    userPreferences.colorParashaV};

#endif
