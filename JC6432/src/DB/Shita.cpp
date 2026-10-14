#include "Shita.h"
extern UserPreferences userPreferences;
extern Place* place;


orHachaim _orHachaim = orHachaim("ohhjv rut", 0);
kiseRachamim _kiseRachamim = kiseRachamim("ohnjr txf", 1);
chazonShamaim _chazonShamaim = chazonShamaim("ohna iuzj", 2);
yeshiva _yeshiva = yeshiva("vchah = snkn crv", 3);
habad _habad = habad("s\"cj kkuf - z\"ar", 4);
habad1 _habad1 = habad1("whwhntv vghea - s\"cj", 5);
tucazinsky _tucazinsky = tucazinsky("hexbhmeuy crv", 6);
benIshHai _benIshHai = benIshHai("b\"gn = hj aht icv", 7);
Shita* shitot[numOfShitot] = {
    &_orHachaim,
    &_kiseRachamim,
    &_chazonShamaim,
    &_yeshiva,
    &_habad,
    &_habad1,
    &_tucazinsky,
    &_benIshHai,
};

Zman Shita::calcTimeZmaniot(Zman base, Zman startDay, Zman EndDay, double dakot)
{
    Zman dayLen = EndDay - startDay;
    return base + dayLen / (720.0 / dakot);
}

