#ifndef SHITA_H
#define SHITA_H

#include "Zman.h"
#include "Sun.h"
#include "Preferences/UserPreferences.h"

enum Minhag {EDOT_MIZRAH=1, SFARAD, ASHKENAZ, TEIMAN_BALADI, TEIMAN_SHAMI, CHABAD};

const int numOfShitot = 8;

const int numOfZeetCochavin = 27;
const char zeetMethod[numOfZeetCochavin+1][18] = 
{ " ",
    "wuguce wues 13.5",     //1- דקות קבועות 13.5
    "wuhbnz wues 13.5",     //2- דקות זמניות 13.5
    "trnujk wues 13.5",     //3- דקות לחומרא 13.5
    "(3.65@) wues 13.5",    //4- דקות במעלות 13.5
    "wuguce wues 18",       //5- דקות קבועןת 18
    "trnujk wues 18",       //6- 18 דקות לחומרא
    "(4.61@) wues 18",      //7- דקות במעלות 18
    "(4.725@) wues 18",     //8- דקות במעלות 18
    "(4.9@) wues 18",       //9- דקות במעלות 18
    "wuguce wues 20",       //10- דקות קבועות 20
    "wuguce wues 22.5",     //11- דקות קבועות 22.5
    "(6@) wues 24",         //12- 24 דקות במעלות
    "(6.45@) wues 25",      //13- 25 דקות במעלות
    "wuguce wues 30",       //14- דקות קבועות 30
    "(8@) wues 34",         //15- 34 דקות במעלות
    "(8.5@) wues 36",       //16- 36 דקות במעלות
    "(9.28@) wues 40",      //17- 40 דקות במעלות
    "wuguce wues 42",       //18- דקות קבועות 42
    "wuguce wues 50",       //19- דקות קבועות 50
    "wuguce wues 72",       //20- דקות קבועות 72
    "wuhbnz wues 72",       //21- דקות זמניות 72
    "(16@) wues 72",        //22- 72 דקות במעלות
    "(16.013@) wues 72",    //23- 72 דקות במעלות
    "(16.1@) wues 72",      //24- 72 דקות במעלות
    "wuguce wues 90",       //25- 90 דקות קבועות
    "wuhbnz wues 90",       //26- 90 דקות זמניות
    "(19.75@) wues 90",     //27- 90 דקות במעלות
};

const int numOfAlot = 7;
const char alotMethod[numOfAlot+1][18] = 
{ " ",
    "wuguce wues 72",       //1- דקות קבועות 72
    "wuhbnz wues 72",       //2- דקות זמניות 72
    "(16@) wues 72",        //3- 72 דקות במעלות
    "(16.013@) wues 72",    //4- 72 דקות במעלות
    "(16.1@) wues 72",      //5- 72 דקות במעלות
    "(19.75@) wues 90",     //6- 90 דקות במעלות
    "(25.95@) wues 120",    //7- 120 דקות במעלות
};

const int numOfTalit = 10;
const char talitMethod[numOfTalit+1][15] = 
{ " ",
    "wuguce wues 45",  //1- דקות קבועות 45
    "wuguce wues 50",  //2- דקות קבועות 50
    "wuguce wues 52",  //3- דקות קבועות 52
    "wuguce wues 60",  //4- דקות קבועות 60
    "wuhbnz wues 66",  //5- דקות זמניות 66
    "wukgn 7.65",      //6- מעלות 7.65
    "wukgn 10.2",      //7- מעלות 10.2
    "wukgn 10.5",      //8- מעלות 10.5
    "wukgn 11",        //9- מעלות 11
    "wukgn 11.5",      //10- מעלות 11.5
};

const int numOfSozMga = 4;
const char sozMgaMethod[numOfSozMga+1][13] = 
{ " ",
    "90 rjav wukg",  //1- עלות השחר 90
    "72 rjav wukg",  //2- עלות השחר 72
    "ohxhb vagn",    //3- מעשה ניסים
    "iuhmk rut",     //4- אור לציון
};

const int numOfMinhaG = 3;
const char minhaGMethod[numOfMinhaG+1][15] = 
{ " ",
    "wuguce wues 30",  //1- דקות קבועות 30
    "wuhbnz wues 30",  //2- דקות זמניות 30
    "trnujk wues 30",  //3- דקות לחומרא 30
};

const int numOfPlag = 3;
const char plagMethod[numOfPlag+1][13] = 
{ " ",
    "t\"rdv",        //1- הגר"א
    "ohxhb vagn",    //2- מעשה ניסים
    "iuhmk rut",     //3- אור לציון
};

const int numOfZS = 3;
const char zsMethod[numOfZS+1][11] = 
{ " ",
    "whruahn",     //1- מישורית
    "whtrbv",      //2- הנראית
    "whnuburyxt",  //3- אסטרונומית
};

