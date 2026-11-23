#ifndef PRESENTATIONPREFS_H
#define PRESENTATIONPREFS_H

#include "UserPreferences.h"

extern UserPreferences userPreferences;

// ---------------------------------------------------------------------------
// PresentationPrefs: העדפות שכבת ההצגה - שפה, פורמט שעה ותאריך, פריסה
// הבעלים של ההעדפות (UserPref) של השכבה; ההגדרות ב-PresentationPrefs.cpp. הסדר לפי אינדקס BLE.
// ---------------------------------------------------------------------------

// פריסת המסך הראשי: בחירת הפריסה וגרסת כל אזור
struct LayoutPrefs
{
    UserPref<uint8_t> middleLine;
    UserPref<uint8_t> upLine;
    UserPref<uint8_t> screen;
    LayoutPrefs();
};

struct PresentationPrefs
{
    UserPref<uint8_t> safa;
    UserPref<uint8_t> dateFormat;
    UserPref<uint8_t> clockFormat;
    UserPref<uint8_t> timeFormat;
    UserPref<uint8_t> leil;
    LayoutPrefs layout;
    PresentationPrefs();
};

extern PresentationPrefs presentationPrefs;

#endif
