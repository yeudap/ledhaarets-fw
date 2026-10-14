#ifndef Screens_H
#define Screens_H

#include "Area.h"
#include "DB/DayInfo.h"

extern Strings strings;


class Screen
{

public:
    virtual void printScreen();
};

class MainScreen : public Screen
{
public:
    // up
    int colorUp;
    int colorIluy;
    Area* ivriDate;
    Area* iluyNishmat;
    Area* loaziDate;
    // middle
    int colorMiddle;
    int colorYom;
    int colorParasha;
    const GFXfont* clockFont;
    Area* clockAll;
    Area* yom;
    Area* hour;
    Area* nekudotaim1;
    Area* minute;
    Area* nekudotaim2;
    Area* second1;
    Area* second2;
    Area* parashaTitle;
    Area* parashaValue;
    Area* parashaAll;

    // bottom
    int colorBottomT;
    int colorBottomV;

    Area* zmanAll;
    Area* zmanUp;
    Area* zmanDown;
    
    Area* zmanRightUp;
    Area* zmanRightDown;
    Area* zmanMiddleUp;
    Area* zmanMiddleDown;
    Area* zmanLeftUp;
    Area* zmanLeftDown;

    Area* zmanInfoRightUp;
    Area* zmanInfoRightDown;
    Area* zmanInfoMiddleUp;
    Area* zmanInfoMiddleDown;
    Area* zmanInfoLeftUp;
    Area* zmanInfoLeftDown;

    Area* zmanRightUpS;
    Area* zmanRightDownS;
    Area* zmanMiddleUpS;
    Area* zmanMiddleDownS;
    Area* zmanLeftUpS;
    Area* zmanLeftDownS;

    Area* infoAll;
    Area* infoUp;
    Area* infoDown;

    Area* infoUpHalfRight;
    Area* infoUpHalfLeft;
    Area* infoDownHalfRight;
    Area* infoDownHalfLeft;

    Area* infoAllS;
    Area* infoUpS;
    Area* infoDownS;
    Area* infoUpHalfRightS;
    Area* infoUpHalfLeftS;
    Area* infoShortUpLeft;
    Area* infoShortDownLeft;

    MainScreen();
    void printScreen();
    void updateHour(char* _hour);
    void updateMinute(char* _minute);
    void updateSecond1(char* _second1);
    void updateSecond2(char* _second2);
    void updateNekudotaim(char* _nekudotaim);
    void updateIluyNishmat(String* _iluyNishmat); 
    void updateHebDate(const char* _ivriDate1, const char* _ivriDate2); 
    void updateLoaziDate(const char* _loaziDate); 
    void updateYom(const char* _yomTitle, const char* _yom);

    void updateParashaTitle(const char* _parashaTitle); 
    void updateParashaValue(const char* _parashaValue); 

    void update(Area* area, const char* str, Text &txt);
    void update(Area* area, Zman zman, Text &txt);
};

class MenuScreen : public Screen
{
public:
    Area* title;
    Area* option1;
    Area* option2;
    Area* option3;
    Area* kezev;
    Area* nerot;
    Area* timeZone;
    Area* clkFormat;
    Area* bt;
    Area* format;
    Area* zmanFormat;
    Area* netz;
    Area* shanim;
    Area* kaiz;
    MenuScreen();
    void printTitle(const char* _title);
    void printMenu(const char* _title, const char* a, const char* b, const char* c);
    void printScreen();
    void addZman(Zman zman, bool sec = true);
};

class BrightnessScreen : public Screen
{
public:
    Area* title;
    Area* scale;
    BrightnessScreen();
    void setBrightness(int Brightness);
    void printScreen();
};

class ColorScreen : public Screen
{
public:
    Area* title;
    Area* colors;
    ColorScreen();
    void setColor(uint16_t colorIdx);
    void printScreen();
};

class AboutScreen : public Screen
{
public:
    Area* logoArea;
    Area* info;
    Area* phone;
    Area* url;
    Area* swVer;
    AboutScreen();
    void printScreen();    
};
class VersionScreen : public Screen
{
public:
    Area* title;
    Area* modelTitle;
    Area* swTitle;
    Area* hwTitle;
    Area* serialTitle;
    Area* updateTitle;
    Area* modelValue;
    Area* swValue;
    Area* hwValue;
    Area* serialValue;
    Area* updateValue;

    VersionScreen();
    void printScreen();    
    void printValues();    
    void printValue(Area* area, int value, int defaultValue);    
};
class LogoScreen : public Screen
{
public:
    Area* logoArea;
    LogoScreen();
    void printScreen();    
};
class RegsScreen : public Screen
{
public:
    Area* a1;
    Area* a2;
    Area* a3;
    Area* a4;
    Area* a5;
    Area* a6;
    Area* a7;
    Area* b1;
    Area* b2;
    Area* b3;
    Area* b4;
    Area* b5;
    Area* b6;
    Area* b7;
    Area* c1;
    Area* c2;
    Area* c3;
    Area* c4;
    Area* c5;
    Area* c6;
    Area* c7;
    Area* d1;
    Area* d2;
    Area* d3;
    Area* d4;
    Area* d5;
    Area* d6;
    Area* d7;
    Area* e1;
    Area* e2;
    Area* e3;
    Area* e4;
    Area* e5;
    Area* e6;
    Area* e7;
    Area* f1;
    Area* f2;
    Area* f3;
    Area* f4;
    Area* f5;
    Area* f6;
    Area* f7;
    Area* g1;
    Area* g2;
    Area* g3;
    Area* g4;
    Area* g5;
    Area* g6;
    Area* g7;
    Area* h1;
    Area* h2;
    Area* h3;
    Area* h4;
    Area* h5;
    Area* h6;
    Area* h7;
    Area* i1;
    Area* i2;
    Area* i3;
    Area* i4;
    Area* i5;
    Area* i6;
    Area* i7;

    RegsScreen();
    void printScreen();    
    void printValues();    
};

class ResetScreen : public Screen
{
public:
    Area* title;
    Area* question;
    Area* yes;
    Area* no;
    ResetScreen();
    void SetScreen(const char * text);
    void focusArea(bool reset);
    void printScreen();    
};

class SetClockScreen : public Screen
{
public:
    Area* title;
    Area* hour;
    Area* minute;
    Area* second;
    Area* day;
    Area* month;
    Area* year; 
    Area* point;
    SetClockScreen();
    void printScreen();
    void updateHour(char* _hour);
    void updateMinute(char* _minute);
    void updateSecond(char* _second);
    void updateDay(char* _day);
    void updateMonth(char* _month);
    void updateYear(char* _year);
    void focusArea(Area* area);
    void printPoint();
};

class MessageScreen : public Screen
{
public:
    enum MessageType {ERROR=RED, INFO=YELLOW, SUCCESS=GREEN};
    static const uint16_t defaultDelay = 1000;
    Area* icon;
    Area* message1;
    Area* message2;
    Area* anima;
    MessageScreen();
    void printMessages(MessageType, const char*, const char*, uint16_t=defaultDelay);
    void printScreen();
    void animateTime(int, uint16_t);
    void animatePrercent(int, uint8_t);
    void printProgressBar(MessageType, const char*, const char*, uint8_t=1);
};

class TextScreen : public Screen
{
public:
    Area* title;
    Area* index1;
    Area* index2;
    Area* index3;
    Area* text1;
    Area* text2;
    Area* text3;
    Area* delete1;
   
    TextScreen();
    void printScreen();
    void printDelete();
    void printTitle();
};

#endif