void orHachaim::calc(Sun* s, Zman zrihaA, Zman shkiaaA, Zman zrihaM, Zman shkiaaM, Zman zrihaN, Zman shkiaaN)
{
    alot90 = s->calcAlot90();
    zeet90 = s->calcZeet90();
    Zman zrihaMukdemet = min(zrihaA, zrihaN);
    Zman shkiaaMeuheret = max(shkiaaA, shkiaaN);
    alot72 = calcTimeZmaniot(zrihaA, zrihaA, shkiaaA, -72);
    zeet72 = calcTimeZmaniot(shkiaaA, zrihaA, shkiaaA, 72);
    Zman zeet = calcTimeZmaniot(shkiaaMeuheret, zrihaMukdemet, shkiaaMeuheret, 13.5);

    alotHashahar = calcAlot(s, zrihaMukdemet, shkiaaMeuheret).floor();
    rabenuTam = calcRabenuTam(s, zrihaA, shkiaaA).ceil();
    zmanTalit = calcTalit(s, alot72, zrihaA, shkiaaA).ceil();
    tzetHacochavim = calcZeetCochavim(s, zrihaMukdemet, shkiaaMeuheret).ceil();
    sofZmanShmaMga = calcsozksMga(s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanShmaGra = calcTimeZmaniot(zrihaA, zrihaA, shkiaaA, 180).floor();
    sofZmanTfilaMga = calcsoztMga(s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanTfilaGra = calcTimeZmaniot(zrihaA, zrihaA, shkiaaA, 240).floor();
    hatsot = calcTimeZmaniot(zrihaA, zrihaA, shkiaaA, 360).round();
    minhaGdola = calcMinhaG(s, hatsot, zrihaA, shkiaaA).round();
    minhaKtana = calcTimeZmaniot(zrihaA, zrihaA, shkiaaA, 9.5*60).ceil();
    plagHaminha = calcPlag(s, zrihaM, shkiaaM, zeet).ceil();
    shabatIn = shkiaaA.ceil() - userPreferences.nerot;
    shabatOut = calcZeetShabat(s, zrihaA, shkiaaA).ceil() +1;
    tzomIn = alotHashahar;
    tzomOut = calcZeetZom(s, zrihaA, shkiaaA).ceil();
    tBeavIn = shkiaaA.floor();
    tBeavOut = tzomOut;
    sofZmanBeiurHamez = calcTimeZmaniot(alot72, alot72, rabenuTam, 300).floor();
}
void orHachaim::setDefault()
{
    userPreferences.zriha= 2;        // הנראית
    userPreferences.shkiaa= 3;       // אסטרונומית
    userPreferences.zeetCochavim= 2; // 13.5 זמניות
    userPreferences.rabenuTam= 21;   // 72 זמניות
    userPreferences.zmanTalit= 5;    // 66 זמניות
    userPreferences.alotHashahar= 2; // 72 זמניות
    userPreferences.sozksMga= 2;     // עלות השחר 72
    userPreferences.soztMga= 2;      // עלות השחר 72
    userPreferences.minhaG= 3;       // 30 לחומרא
    userPreferences.plagMinha= 1;    // הגר"א
    userPreferences.zeetShabat= 14;  // 30 קבועות
    userPreferences.zeetZom= 2;      // 13.5 זמניות
    if(place->orHachaim) { userPreferences.nerot= 0; }
}

void kiseRachamim::calc(Sun* s, Zman zrihaA, Zman shkiaaA, Zman zrihaM, Zman shkiaaM, Zman zrihaN, Zman shkiaaN)
{
    alot90 = s->calcAlot90();
    zeet90 = s->calcZeet90();
    alot72 = s->getTimeByElev(-(90 + 16.013));
    zeet72 = s->getTimeByElev(90 + 16.1);

    alotHashahar = calcAlot(s, zrihaM, shkiaaM).round();
    rabenuTam = calcRabenuTam(s, zrihaA, shkiaaA).round();
    zmanTalit = calcTalit(s, alot72, zrihaN + 2.5, shkiaaM).ceil();
    tzetHacochavim = calcZeetCochavim(s, zrihaA, shkiaaA).ceil();
    sofZmanShmaMga = calcsozksMga(s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanShmaGra = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 180).ceil();
    sofZmanTfilaMga = calcsoztMga(s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanTfilaGra = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 240).round();
    hatsot = s->getTimeByElev(0).round();
    minhaGdola = calcMinhaG(s, hatsot, zrihaM, shkiaaM).round();
    minhaKtana = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 9.5*60).ceil();
    plagHaminha = calcPlag(s, zrihaM, shkiaaM, shkiaaM).round();
    shabatIn = shkiaaM.ceil() - userPreferences.nerot;
    shabatOut = calcZeetShabat(s, zrihaM, shkiaaM).ceil();
    tzomIn = alot72.floor();
    tzomOut = calcZeetZom(s, zrihaA, shkiaaA).ceil();
    tBeavIn = shkiaaA.floor();
    tBeavOut = tzomOut;
    sofZmanBeiurHamez = calcTimeZmaniot(alot72, alot72, zeet72, 300).floor();
}
void kiseRachamim::setDefault()
{
    userPreferences.zriha= 2;        // הנראית
    userPreferences.shkiaa= 3;       // אסטרונומית
    userPreferences.zeetCochavim= 7; // 4.61 מעלות
    userPreferences.rabenuTam= 20;   // 72 קבועות
    userPreferences.zmanTalit= 4;    // 60 קבועות
    userPreferences.alotHashahar= 4; // 16.013 מעלות
    userPreferences.sozksMga= 2;     // עלות השחר 72
    userPreferences.soztMga= 2;      // עלות השחר 72
    userPreferences.minhaG= 1;       // 30 קבועות
    userPreferences.plagMinha= 1;    // הגר"א
    userPreferences.zeetShabat= 16;  // 8.5 מעלות
    userPreferences.zeetZom= 10;     // 20 קבועות
    if(place->kiseRahamim) { userPreferences.nerot= 0; }
}

void chazonShamaim::calc(Sun* s, Zman zrihaA, Zman shkiaaA, Zman zrihaM, Zman shkiaaM, Zman zrihaN, Zman shkiaaN)
{
    alot90 = s->calcAlot90();
    zeet90 = s->calcZeet90();
    alot72 = s->calcAlot72();
    zeet72 = s->calcZeet72();

    alotHashahar = calcAlot(s, zrihaM, shkiaaM).floor();
    zmanTalit = calcTalit(s, alot72, zrihaM, shkiaaM).ceil();
    sofZmanShmaMga = calcsozksMga(s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanShmaGra = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 180).floor();
    sofZmanTfilaMga = calcsoztMga(s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanTfilaGra = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 240).floor();
    hatsot = s->getTimeByElev(0).floor();
    minhaGdola = calcMinhaG(s, hatsot, zrihaM, shkiaaM).ceil();
    minhaKtana = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 9.5*60).ceil();
    plagHaminha = calcPlag(s, zrihaM, shkiaaM, shkiaaM).ceil();
    tzetHacochavim = calcZeetCochavim(s, zrihaM, shkiaaM).ceil();
    shabatIn = shkiaaA.round() - userPreferences.nerot;
    shabatOut = calcZeetShabat(s, zrihaM, shkiaaM).ceil();
    rabenuTam = calcRabenuTam(s, zrihaA, shkiaaA).round();
    tzomIn = alotHashahar;
    tzomOut = calcZeetZom(s, zrihaM, shkiaaM).ceil();
    tBeavIn = shkiaaA.floor();
    tBeavOut = tzomOut;
    sofZmanBeiurHamez = calcTimeZmaniot(alot72, alot72, zeet72, 300).floor();
}
void chazonShamaim::setDefault()
{
    userPreferences.zriha= 1;        // מישורית
    userPreferences.shkiaa= 3;       // אסטרונומית
    userPreferences.zeetCochavim= 8; // 4.725 במעלות
    userPreferences.rabenuTam= 20;   // 72 קבועות
    userPreferences.zmanTalit= 10;   // 11.5 מעלות
    userPreferences.alotHashahar= 4; // 16.013 במעלות
    userPreferences.sozksMga= 1;     // עלות השחר 90
    userPreferences.soztMga= 1;      // עלות השחר 90
    userPreferences.minhaG= 3;       // 30 לחומרא
    userPreferences.plagMinha= 1;    // הגר"א
    userPreferences.zeetShabat= 16;  // 8.5 במעלות
    userPreferences.zeetZom= 13;     // 6.45 במעלות
}

