#include "States.h"
#include "DB/DB.h"
#include "BLE/TextCommandHandler.h"

extern Place* place;
extern DayInfo dayInfo;
extern DayInfo dayInfoYesterday;
extern DayInfo dayInfoTomorrow;
extern DayInfo dayInfoNextShishi;
extern DayInfo dayInfoNextShabat;
extern int currentHour;
extern int currentMinute;
extern int currentSecond;
extern int currentSecond1;
extern int currentSecond2;
extern void MorphMulty(Digit* h0, Digit* h1, Digit* m0, Digit* m1, Digit* s0, Digit* s1, int c);

MainState::MainState()
{
    flagTimeSetChanged = false;
    flagInfoSetChanged = false;
    flagNameSetChanged = false;
    stateIndexTimeSet = 0;
    stateIndexInfoSet = 0;
    stateIndexNameSet = 0;
    indexText = 0;
    numOfValidTexts = 0;
    updateDayValues = false;
}

void MainState::initInfoSetFuncList()
{
    stateIndexInfoSet = 0;
    if(userPreferences.screen == 1) initInfoSetFuncList1();
    if(userPreferences.screen == 2) initInfoSetFuncList2();
}
void MainState::upAction() 
{ 
    int8_t curr = userPreferences.screen;
    int8_t next = curr%mainScreen.numOfMainScreens;
    next++;
    userPreferences.screen = next;
    initInfoSetFuncList();
    mainScreen.setScreen(next);
}
void MainState::downAction() 
{ 
    int8_t curr = userPreferences.screen;
    int8_t next = curr+(mainScreen.numOfMainScreens-2);
    next = next%mainScreen.numOfMainScreens;
    next++;
    userPreferences.screen = next;
    initInfoSetFuncList();
    mainScreen.setScreen(next);
}
void MainState::showStateScreen() 
{ 
    mainScreen.printScreen(); 
    updateUpLine();
    updateMiddleLine();
    flagTimeSetChanged=false;
    flagNameSetChanged=false;
    flagInfoSetChanged=false;
    if(userPreferences.clockFont == 0) { morphing=true;}
}
void MainState::stateLoop()
{
    checkTimeChanged();
    if (dayInfo.isNewDay(&now) || updateDayValues)
    {
        setNewDay();
        showStateScreen();
        updateDayValues = false;
        Serial.println("Day init succesfully!");
    }
    if (dayInfo.isAfterShkiaa(&now))
    {
        dayInfo.setAfterShkiaa();
        showStateScreen();
    }
    if (dayInfo.isZmanMinha(&now))
    {
        dayInfo.setZmanMinha();
    }
    for (uint8_t i = 0; i < numOfTimeSets; i++)
    {
        if(stateIndexTimeSet == i) { UpdateTimeSet(i); }
    }
    if(showTimeToNetz && isTimeInRange(startTimeToNetz, dayInfo.getZriha()))
    {
        if(flagNetz)
        {
            timeToNetz();
            flagNetz=false;
        }
    }
    else
    {
         for (uint8_t j = 0; j < numOfInfoSets; j++)
        {
            if(stateIndexInfoSet == j) { UpdateInfoSet(j); }
        }
    }
    for (uint8_t k = 0; k < numOfNameSets; k++)
    {
        if(stateIndexNameSet == k) { UpdateNameSet(k); }
    }
    if(timeIs(2,0,0))
    {
        setShaonKaitzHoref();
    }
    if(timeIs(3,1,0))
    {
        resetDevice();
    }
}
void MainState::UpdateTimeSet(uint8_t index)
{
    if ((this->*timeSetFuncList[index].second)()) // condition: if show this info
    {
        if (flagTimeSetChanged == false) // first time show
        {
            mainScreen.zmanAll->clearArea();
            (this->*timeSetFuncList[index].first)(); // run func: show info
            nowPlusIntervalTimeSet = now+TimeSpan(intervalTimeSet);
            flagTimeSetChanged=true;
        }
        else if (nowPlusIntervalTimeSet <= now) // time over
        {
            stateIndexTimeSet++;
            stateIndexTimeSet %= numOfTimeSets;
            flagTimeSetChanged=false;
        }
    }
    else // dont show this info
    {
        stateIndexTimeSet++;
        stateIndexTimeSet %= numOfTimeSets;
    }
}
void MainState::UpdateInfoSet(uint8_t index)
{
    if ((this->*infoSetFuncList[index].second)()) // condition: if show this info
    {
        if (flagInfoSetChanged == false) // first time show
        {
            mainScreen.infoAll->clearArea();
            (this->*infoSetFuncList[index].first)(); // run func: show info
            nowPlusIntervalInfoSet = now+TimeSpan(intervalInfoSet);
            flagInfoSetChanged=true;
        }
        else if (nowPlusIntervalInfoSet <= now) // time over
        {
            stateIndexInfoSet++;
            stateIndexInfoSet %= numOfInfoSets;
            flagInfoSetChanged=false;
        }
    }
    else // dont show this info
    {
        stateIndexInfoSet++;
        stateIndexInfoSet %= numOfInfoSets;
    }
}
void MainState::UpdateNameSet(uint8_t index)
{
    String currentText = getTextByIndex(index);
    if(currentText != "")
    {
        if (flagNameSetChanged == false) // first time show
        {
            updateIluyNishmat(&currentText); // run func: show info
            nowPlusIntervalNameSet = now+TimeSpan(intervalNameSet);
            flagNameSetChanged=true;
        }
        else if (nowPlusIntervalNameSet <= now) // time over
        {
            stateIndexNameSet++;
            stateIndexNameSet %= numOfNameSets;
            flagNameSetChanged=false;
        }
    }
    else
    {
        stateIndexNameSet++;
        stateIndexNameSet %= numOfNameSets;
    }
}
void MainState::updateUpLine()
{
    updateHebDate();
    updateParashaValue();
    updateYom();
    if(userPreferences.dateFormat != 8) updateLoaziDate();
    if(!(userPreferences.dateFormat != 8 && userPreferences.screen == 2)) updateParashaTitle();
}
void MainState::updateClockOnScreen()
{
    if(morphing) 
    { 
        uint8_t _hour = now.hour();
        if (userPreferences.clockFormat == 12)
        {
            if (_hour == 0 || _hour == 12) { _hour = 12; }
            else  { _hour %= 12; }
        }
        mainScreen.h0.value = _hour/10;
        mainScreen.h1.value = _hour%10;
        mainScreen.m0.value = now.minute()/10;
        mainScreen.m1.value = now.minute()%10;
        mainScreen.s0.value = now.second()/10;
        mainScreen.s1.value = now.second()%10;
    }
    updateHour(); 
    updateMinute();
    updateSecond1();
    updateSecond2();
    updateNekudotaim();
}
void MainState::updateMiddleLine()
{
    updateClockOnScreen();
    display->drawLine(180, 58, 180, 90, WHITE);
}
void MainState::checkTimeChanged()
{
    updateClockTime();
    if (secondChanged()) 
    { 
        if(morphing) 
        { 
            uint8_t _hour = now.hour();
            if (userPreferences.clockFormat == 12)
            {
                if (_hour == 0 || _hour == 12) { _hour = 12; }
                else  { _hour %= 12; }
            }
            mainScreen.h0.newValue = _hour/10;
            mainScreen.h1.newValue = _hour%10;
            mainScreen.m0.newValue = now.minute()/10;
            mainScreen.m1.newValue = now.minute()%10;
            mainScreen.s0.newValue = now.second()/10;
            mainScreen.s1.newValue = now.second()%10;
            MorphMulty(&mainScreen.h0, &mainScreen.h1, &mainScreen.m0, &mainScreen.m1, &mainScreen.s0, &mainScreen.s1, mainScreen.colorMiddle);
        }
        if(!morphing) updateSecond();
        flagNetz = true;
        currentSecond = now.second();
        currentSecond1 = now.second()/10;
        currentSecond2 = now.second()%10;
    }
    if (minuteChanged()) 
    { 
        if(!morphing) updateMinute(); 
        currentMinute = now.minute();
    }
    if (hourChanged()) 
    { 
        if(!morphing) updateHour(); 
        currentHour = now.hour();
    }
}
void MainState::setShaonKaitzHoref()
{
    if(userPreferences.shaonKaiz==1)
    {
        if(dayInfo.shaon==1) { setShaonKaitz(); }
        if(dayInfo.shaon==2) { setShaonHoref(); }
        dayInfo.shaon = 0;
    }
}
void MainState::resetDevice()
{
    messageScreen.printMessages(messageScreen.INFO, "! ck oha", "hwrda  kujwt  gmcn  iugav");
    ESP.restart();
}
void MainState::timeToNetz()
{
    timeLeft = netz-now;
    updateNetz(Zman(timeLeft.minutes(), timeLeft.seconds()));
}
void MainState::setNewDay()
{
    int idx = getCurrentDayIndex();
    dayInfo.initDay(idx, &now, place);

    dayInfo.tomorrow = &dayInfoTomorrow;
    DateTime tomorrow = now + TimeSpan(1,0,0,0);
    dayInfo.tomorrow->initDay(idx+1, &tomorrow, place);

    dayInfo.yesterday = &dayInfoYesterday;
    DateTime yesterday = now - TimeSpan(1,0,0,0);
    dayInfo.yesterday->initDay(idx-1, &yesterday, place);

    int8_t daysToShishi = 5-now.dayOfTheWeek();
    dayInfo.nextShishi = &dayInfoNextShishi;
    DateTime nextShishi = now + TimeSpan(daysToShishi,0,0,0);
    dayInfo.nextShishi->initDay(idx+daysToShishi, &nextShishi, place);

    uint8_t daysToShabat = 6-now.dayOfTheWeek();
    dayInfo.nextShabat = &dayInfoNextShabat;
    DateTime nextShabat = now + TimeSpan(daysToShabat,0,0,0);
    dayInfo.nextShabat->initDay(idx+daysToShabat, &nextShabat, place);
    
    startTimeToNetz = dateTimeToZman(zmanMinusDakot(dayInfo.getZriha(), userPreferences.netzClk));
    netz = zmanToDateTime(dayInfo.getZriha()); // DateTime

    dayInfo.setNewDay();
    // dayInfo.printDayInfo();
    // userPreferences.dayIndex= idx;
}
void MainState::updateHour()
{
    uint8_t _hour = now.hour();
    if (userPreferences.clockFormat == 12)
    {
        if (_hour == 0 || _hour == 12) { _hour = 12; }
        else  { _hour %= 12; }
    }    
    char hourString[3];
    snprintf(hourString, sizeof(hourString), "%02d", _hour);
    mainScreen.updateHour(hourString);
}
void MainState::updateMinute()
{
    char minuteString[3];
    snprintf(minuteString, sizeof(minuteString), "%02d", now.minute());
    mainScreen.updateMinute(minuteString);
}
void MainState::updateSecond()
{
    if (now.second()%10 != currentSecond2) { updateSecond2(); }
    if (now.second()/10 != currentSecond1) { updateSecond1(); }
}
void MainState::updateSecond1()
{
    char secondString[2];
    snprintf(secondString, sizeof(secondString), "%d", now.second()/10);
    mainScreen.updateSecond1(secondString);
}
void MainState::updateSecond2()
{
    char secondString[2];
    snprintf(secondString, sizeof(secondString), "%d", now.second()%10);
    mainScreen.updateSecond2(secondString);
}
void MainState::updateNekudotaim()
{
    char nekudotaim[2];
    snprintf(nekudotaim, sizeof(nekudotaim), "%s", ":");
    mainScreen.updateNekudotaim(nekudotaim);
}
void MainState::updateHebDate()
{
    char dateStrS[25];
    dayInfo.hebrewDate.toString(dateStrS, 3);
    char dateStrL[25];
    dayInfo.hebrewDate.toString(dateStrL, 2);
    mainScreen.updateHebDate(dateStrL, dateStrS);
}
void MainState::updateLoaziDate()
{
    char loaziStr[30];
    dayInfo.loaziDate.toString(loaziStr, userPreferences.dateFormat);
    mainScreen.updateLoaziDate(loaziStr);
}
void MainState::updateYom()
{
    Text txt = Text(mainScreen.colorYom, &ariel10, 0, 13, CENTER);
    char dayStr[20];
    dayInfo.getDayInWeek(dayStr, true);
    mainScreen.update(mainScreen.yom, dayStr, txt);
}
void MainState::updateParashaTitle()
{
    mainScreen.updateParashaTitle(strings.parashatShavua);
}
void MainState::updateParashaValue()
{
    char parasha[50];
    dayInfo.getParasha(parasha);
    mainScreen.updateParashaValue(parasha);
}
void MainState::updateNetz(Zman zmanLeft)
{
    char zmanLeftStr[100];
    sprintf(zmanLeftStr, "%s %d:%02d %s %s", strings.dakot, zmanLeft.shaot, zmanLeft.dakot, "sugc", strings.henetzHahama);
    mainScreen.updateInfo(zmanLeftStr);
}
void MainState::updateIluyNishmat(String* _shem)
{
    mainScreen.updateIluyNishmat(_shem);
}
void MainState::setText()
{
    txtTitle5 = Text(mainScreen.colorBottomT, &font5, 0, 9, CENTER);
    txtValue5 = Text(mainScreen.colorBottomT, &font5, 0, 7, CENTER);
    txtTitle6 = Text(mainScreen.colorBottomT, &font6, 0, 9, CENTER);
    txtValue6 = Text(mainScreen.colorBottomV, &font6, 0, 8, CENTER);

    txtZmanT =  Text(mainScreen.colorBottomT, &ariel10, 0, 13, CENTER);
    txtZmanV =  Text(mainScreen.colorBottomV, &ariel10, 0, 13, CENTER);
    txtInfo =   Text(mainScreen.colorUp,      &ariel10, 0, 13, CENTER);
    
    txtInfoUp =   Text(RED,      &ariel10, 0, 13, CENTER);
    txtInfoDown =   Text(WHITE,      &ariel10, 0, 13, CENTER);
}