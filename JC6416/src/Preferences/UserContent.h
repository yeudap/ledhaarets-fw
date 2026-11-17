#ifndef USERCONTENT_H
#define USERCONTENT_H

#include "UserPreferences.h"

extern UserPreferences userPreferences;

// ---------------------------------------------------------------------------
// UserContent: תוכן אישי - 50 הטקסטים (textInput1 ... textInput50)
// חלון (הפניה) לווקטור textPrefs הקיים ב-userPreferences.
// ---------------------------------------------------------------------------
struct UserContent
{
    std::vector<UserPrefBase *> &textPrefs;
};

inline constexpr UserContent userContent{
    userPreferences.textPrefs};

#endif