void yeshiva::calc(Sun* s, Zman zrihaA, Zman shkiaaA, Zman zrihaM, Zman shkiaaM, Zman zrihaN, Zman shkiaaN)
{
    alot90 = s->calcAlot90();
    zeet90 = s->calcZeet90();
    alot72 = zrihaM - 72;
    zeet72 = shkiaaM + 72;

    alotHashahar = calcAlot(s, zrihaM, shkiaaM).round();
    zmanTalit = calcTalit(s, alot72, zrihaM, shkiaaM).floor();
    sofZmanShmaMga = calcsozksMga(s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanShmaGra = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 180).floor();
    sofZmanTfilaMga = calcsoztMga(s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanTfilaGra = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 240).floor();
    hatsot = s->getTimeByElev(0).floor();
    minhaGdola = calcMinhaG(s, hatsot, zrihaM, shkiaaM).floor();
    minhaKtana = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 9.5 * 60).round();
    plagHaminha = calcPlag(s, zrihaM, shkiaaM, shkiaaM).floor();
    tzetHacochavim = calcZeetCochavim(s, zrihaM, shkiaaM).ceil();
    rabenuTam = calcRabenuTam(s, zrihaM, shkiaaM).round();
    shabatIn = shkiaaM.ceil() - userPreferences.nerot;
    shabatOut = calcZeetShabat(s, zrihaM, shkiaaM).floor();
    tzomIn = alot72.floor();
    tzomOut = tzetHacochavim;
    tBeavIn = shkiaaA.floor();
    tBeavOut = s->getTimeByElev(90 + 6.45).ceil();
    sofZmanBeiurHamez = calcTimeZmaniot(alot72, alot72, zeet72, 300).ceil();
}
void yeshiva::setDefault()
{
    userPreferences.zriha= 2;        // הנראית
    userPreferences.shkiaa= 1;       // מישורית
    userPreferences.zeetCochavim= 6; // 18 לחומרא
    userPreferences.rabenuTam= 20;   // 72 קבועות
    userPreferences.zmanTalit= 1;    // 45 קבועות
    userPreferences.alotHashahar= 1; // 72 קבועות
    userPreferences.sozksMga= 2;     // עלות השחר 72
    userPreferences.soztMga= 2;      // עלות השחר 72
    userPreferences.minhaG= 3;       // 30 לחומרא
    userPreferences.plagMinha= 1;    // הגר"א
    userPreferences.zeetShabat= 16;  // 8.5 במעלות
    userPreferences.zeetZom= 6;      // 18 לחומרא
}

