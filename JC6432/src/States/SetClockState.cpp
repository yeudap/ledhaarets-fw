#include "States.h"
#include "DB/DB.h"
#include "Logic.h"

extern Logic bl;
extern DayInfo dayInfo;
extern int currentHour;
extern int currentMinute;
extern int currentSecond;
extern int currentDay;
const uint8_t numOfFormats = 8;
const uint8_t numOfKaiz = 3;
bool globalchanged = false;

void setClkFormatStr(char* buffer, uint8_t clkFormat)
{
    sprintf(buffer, "%s %d %s%s", "wuga", clkFormat, ":", strings.mivneShaa);
}
void setFormatStr(char* buffer, uint8_t format)
{
    char loaziDateBuffer[20];
    if (format == 8) 
    { 
        sprintf(buffer, "%s       %s%s", "tkk", ":", strings.mivneTaarich);
        return;
    }
    dayInfo.loaziDate.toString(loaziDateBuffer, format);
    sprintf(buffer, "%s  %s%s", loaziDateBuffer, ":", strings.mivneTaarich);
}
void setTimeFormatStr(char* buffer)
{
    char zmanStr[6];
    Zman zmanTemp = Zman(7,8);
    zmanTemp.toString(zmanStr);
    sprintf(buffer, "%s  %s%s", zmanStr, ":", strings.mivneZman);
}
void leilAhreiShkiaaStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.leilAhreiShkiaa, ">");
    if(userPreferences.leil==1) { sprintf(buffer, "%s %s", strings.leilAhreiShkiaa, "<"); }
}
void shaonKaizStr(char* buffer, uint8_t kaiz)
{
    char kaizStr[20];
    if(kaiz==1) 
    { 
        if(dayInfo.isShaonKaiz) { sprintf(kaizStr, "%s - %s", strings.pail, strings.otomati); }
        else { sprintf(kaizStr, "%s - %s", strings.kavuy, strings.otomati); }
    }
    if(kaiz==2) { sprintf(kaizStr, "%s", strings.pail); }
    if(kaiz==3) { sprintf(kaizStr, "%s", strings.kavuy); }
    sprintf(buffer, "%s %s%s", kaizStr, ":", strings.shaonKaiz);
}

void MinhagLeilState::showStateScreen() 
{ 
    char setTimeFormat[50];
    setTimeFormatStr(setTimeFormat);
    char leilAhreiShkiaa[50];
    leilAhreiShkiaaStr(leilAhreiShkiaa);
    char shaonKaiz[50];
    shaonKaizStr(shaonKaiz, userPreferences.shaonKaiz);
    menuScreen.printMenu(strings.menuMinag, setTimeFormat, leilAhreiShkiaa, shaonKaiz); 
}
void MinhagLeilState::okAction()
{
    if(userPreferences.leil==0) { userPreferences.leil= 1; }
    else { userPreferences.leil= 0; } 
}
void ShaonKaiz::showStateScreen() 
{ 
    char leilAhreiShkiaa[50];
    leilAhreiShkiaaStr(leilAhreiShkiaa);
    char shaonKaiz[50];
    shaonKaizStr(shaonKaiz, userPreferences.shaonKaiz);
    menuScreen.printMenu(strings.menuClock, leilAhreiShkiaa, shaonKaiz, strings.kivunShaon); 
}
void ClockSetTime::showStateScreen() 
{ 
    char clkFormatStr[50];
    setClkFormatStr(clkFormatStr, userPreferences.clockFormat);
    char shaonKaiz[50];
    shaonKaizStr(shaonKaiz, userPreferences.shaonKaiz);
    menuScreen.printMenu(strings.menuClock, shaonKaiz, strings.kivunShaon, clkFormatStr); 
}

void ClkFormat::showStateScreen() 
{ 
    char setFormat[50];
    setFormatStr(setFormat, userPreferences.dateFormat);
    char clkFormatStr[50];
    setClkFormatStr(clkFormatStr, userPreferences.clockFormat);
    menuScreen.printMenu(strings.menuClock, strings.kivunShaon, clkFormatStr, setFormat); 
}
void DateFormat::showStateScreen() 
{ 
    char setFormat[50];
    setFormatStr(setFormat, userPreferences.dateFormat);
    char clkFormatStr[50];
    setClkFormatStr(clkFormatStr, userPreferences.clockFormat);
    char setTimeFormat[50];
    setTimeFormatStr(setTimeFormat);
    menuScreen.printMenu(strings.menuClock, clkFormatStr, setFormat, setTimeFormat); 
}
void TimeFormat::showStateScreen() 
{ 
    char setFormat[50];
    setFormatStr(setFormat, userPreferences.dateFormat);
    char setTimeFormat[50];
    setTimeFormatStr(setTimeFormat);
    char leilAhreiShkiaa[50];
    leilAhreiShkiaaStr(leilAhreiShkiaa);
    menuScreen.printMenu(strings.menuClock, setFormat, setTimeFormat, leilAhreiShkiaa); 
}

