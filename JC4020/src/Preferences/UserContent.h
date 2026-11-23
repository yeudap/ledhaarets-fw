#ifndef USERCONTENT_H
#define USERCONTENT_H

#include "UserPreferences.h"

extern UserPreferences userPreferences;

// ---------------------------------------------------------------------------
// UserContent: תוכן אישי - 50 הטקסטים (textInput1 ... textInput50)
// הבעלים של 50 הטקסטים; ההגדרות ב-UserContent.cpp. textPrefs הוא הפניה לווקטור של הרישום.
// ---------------------------------------------------------------------------
struct UserContent
{
    std::vector<UserPrefBase *> &textPrefs;
    UserPref<String> textInput1;
    UserPref<String> textInput2;
    UserPref<String> textInput3;
    UserPref<String> textInput4;
    UserPref<String> textInput5;
    UserPref<String> textInput6;
    UserPref<String> textInput7;
    UserPref<String> textInput8;
    UserPref<String> textInput9;
    UserPref<String> textInput10;
    UserPref<String> textInput11;
    UserPref<String> textInput12;
    UserPref<String> textInput13;
    UserPref<String> textInput14;
    UserPref<String> textInput15;
    UserPref<String> textInput16;
    UserPref<String> textInput17;
    UserPref<String> textInput18;
    UserPref<String> textInput19;
    UserPref<String> textInput20;
    UserPref<String> textInput21;
    UserPref<String> textInput22;
    UserPref<String> textInput23;
    UserPref<String> textInput24;
    UserPref<String> textInput25;
    UserPref<String> textInput26;
    UserPref<String> textInput27;
    UserPref<String> textInput28;
    UserPref<String> textInput29;
    UserPref<String> textInput30;
    UserPref<String> textInput31;
    UserPref<String> textInput32;
    UserPref<String> textInput33;
    UserPref<String> textInput34;
    UserPref<String> textInput35;
    UserPref<String> textInput36;
    UserPref<String> textInput37;
    UserPref<String> textInput38;
    UserPref<String> textInput39;
    UserPref<String> textInput40;
    UserPref<String> textInput41;
    UserPref<String> textInput42;
    UserPref<String> textInput43;
    UserPref<String> textInput44;
    UserPref<String> textInput45;
    UserPref<String> textInput46;
    UserPref<String> textInput47;
    UserPref<String> textInput48;
    UserPref<String> textInput49;
    UserPref<String> textInput50;
    UserContent();
};

extern UserContent userContent;

#endif