void habad::calc(Sun* s, Zman zrihaA, Zman shkiaaA, Zman zrihaM, Zman shkiaaM, Zman zrihaN, Zman shkiaaN)
{
    alot90 = s->calcAlot90();
    zeet90 = s->calcZeet90();
    alot72 = calcTimeZmaniot(zrihaN, zrihaM, shkiaaM, -72);
    zeet72 = calcTimeZmaniot(shkiaaN, zrihaM, shkiaaM, 72);

    alotHashahar = calcAlot(s, zrihaM, shkiaaM).round();
    zmanTalit = calcTalit(s, alot72, zrihaM, shkiaaM).floor();
    sofZmanShmaMga = calcsozksMga(s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanShmaGra = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 180).floor();
    sofZmanTfilaMga = calcsoztMga(s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanTfilaGra = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 240).floor();
    hatsot = s->getTimeByElev(0).floor();
    minhaGdola = calcMinhaG(s, hatsot, zrihaM, shkiaaM).floor();
    minhaKtana = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 9.5 * 60).round();
    plagHaminha = calcPlag(s, zrihaM, shkiaaM, shkiaaN).ceil();
    tzetHacochavim = calcZeetCochavim(s, zrihaM, shkiaaM).ceil();
    rabenuTam = calcRabenuTam(s, zrihaM, shkiaaM).round();
    shabatIn = shkiaaA.ceil() - userPreferences.nerot;
    shabatOut = calcZeetShabat(s, zrihaM, shkiaaM).floor();
    tzomIn = alot72.floor();
    tzomOut = tzetHacochavim;
    tBeavIn = shkiaaA.floor();
    tBeavOut = s->getTimeByElev(90 + 6.45).ceil();
    sofZmanBeiurHamez = calcTimeZmaniot(alot72, alot72, zeet72, 300).ceil();
}
void habad::setDefault()
{
    userPreferences.zriha= 3;        // אסטרונומית
    userPreferences.shkiaa= 2;       // הנראית
    userPreferences.zeetCochavim= 12;// 6 במעלות
    userPreferences.rabenuTam= 21;   // 72 זמניות
    userPreferences.zmanTalit= 10;   // 11.5 במעלות
    userPreferences.alotHashahar= 7; // 25.95 במעלות
    userPreferences.sozksMga= 2;     // עלות השחר 72
    userPreferences.soztMga= 2;      // עלות השחר 72
    userPreferences.minhaG= 2;       // 30 זמניות
    userPreferences.plagMinha= 1;    // הגר"א
    userPreferences.zeetShabat= 16;  // 8.5 במעלות
    userPreferences.zeetZom= 13;     // 6.45 במעלות
}