// Set clock format
void SetClkFormat::showStateScreen() 
{ 
    char setFormat[50];
    setFormatStr(setFormat, userPreferences.dateFormat);
    char clkFormatStr[50];
    setClkFormatStr(clkFormatStr, this->formatTemp);
    menuScreen.printMenu(strings.menuClock, strings.kivunShaon, clkFormatStr, setFormat); 
    menuScreen.clkFormat->addRoundRectBorder();
    menuScreen.clkFormat->addArrows();
}
void SetClkFormat::menuAction() 
{ 
    this->formatTemp = userPreferences.clockFormat;
}
void SetClkFormat::upAction() 
{
    if(this->formatTemp == 12) this->formatTemp = 24;
    else { this->formatTemp = 12; }
}
void SetClkFormat::downAction() 
{
    if(this->formatTemp == 12) this->formatTemp = 24;
    else { this->formatTemp = 12; }
}
void SetClkFormat::okAction() 
{
    userPreferences.clockFormat= this->formatTemp;
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbuwb  ifsgn");
}

// Set shaon kaiz
void SetShaonKaiz::showStateScreen() 
{ 
    char leilAhreiShkiaa[50];
    leilAhreiShkiaaStr(leilAhreiShkiaa);
    char shaonKaiz[50];
    shaonKaizStr(shaonKaiz, this->kaizTemp);
    menuScreen.printMenu(strings.menuClock, leilAhreiShkiaa, shaonKaiz, strings.kivunShaon); 
    menuScreen.kaiz->addRoundRectBorder();
    menuScreen.kaiz->addArrows();
}
void SetShaonKaiz::menuAction() 
{ 
    this->kaizTemp = userPreferences.shaonKaiz;
}
void SetShaonKaiz::upAction() 
{
    this->kaizTemp += numOfKaiz-2;
    this->kaizTemp %= numOfKaiz;
    this->kaizTemp++;
}
void SetShaonKaiz::downAction() 
{
    this->kaizTemp %= numOfKaiz;
    this->kaizTemp++;
}
void SetShaonKaiz::okAction() 
{
    userPreferences.shaonKaiz= this->kaizTemp;
    if(this->kaizTemp==2) { setShaonKaitz(); }
    if(this->kaizTemp==3) { setShaonHoref(); }
    bl.mainState.updateDayValues = true;
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbuwb  ifsgn");
}

// Set date format
void SetDateFormat::showStateScreen() 
{ 
    char setFormat[50];
    setFormatStr(setFormat, this->formatTemp);
    char setTimeFormat[50];
    setTimeFormatStr(setTimeFormat);
    menuScreen.printMenu(strings.menuClock, strings.kivunShaon, setFormat, setTimeFormat); 
    menuScreen.format->addRoundRectBorder();
    menuScreen.format->addArrows();
}
void SetDateFormat::menuAction() 
{ 
    this->formatTemp = userPreferences.dateFormat;
}
void SetDateFormat::upAction() 
{
    this->formatTemp += numOfFormats-2;
    this->formatTemp %= numOfFormats;
    this->formatTemp++;
}
void SetDateFormat::downAction() 
{
    this->formatTemp %= numOfFormats;
    this->formatTemp++;
}
void SetDateFormat::okAction() 
{
    userPreferences.dateFormat= this->formatTemp;
    mainScreen.setScreen(userPreferences.screen);
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbuwb  ifsgn");
}

// Set time format
void SetTimeFormat::showStateScreen() 
{ 
    char setFormat[50];
    setFormatStr(setFormat, userPreferences.dateFormat);
    char setTimeFormat[50];
    setTimeFormatStr(setTimeFormat);
    char leilAhreiShkiaa[50];
    leilAhreiShkiaaStr(leilAhreiShkiaa);
    menuScreen.printMenu(strings.menuClock, setFormat, setTimeFormat, leilAhreiShkiaa); 
    menuScreen.zmanFormat->addRoundRectBorder();
    menuScreen.zmanFormat->addArrows();
}
void SetTimeFormat::menuAction()
{
    userPreferences.timeFormat = userPreferences.timeFormat;
}
void SetTimeFormat::upAction()
{
    if (userPreferences.timeFormat == 0) { userPreferences.timeFormat = 1; }
    else{ userPreferences.timeFormat = 0; }
}
void SetTimeFormat::downAction()
{
    if (userPreferences.timeFormat == 0) { userPreferences.timeFormat = 1; }
    else{ userPreferences.timeFormat = 0; }
}
void SetTimeFormat::okAction()
{
    userPreferences.timeFormat= userPreferences.timeFormat;
}