const int numOfZmanim = 12;
const char zmanim[numOfZmanim+1][13] = 
{ " ",
    "rjav wukg",      //1. עלות השחר
    "ihkhpwu whky",   //2. טלית ותפילין
    "t\"dn a\"ezux",  //3. סוזק"ש מג"א
    "t\"rd a\"ezux",  //4. סוזק"ש גר"א
    "t\"dn w\"zux",   //5. סוז"ת מג"א
    "t\"rd w\"zux",   //6. סוז"ת גר"א
    "wumj",           //7. חצות
    "vkusd vjbn",     //8. מנחה גדולה
    "vbye vjbn",      //9. מנחה קטנה
    "vjbnv dkp",      //10. פלג המנחה
    "ohcfufv wtm",    //11. צאת הכוכבים
    "ow ubcr",        //12. רבנו תם
};

class Shita
{
public:
    char name[20];
    int id;
    Zman alot90;
    Zman zeet90;
    Zman alot72;
    Zman zeet72;
    Zman alotHashahar;
    Zman zmanTalit;
    Zman sofZmanShmaMga;
    Zman sofZmanShmaGra;
    Zman sofZmanTfilaMga;
    Zman sofZmanTfilaGra;
    Zman hatsot;
    Zman minhaGdola;
    Zman minhaKtana;
    Zman plagHaminha;
    Zman tzetHacochavim;
    Zman shabatIn;
    Zman shabatOut;
    Zman rabenuTam;
    Zman tzomIn;
    Zman tzomOut;
    Zman tBeavIn;
    Zman tBeavOut;
    Zman sofZmanBeiurHamez;

    virtual void calc(Sun*, Zman, Zman, Zman, Zman, Zman, Zman){Serial.println("Shita::calc");}
    virtual void setDefault(){}
    Zman calcTimeZmaniot(Zman base, Zman startDay, Zman EndDay, double dakot);
    Zman zeetCochavimMethods(uint8_t, Sun*, Zman, Zman);
    Zman calcZeetCochavim(Sun*, Zman, Zman);
    Zman calcRabenuTam(Sun*, Zman, Zman);
    Zman calcZeetShabat(Sun*, Zman, Zman);
    Zman calcZeetZom(Sun*, Zman, Zman);
    Zman zmanTalitMethods(uint8_t, Sun*, Zman, Zman, Zman);
    Zman calcTalit(Sun*, Zman, Zman, Zman);
    Zman alotMethods(uint8_t, Sun*, Zman, Zman);
    Zman calcAlot(Sun*, Zman, Zman);
    Zman minhaGMethods(uint8_t, Sun*, Zman, Zman, Zman);
    Zman calcMinhaG(Sun*, Zman, Zman, Zman);
    Zman sozksMgaMethods(uint8_t, Sun*, Zman, Zman, Zman, Zman, Zman, Zman, Zman, Zman);
    Zman calcsozksMga(Sun*, Zman, Zman, Zman, Zman, Zman, Zman, Zman, Zman);
    Zman soztMgaMethods(uint8_t, Sun*, Zman, Zman, Zman, Zman, Zman, Zman, Zman, Zman);
    Zman calcsoztMga(Sun*, Zman, Zman, Zman, Zman, Zman, Zman, Zman, Zman);
    Zman plagMethods(uint8_t, Sun*, Zman, Zman, Zman);
    Zman calcPlag(Sun*, Zman, Zman, Zman);
    Shita(){}
    Shita(const char* s, int i)
    {
        strcpy(name, s);
        id = i;
    }
};
class orHachaim : public Shita
{
public:
    void calc(Sun*, Zman, Zman, Zman, Zman, Zman, Zman);
    void setDefault();
    orHachaim(const char* s, int i) : Shita(s,i) {}
};
class kiseRachamim : public Shita
{
public:
    void calc(Sun*, Zman, Zman, Zman, Zman, Zman, Zman);
    void setDefault();
    kiseRachamim(const char* s, int i) : Shita(s,i) {}
};
class chazonShamaim : public Shita
{
public:
    void calc(Sun*, Zman, Zman, Zman, Zman, Zman, Zman);
    void setDefault();
    chazonShamaim(const char* s, int i) : Shita(s,i) {}
};
class yeshiva : public Shita
{
public:
    void calc(Sun*, Zman, Zman, Zman, Zman, Zman, Zman);
    void setDefault();
    yeshiva(const char* s, int i) : Shita(s,i) {}
};
class habad : public Shita
{
public:
    void calc(Sun*, Zman, Zman, Zman, Zman, Zman, Zman);
    void setDefault();
    habad(const char* s, int i) : Shita(s,i) {}
};
class habad1 : public Shita
{
public:
    void calc(Sun*, Zman, Zman, Zman, Zman, Zman, Zman);
    void setDefault();
    habad1(const char* s, int i) : Shita(s,i) {}
};
class tucazinsky : public Shita
{
public:
    void calc(Sun*, Zman, Zman, Zman, Zman, Zman, Zman);
    void setDefault();
    tucazinsky(const char* s, int i) : Shita(s,i) {}
};
class benIshHai : public Shita
{
public:
    void calc(Sun*, Zman, Zman, Zman, Zman, Zman, Zman);
    void setDefault();
    benIshHai(const char* s, int i) : Shita(s,i) {}
};

#endif