void habad1::calc(Sun* s, Zman zrihaA, Zman shkiaaA, Zman zrihaM, Zman shkiaaM, Zman zrihaN, Zman shkiaaN)
{
    Zman _90 = s->getTimeByElev(90);
    Zman _90_4_25 = s->getTimeByElev(90 + 4.25);
    Zman _4_25_deg = _90_4_25 - _90;

    alot90 = calcAlot(s, zrihaA, shkiaaA)-4;
    zeet90 = shkiaaA+_4_25_deg;
    alot72 = zeet90;
    zeet72 = calcRabenuTam(s, zrihaA, shkiaaA).round();

    alotHashahar = alot90.round();
    zmanTalit = calcTalit(s, alotHashahar, zrihaA, shkiaaA).floor();
    sofZmanShmaMga = calcsozksMga(s, alot90, zeet90, alot90, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanShmaGra = calcTimeZmaniot(zrihaA, zrihaA, shkiaaA, 180).floor();
    sofZmanTfilaMga = calcsoztMga(s, alot90, zeet90, alot90, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanTfilaGra = calcTimeZmaniot(zrihaA, zrihaA, shkiaaA, 240).floor();
    hatsot = s->getTimeByElev(0).floor();
    minhaGdola = calcMinhaG(s, hatsot, zrihaA, shkiaaA).floor();
    minhaKtana = calcTimeZmaniot(zrihaA, zrihaA, shkiaaA, 9.5 * 60).round();
    plagHaminha = calcPlag(s, zrihaA, shkiaaA, shkiaaA).ceil();
    tzetHacochavim = zeet90.ceil();
    rabenuTam = calcRabenuTam(s, zrihaA, shkiaaA).round();
    shabatIn = shkiaaA.ceil() - userPreferences.nerot;
    shabatOut = tzetHacochavim.ceil()+4;
    tzomIn = alot90.floor();
    tzomOut = tzetHacochavim.ceil();
    tBeavIn = shkiaaA.floor();
    tBeavOut = s->getTimeByElev(90 + 6.45).ceil();
    sofZmanBeiurHamez = calcTimeZmaniot(alot90, alot90, rabenuTam, 300).ceil();
}
void habad1::setDefault()
{
    userPreferences.zriha= 3;        // אסטרונומית
    userPreferences.shkiaa= 3;       // אסטרונומית
    userPreferences.zeetCochavim= 12;// 6 במעלות
    userPreferences.rabenuTam= 21;   // 72 זמניות
    userPreferences.zmanTalit= 10;   // 11.5 במעלות
    userPreferences.alotHashahar= 7; // 25.95 במעלות
    userPreferences.sozksMga= 2;     // עלות השחר 72
    userPreferences.soztMga= 2;      // עלות השחר 72
    userPreferences.minhaG= 2;       // 30 זמניות
    userPreferences.plagMinha= 1;    // הגר"א
    userPreferences.zeetShabat= 16;  // 8.5 במעלות
    userPreferences.zeetZom= 13;     // 6.45 במעלות
}

void tucazinsky::calc(Sun* s, Zman zrihaA, Zman shkiaaA, Zman zrihaM, Zman shkiaaM, Zman zrihaN, Zman shkiaaN)
{
    alot90 = s->calcAlot90();
    zeet90 = s->calcZeet90();
    alot72 = s->calcAlot72();
    zeet72 = s->calcZeet72();

    alotHashahar = calcAlot(s, zrihaM, shkiaaM).floor();
    zmanTalit = calcTalit(s, alot90, zrihaM, shkiaaM).ceil();
    sofZmanShmaMga = calcsozksMga(s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanShmaGra = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 180).floor();
    sofZmanTfilaMga = calcsoztMga(s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanTfilaGra = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 240).floor();
    hatsot = s->getTimeByElev(0).floor();
    minhaGdola = calcMinhaG(s, hatsot, zrihaM, shkiaaM).ceil();
    minhaKtana = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 9.5*60).ceil();
    plagHaminha = calcPlag(s, zrihaM, shkiaaM, shkiaaM).ceil();
    tzetHacochavim = calcZeetCochavim(s, zrihaM, shkiaaM).ceil();
    shabatIn = shkiaaA.round() - userPreferences.nerot;
    shabatOut = calcZeetShabat(s, zrihaM, shkiaaM).ceil();
    rabenuTam = calcRabenuTam(s, zrihaA, shkiaaA).round();
    tzomIn = alotHashahar;
    tzomOut = calcZeetZom(s, zrihaM, shkiaaM).ceil();
    tBeavIn = shkiaaA.floor();
    tBeavOut = tzomOut;
    sofZmanBeiurHamez = calcTimeZmaniot(alot90, alot90, zeet90, 300).floor();
}
void tucazinsky::setDefault()
{
    userPreferences.zriha= 2;        // הנראית
    userPreferences.shkiaa= 3;       // אסטרונומית
    userPreferences.zeetCochavim= 8; // 4.725 במעלות
    userPreferences.rabenuTam= 20;   // 72 קבועות
    userPreferences.zmanTalit= 10;   // 11.5 מעלות
    userPreferences.alotHashahar= 6; // 19.75 במעלות
    userPreferences.sozksMga= 1;     // עלות השחר 90
    userPreferences.soztMga= 1;      // עלות השחר 90
    userPreferences.minhaG= 3;       // 30 לחומרא
    userPreferences.plagMinha= 1;    // הגר"א
    userPreferences.zeetShabat= 16;  // 8.5 במעלות
    userPreferences.zeetZom= 13;     // 6.45 במעלות
}

void benIshHai::calc(Sun* s, Zman zrihaA, Zman shkiaaA, Zman zrihaM, Zman shkiaaM, Zman zrihaN, Zman shkiaaN)
{
    alot90 = s->calcAlot90();
    zeet90 = s->calcZeet90();
    alot72 = s->calcAlot72();
    zeet72 = s->calcZeet72();

    alotHashahar = calcAlot(s, zrihaM, shkiaaM).floor();
    zmanTalit = calcTalit(s, alot90, zrihaM, shkiaaM).ceil();
    sofZmanShmaMga = calcsozksMga(s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanShmaGra = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 180).floor();
    sofZmanTfilaMga = calcsoztMga(s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN).floor();
    sofZmanTfilaGra = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 240).floor();
    hatsot = s->getTimeByElev(0).floor();
    minhaGdola = calcMinhaG(s, hatsot, zrihaM, shkiaaM).ceil();
    minhaKtana = calcTimeZmaniot(zrihaM, zrihaM, shkiaaM, 9.5*60).ceil();
    plagHaminha = calcPlag(s, zrihaM, shkiaaM, shkiaaM).ceil();
    tzetHacochavim = calcZeetCochavim(s, zrihaM, shkiaaM).ceil();
    shabatIn = shkiaaA.round() - userPreferences.nerot;
    shabatOut = calcZeetShabat(s, zrihaM, shkiaaM).ceil();
    rabenuTam = calcRabenuTam(s, zrihaA, shkiaaA).round();
    tzomIn = alotHashahar;
    tzomOut = calcZeetZom(s, zrihaM, shkiaaM).ceil();
    tBeavIn = shkiaaA.floor();
    tBeavOut = tzomOut;
    sofZmanBeiurHamez = calcTimeZmaniot(alot90, alot90, zeet90, 300).floor();
}
void benIshHai::setDefault()
{
    userPreferences.zriha= 2;        // הנראית
    userPreferences.shkiaa= 3;       // אסטרונומית
    userPreferences.zeetCochavim= 13;// 6.45 במעלות
    userPreferences.rabenuTam= 20;   // 72 קבועות
    userPreferences.zmanTalit= 10;   // 11.5 מעלות
    userPreferences.alotHashahar= 6; // 19.75 במעלות
    userPreferences.sozksMga= 3;     // מעשה ניסים
    userPreferences.soztMga= 3;      // מעשה ניסים
    userPreferences.minhaG= 3;       // 30 לחומרא
    userPreferences.plagMinha= 2;    // מעשה ניסים
    userPreferences.zeetShabat= 16;  // 8.5 במעלות
    userPreferences.zeetZom= 14;     // 6.45 במעלות
}

