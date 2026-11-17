#ifndef DATAPREFS_H
#define DATAPREFS_H

#include "UserPreferences.h"

extern UserPreferences userPreferences;

// ---------------------------------------------------------------------------
// DataPrefs: העדפות שכבת הנתונים - מקום, זמנים (שיטה), מנהגים
// חלון (הפניות) לשדות הקיימים ב-userPreferences. הסדר לפי אינדקס BLE.
// ---------------------------------------------------------------------------

// רשומת המקום
struct PlacePrefs
{
    UserPref<String> &textPlace;
    UserPref<uint8_t> &ofek;
    UserPref<double_t> &nz_lat;
    UserPref<double_t> &nz_long;
    UserPref<int16_t> &nz_height;
    UserPref<uint8_t> &nz_hul;
    UserPref<int8_t> &timezone;
    UserPref<uint8_t> &shaonKaiz;
    UserPref<uint8_t> &homa;
};

// שיטת החישוב ו-12 שיטות הזמנים
struct ZmanimPrefs
{
    UserPref<uint8_t> &shita;
    UserPref<uint8_t> &zriha;
    UserPref<uint8_t> &shkiaa;
    UserPref<uint8_t> &rabenuTam;
    UserPref<uint8_t> &zeetCochavim;
    UserPref<uint8_t> &zmanTalit;
    UserPref<uint8_t> &alotHashahar;
    UserPref<uint8_t> &minhaG;
    UserPref<uint8_t> &sozksMga;
    UserPref<uint8_t> &soztMga;
    UserPref<uint8_t> &plagMinha;
    UserPref<uint8_t> &zeetShabat;
    UserPref<uint8_t> &zeetZom;
    UserPref<uint8_t> &nerot;
};

// מנהגים
struct MinhagimPrefs
{
    UserPref<uint8_t> &eda;
    UserPref<uint8_t> &hul;
    UserPref<uint8_t> &shanim;
};

struct DataPrefs
{
    PlacePrefs place;
    ZmanimPrefs zmanim;
    MinhagimPrefs minhagim;
};

inline constexpr DataPrefs dataPrefs{
    {userPreferences.textPlace,
     userPreferences.ofek,
     userPreferences.nz_lat,
     userPreferences.nz_long,
     userPreferences.nz_height,
     userPreferences.nz_hul,
     userPreferences.timezone,
     userPreferences.shaonKaiz,
     userPreferences.homa},
    {userPreferences.shita,
     userPreferences.zriha,
     userPreferences.shkiaa,
     userPreferences.rabenuTam,
     userPreferences.zeetCochavim,
     userPreferences.zmanTalit,
     userPreferences.alotHashahar,
     userPreferences.minhaG,
     userPreferences.sozksMga,
     userPreferences.soztMga,
     userPreferences.plagMinha,
     userPreferences.zeetShabat,
     userPreferences.zeetZom,
     userPreferences.nerot},
    {userPreferences.eda,
     userPreferences.hul,
     userPreferences.shanim}};

#endif
