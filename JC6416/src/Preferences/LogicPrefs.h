#ifndef LOGICPREFS_H
#define LOGICPREFS_H

#include "UserPreferences.h"

extern UserPreferences userPreferences;

// ---------------------------------------------------------------------------
// LogicPrefs: העדפות שכבת הלוגיקה - מה מוצג, מתי ולכמה זמן
// חלון (הפניות) לשדות הקיימים ב-userPreferences. הסדר לפי אינדקס BLE.
// ---------------------------------------------------------------------------
struct LogicPrefs
{
    UserPref<uint8_t> &netzClk;
    UserPref<uint16_t> &interval;
    UserPref<uint16_t> &intervalIluy;
    UserPref<uint16_t> &intervalTime;
    UserPref<uint16_t> &intervalInfo;
    UserPref<uint8_t> &dafHayomi;
    UserPref<uint8_t> &yerushalmi;
    UserPref<uint8_t> &yom;
    UserPref<uint8_t> &parasha;
    UserPref<uint8_t> &haftara;
    UserPref<uint8_t> &ivri;
    UserPref<uint8_t> &loazi;
    UserPref<uint8_t> &tfila;
    UserPref<uint8_t> &moadim;
    UserPref<uint8_t> &showIsraeli;
    UserPref<uint8_t> &TAZ;
    UserPref<uint8_t> &SOZT;
    UserPref<uint8_t> &SOZKS;
    UserPref<uint8_t> &HMM;
    UserPref<uint8_t> &PSZ;
    UserPref<uint8_t> &HAZ;
    UserPref<uint8_t> &ZSZ1;
    UserPref<uint8_t> &ZSZ;
    UserPref<uint8_t> &ZYS;
    UserPref<uint8_t> &ZPS;
    UserPref<uint8_t> &IPO;
    UserPref<uint8_t> &IORT;
};

inline constexpr LogicPrefs logicPrefs{
    userPreferences.netzClk,
    userPreferences.interval,
    userPreferences.intervalIluy,
    userPreferences.intervalTime,
    userPreferences.intervalInfo,
    userPreferences.dafHayomi,
    userPreferences.yerushalmi,
    userPreferences.yom,
    userPreferences.parasha,
    userPreferences.haftara,
    userPreferences.ivri,
    userPreferences.loazi,
    userPreferences.tfila,
    userPreferences.moadim,
    userPreferences.showIsraeli,
    userPreferences.TAZ,
    userPreferences.SOZT,
    userPreferences.SOZKS,
    userPreferences.HMM,
    userPreferences.PSZ,
    userPreferences.HAZ,
    userPreferences.ZSZ1,
    userPreferences.ZSZ,
    userPreferences.ZYS,
    userPreferences.ZPS,
    userPreferences.IPO,
    userPreferences.IORT};

#endif