Zman Shita::calcZeetCochavim(Sun* s, Zman zriha, Zman shkiaa)
{
    return zeetCochavimMethods(userPreferences.zeetCochavim, s, zriha, shkiaa);
}
Zman Shita::calcRabenuTam(Sun* s, Zman zriha, Zman shkiaa)
{
    return zeetCochavimMethods(userPreferences.rabenuTam, s, zriha, shkiaa);
}
Zman Shita::calcZeetShabat(Sun* s, Zman zriha, Zman shkiaa)
{
    return zeetCochavimMethods(userPreferences.zeetShabat, s, zriha, shkiaa);
}
Zman Shita::calcZeetZom(Sun* s, Zman zriha, Zman shkiaa)
{
    return zeetCochavimMethods(userPreferences.zeetZom, s, zriha, shkiaa);
}
Zman Shita::zeetCochavimMethods(uint8_t choose, Sun* s, Zman zriha, Zman shkiaa)
{
    Zman zeet;
    switch (choose)
    {
        case 1:
            zeet = shkiaa + 13.5;
            break;
        case 2:
            zeet = calcTimeZmaniot(shkiaa, zriha, shkiaa, 13.5);
            break;
        case 3:
            zeet = max(shkiaa + 13.5, calcTimeZmaniot(shkiaa, zriha, shkiaa, 13.5));
            break;
        case 4:
            zeet = s->getTimeByElev(90 + 3.65);
            break;
        case 5:
            zeet = shkiaa + 18;
            break;
        case 6:
            zeet = max(shkiaa + 18, calcTimeZmaniot(shkiaa, zriha, shkiaa, 18));
            break;
        case 7:
            zeet = s->getTimeByElev(90 + 4.61);
            break;
        case 8:
            zeet = s->getTimeByElev(90 + 4.725);
            break;
        case 9:
            zeet = s->getTimeByElev(90 + 4.9);
            break;
        case 10:
            zeet = shkiaa + 20;
            break;
        case 11:
            zeet = shkiaa + 22.5;
            break;
        case 12:
            zeet = s->getTimeByElev(90 + 6.45);
            break;
        case 13:
            zeet = s->getTimeByElev(90 + 6);
            break;
        case 14:
            zeet = shkiaa + 30;
            break;
        case 15:
            zeet = s->getTimeByElev(90 + 8);
            break;
        case 16:
            zeet = s->getTimeByElev(90 + 8.5);
            break;
        case 17:
            zeet = s->getTimeByElev(90 + 9.28);
            break;
        case 18:
            zeet = shkiaa + 42;
            break;
        case 19:
            zeet = shkiaa + 50;
            break;
        case 20:
            zeet = shkiaa + 72;
            break;
        case 21:
            zeet = calcTimeZmaniot(shkiaa, zriha, shkiaa, 72);
            break;
        case 22:
            zeet = s->calcZeet72();
            break;
        case 23:
            zeet = s->getTimeByElev(90 + 16.013);
            break;
        case 24:
            zeet = s->getTimeByElev(90 + 16.1);
            break;
        case 25:
            zeet = shkiaa + 90;
            break;
        case 26:
            zeet = calcTimeZmaniot(shkiaa, zriha, shkiaa, 90);
            break;
        case 27:
            zeet = s->calcZeet90();
            break;
        default:
            zeet = s->getTimeByElev(90 + 4.725);
            break;
    }
    return zeet;
}
Zman Shita::calcTalit(Sun* s, Zman alot, Zman zriha, Zman shkiaa)
{
    return zmanTalitMethods(userPreferences.zmanTalit, s, alot, zriha, shkiaa);
}
Zman Shita::zmanTalitMethods(uint8_t choose, Sun* s, Zman alot, Zman zriha, Zman shkiaa)
{
    Zman talit;
    switch (choose)
    {
        case 1:
            talit = zriha - 45;
            break;
        case 2:
            talit = zriha - 50;
            break;
        case 3:
            talit = zriha - 52;
            break;
        case 4:
            talit = zriha - 60;
            break;
        case 5:
            talit = calcTimeZmaniot(alot, zriha, shkiaa, 6);
            break;
        case 6:
            talit = s->getTimeByElev(-(90+7.65));
            break;
        case 7:
            talit = s->getTimeByElev(-(90+10.2));
            break;
        case 8:
            talit = s->getTimeByElev(-(90+10.5));
            break;
        case 9:
            talit = s->getTimeByElev(-(90+11));
            break;
        case 10:
            talit = s->getTimeByElev(-(90+11.5));
            break;
        default:
            talit = s->getTimeByElev(-(90+11.5));
            break;
    }
    return talit;
}
Zman Shita::calcAlot(Sun* s, Zman zriha, Zman shkiaa)
{
    return alotMethods(userPreferences.alotHashahar, s, zriha, shkiaa);
}
Zman Shita::alotMethods(uint8_t choose, Sun* s, Zman zriha, Zman shkiaa)
{
    Zman alot;
    switch (choose)
    {
        case 1:
            alot = zriha - 72;
            break;
        case 2:
            alot = calcTimeZmaniot(zriha, zriha, shkiaa, -72);
            break;
        case 3:
            alot = s->calcAlot72();
            break;
        case 4:
            alot = s->getTimeByElev(-(90 + 16.013));
            break;
        case 5:
            alot = s->getTimeByElev(-(90 + 16.1));
            break;
        case 6:
            alot = s->calcAlot90();
            break;
        case 7:
            alot = s->getTimeByElev(-(90 + 25.95));
            break;
        default:
            alot = s->getTimeByElev(-(90 + 16.013));
            break;
    }
    return alot;
}
Zman Shita::calcsozksMga(Sun* s, Zman alot90, Zman zeet90, Zman alot72, Zman zeet72, Zman zrihaA, Zman shkiaaA, Zman zrihaN, Zman shkiaaN)
{
    return sozksMgaMethods(userPreferences.sozksMga, s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN);
}
Zman Shita::sozksMgaMethods(uint8_t choose, Sun* s, Zman alot90, Zman zeet90, Zman alot72, Zman zeet72, Zman zrihaA, Zman shkiaaA, Zman zrihaN, Zman shkiaaN)
{
    Zman sozks;
    Zman zrihaMukdemet = min(zrihaA, zrihaN);
    Zman shkiaaMeuheret = max(shkiaaA, shkiaaN);
    Zman alot72zmaniot = calcTimeZmaniot(zrihaA, zrihaA, shkiaaA, -72);
    Zman zeet13_5zmaniot = calcTimeZmaniot(shkiaaMeuheret, zrihaMukdemet, shkiaaMeuheret, 13.5);
    Zman zeet6_45 = s->getTimeByElev(90 + 6.45);
    switch (choose)
    {
        case 1:
            sozks = calcTimeZmaniot(alot90, alot90, zeet90, 180);
            break;
        case 2:
            sozks = calcTimeZmaniot(alot72, alot72, zeet72, 180);
            break;
        case 3:
            sozks = calcTimeZmaniot(alot90, alot90, zeet6_45, 180);
            break;
        case 4:
            sozks = calcTimeZmaniot(alot72zmaniot, alot72zmaniot, zeet13_5zmaniot, 180);
            break;
        default:
            sozks = calcTimeZmaniot(alot90, alot90, zeet90, 180);
            break;
    }
    return sozks;
}
Zman Shita::calcsoztMga(Sun* s, Zman alot90, Zman zeet90, Zman alot72, Zman zeet72, Zman zrihaA, Zman shkiaaA, Zman zrihaN, Zman shkiaaN)
{
    return soztMgaMethods(userPreferences.soztMga, s, alot90, zeet90, alot72, zeet72, zrihaA, shkiaaA, zrihaN, shkiaaN);
}
Zman Shita::soztMgaMethods(uint8_t choose, Sun* s, Zman alot90, Zman zeet90, Zman alot72, Zman zeet72, Zman zrihaA, Zman shkiaaA, Zman zrihaN, Zman shkiaaN)
{
    Zman sozt;
    Zman zrihaMukdemet = min(zrihaA, zrihaN);
    Zman shkiaaMeuheret = max(shkiaaA, shkiaaN);
    Zman alot72zmaniot = calcTimeZmaniot(zrihaA, zrihaA, shkiaaA, -72);
    Zman zeet13_5zmaniot = calcTimeZmaniot(shkiaaMeuheret, zrihaMukdemet, shkiaaMeuheret, 13.5);
    Zman zeet6_45 = s->getTimeByElev(90 + 6.45);
    switch (choose)
    {
        case 1:
            sozt = calcTimeZmaniot(alot90, alot90, zeet90, 240);
            break;
        case 2:
            sozt = calcTimeZmaniot(alot72, alot72, zeet72, 240);
            break;
        case 3:
            sozt = calcTimeZmaniot(alot90, alot90, zeet6_45, 240);
            break;
        case 4:
            sozt = calcTimeZmaniot(alot72zmaniot, alot72zmaniot, zeet13_5zmaniot, 240);
            break;
        default:
            sozt = calcTimeZmaniot(alot90, alot90, zeet90, 240);
            break;
    }
    return sozt;
}
Zman Shita::calcPlag(Sun* s, Zman startDay, Zman endDay, Zman ref)
{
    return plagMethods(userPreferences.plagMinha, s, startDay, endDay, ref);
}
Zman Shita::plagMethods(uint8_t choose, Sun* s, Zman startDay, Zman endDay, Zman ref)
{
    Zman plag;

    Zman alot72zmaniot = calcTimeZmaniot(startDay, startDay, endDay, -72);
    Zman zeet13_5zmaniot = calcTimeZmaniot(endDay, startDay, endDay, 13.5);
    Zman alot90 = s->calcAlot90();
    Zman zeet6_45 = s->getTimeByElev(90 + 6.45);
    switch (choose)
    {
        case 1:
            plag = calcTimeZmaniot(ref, startDay, endDay, -75);
            break;
        case 2:
            plag = calcTimeZmaniot(zeet6_45, alot90, zeet6_45, -75);
            break;
        case 3:
            plag = calcTimeZmaniot(zeet13_5zmaniot, alot72zmaniot, zeet13_5zmaniot, -75);
            break;
        default:
            plag = calcTimeZmaniot(ref, startDay, endDay, -75);
            break;
    }
    return plag;
}
Zman Shita::calcMinhaG(Sun* s, Zman hazot, Zman zriha, Zman shkiaa)
{
    return minhaGMethods(userPreferences.minhaG, s, hazot, zriha, shkiaa);
}
Zman Shita::minhaGMethods(uint8_t choose, Sun* s, Zman hazot, Zman zriha, Zman shkiaa)
{
    Zman minha;
    switch (choose)
    {
        case 1:
            minha = hazot + 30;
            break;
        case 2:
            minha = calcTimeZmaniot(hazot, zriha, shkiaa, 30);
            break;
        case 3:
            minha = max(hazot+30, calcTimeZmaniot(hazot, zriha, shkiaa, 30));
            break;
        default:
            minha = max(hazot+30, calcTimeZmaniot(hazot, zriha, shkiaa, 30));
            break;
    }
    return minha;
}