// General
void SetClockState::showStateScreen() 
{ 
    updateClockTime();
    setClockScreen.printScreen(); 
    updateHour(); 
    updateMinute();
    updateSecond();
    updateDay(); 
    updateMonth(); 
    updateYear(); 
    focusArea();
}
void SetClockState::stateLoop()
{
    updateClockTime();
    if (now.hour() != currentHour) 
    {
        currentHour = now.hour();
        updateHour(); 
    }
    if (now.minute() != currentMinute) 
    {
        currentMinute = now.minute();
        updateMinute(); 
    }
    if (now.second() != currentSecond) 
    { 
        currentSecond = now.second();
        updateSecond(); 
    }
    if (now.day() != currentDay) 
    {
        currentDay = now.day();
        updateDay(); 
        updateMonth(); 
        updateYear(); 
    }
    focusArea();
}
void SetClockState::updateHour()
{
    char hourString[3];
    snprintf(hourString, sizeof(hourString), "%02d", now.hour());
    setClockScreen.updateHour(hourString);
}
void SetClockState::updateMinute()
{
    char minuteString[3];
    snprintf(minuteString, sizeof(minuteString), "%02d", now.minute());
    setClockScreen.updateMinute(minuteString);
}
void SetClockState::updateSecond()
{
    char secondString[3];
    snprintf(secondString, sizeof(secondString), "%02d", now.second());
    setClockScreen.updateSecond(secondString);
}
void SetClockState::updateDay()
{
    char dayString[3];
    snprintf(dayString, sizeof(dayString), "%02d", now.day());
    setClockScreen.updateDay(dayString);
}
void SetClockState::updateMonth()
{
    char monthString[3];
    snprintf(monthString, sizeof(monthString), "%02d", now.month());
    setClockScreen.updateMonth(monthString);
}
void SetClockState::updateYear()
{
    char yearString[5];
    snprintf(yearString, sizeof(yearString), "%04d", now.year());
    setClockScreen.updateYear(yearString);
}


void SetHour::focusArea() { setClockScreen.focusArea(setClockScreen.hour); }
void SetMinute::focusArea() { setClockScreen.focusArea(setClockScreen.minute); }
void SetSecond::focusArea() { setClockScreen.focusArea(setClockScreen.second); }
void SetDay::focusArea() { setClockScreen.focusArea(setClockScreen.day); }
void SetMonth::focusArea() { setClockScreen.focusArea(setClockScreen.month); }
void SetYear::focusArea() { setClockScreen.focusArea(setClockScreen.year); }

void SetClockState::validateAndSetTime(DateTime dt)
{
    if(dt.isValid())
    {
        if(MIN_DATE<dt && dt<MAX_DATE)
        {
            setClockTime(dt);
            bl.mainState.updateDayValues = true;
            globalchanged = true;
        }
    }
    else{
        messageScreen.printMessages(messageScreen.ERROR, "! vthda", "ihew  tk  lhrtw  vbcn");
    }
}
void SetClockState::commonMenuAction()
{
    if(globalchanged==true)
    { 
        messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbuwb  ifsgn"); 
        globalchanged = false;
    }
    this->changed=false;
}
// Hour
void SetHour::updateHour()
{
    if (this->changed==false)
    {
        tempHour = now.hour();
    }
    char hourString[3];
    snprintf(hourString, sizeof(hourString), "%02d", tempHour);
    setClockScreen.updateHour(hourString);
}
void SetHour::menuAction() { commonMenuAction(); }
void SetHour::upAction()
{
    if (this->changed==false)
    {
        tempHour = now.hour();
    }
    this->changed=true;
    tempHour++;
    tempHour %= 24;
}
void SetHour::downAction()
{
    if (this->changed==false)
    {
        tempHour = now.hour();
    }
    this->changed=true;
    tempHour += 23;
    tempHour %= 24;
}
void SetHour::okAction()
{
    if (this->changed==true)
    {
        DateTime dt = DateTime(now.year(), now.month(), now.day(), tempHour, now.minute(), now.second());
        validateAndSetTime(dt);
        this->changed=false;
    }
}

// Minute
void SetMinute::updateMinute()
{
    if (this->changed==false)
    {
        tempMinute = now.minute();
    }
    char minuteString[3];
    snprintf(minuteString, sizeof(minuteString), "%02d", tempMinute);
    setClockScreen.updateMinute(minuteString);
}
void SetMinute::menuAction() { commonMenuAction(); }
void SetMinute::upAction()
{
    if (this->changed==false)
    {
        tempMinute = now.minute();
    }
    this->changed=true;
    tempMinute++;
    tempMinute %= 60;
}
void SetMinute::downAction()
{
    if (this->changed==false)
    {
        tempMinute = now.minute();
    }
    this->changed=true;
    tempMinute += 59;
    tempMinute %= 60;
}
void SetMinute::okAction()
{
    if (this->changed==true)
    {
        DateTime dt = DateTime(now.year(), now.month(), now.day(), now.hour(), tempMinute, now.second());
        validateAndSetTime(dt);
        this->changed=false;
    }
}

// Second
void SetSecond::updateSecond()
{
    if (this->changed==false)
    {
        tempSecond = now.second();
    }
    char secondString[3];
    snprintf(secondString, sizeof(secondString), "%02d", tempSecond);
    setClockScreen.updateSecond(secondString);
}
void SetSecond::menuAction() { commonMenuAction(); }
void SetSecond::upAction()
{
    if (this->changed==false)
    {
        tempSecond = now.second();
    }
    this->changed=true;
    tempSecond++;
    tempSecond %= 60;
}
void SetSecond::downAction()
{
    if (this->changed==false)
    {
        tempSecond = now.second();
    }
    this->changed=true;
    tempSecond += 59;
    tempSecond %= 60;
}
void SetSecond::okAction()
{
    if (this->changed==true)
    {
        DateTime dt = DateTime(now.year(), now.month(), now.day(), now.hour(), now.minute(), tempSecond);
        validateAndSetTime(dt);
        this->changed=false;
    }
}

// Day
void SetDay::updateDay()
{
    if (this->changed==false)
    {
        tempDay = now.day();
    }
    char dayString[3];
    snprintf(dayString, sizeof(dayString), "%02d", tempDay);
    setClockScreen.updateDay(dayString);
}
void SetDay::menuAction() { commonMenuAction(); }
void SetDay::upAction()
{
    if (this->changed==false)
    {
        tempDay = now.day();
    }
    this->changed=true;
    tempDay %= 31;
    tempDay++;
}
void SetDay::downAction()
{
    if (this->changed==false)
    {
        tempDay = now.day();
    }
    this->changed=true;
    tempDay += 29;
    tempDay %= 31;
    tempDay++;
}
void SetDay::okAction()
{
    if (this->changed==true)
    {
        DateTime dt = DateTime(now.year(), now.month(), tempDay, now.hour(), now.minute(), now.second());
        validateAndSetTime(dt);
        this->changed=false;
    }
}

// Month
void SetMonth::updateMonth()
{
    if (this->changed==false)
    {
        tempMonth = now.month();
    }
    char monthString[3];
    snprintf(monthString, sizeof(monthString), "%02d", tempMonth);
    setClockScreen.updateMonth(monthString);
}
void SetMonth::menuAction() { commonMenuAction(); }
void SetMonth::upAction()
{
    if (this->changed==false)
    {
        tempMonth = now.month();
    }
    this->changed=true;
    tempMonth %= 12;
    tempMonth++;
}
void SetMonth::downAction()
{
    if (this->changed==false)
    {
        tempMonth = now.month();
    }
    this->changed=true;
    tempMonth += 10;
    tempMonth %= 12;
    tempMonth++;
}
void SetMonth::okAction()
{
    if (this->changed==true)
    {
        DateTime dt = DateTime(now.year(), tempMonth, now.day(), now.hour(), now.minute(), now.second());
        validateAndSetTime(dt);
        this->changed=false;
    }
}

// Year
void SetYear::updateYear()
{
    if (this->changed==false)
    {
        tempYear = now.year();
    }
    char yearString[5];
    snprintf(yearString, sizeof(yearString), "%04d", tempYear);
    setClockScreen.updateYear(yearString);
}
void SetYear::menuAction() { commonMenuAction(); }
void SetYear::upAction()
{
    if (this->changed==false)
    {
        tempYear = now.year();
    }
    this->changed=true;
    tempYear -= 2000;
    tempYear++;
    tempYear %= 50;
    tempYear += 2000;
}
void SetYear::downAction()
{
    if (this->changed==false)
    {
        tempYear = now.year();
    }
    this->changed=true;
    tempYear -= 2000;
    tempYear --;
    tempYear += 50;
    tempYear %= 50;
    tempYear += 2000;
}
void SetYear::okAction()
{
    if (this->changed==true)
    {
        DateTime dt = DateTime(tempYear, now.month(), now.day(), now.hour(), now.minute(), now.second());
        validateAndSetTime(dt);
        this->changed=false;
    }
}

