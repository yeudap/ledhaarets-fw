#include "States.h"
#include "DB/DB.h"

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
    stateIndexTimeSet = 0;
    updateDayValues = false;
}

void MainState::showStateScreen() 
{ 
    mainScreen.printScreen(); 
    if(userPreferences.clockFont == 4) { morphing=true;}
    updateUpLine();
    updateMiddleLine();
    flagTimeSetChanged=false;
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
    if(timeIs(2,0,0))
    {
        setShaonKaitzHoref();
    }
    if(timeIs(3,1,0))
    {
        resetDevice();
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
        for (uint8_t i = 0; i < numOfTimeSets; i++)
        {
            if(stateIndexTimeSet == i) { UpdateTimeSet(i); }
        }
    }
}
void MainState::UpdateTimeSet(uint8_t index)
{
    if ((this->*timeSetFuncList[index].second)()) // condition: if show this info
    {
        if (flagTimeSetChanged == false) // first time show
        {
            mainScreen.infoAll->clearArea();
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

void MainState::updateUpLine()
{
    if(userPreferences.dateFormat==8) { updateHebDate(true); }
    else { updateHebDate(false); updateLoaziDate(); }
}
void MainState::updateMiddleLine()
{
    if(morphing) 
    { 
        uint8_t _hour = now.hour();
        if (userPreferences.clockFormat == 12)
        {
            if (_hour == 0 || _hour == 12) { _hour = 12; }
            else  { _hour %= 12; }
        }
        h0.value = _hour/10;
        h1.value = _hour%10;
        m0.value = now.minute()/10;
        m1.value = now.minute()%10;
        s0.value = now.second()/10;
        s1.value = now.second()%10;
    }
    updateHour(); 
    updateMinute();
    if(userPreferences.middleLine == 1) 
    { 
        updateSecond1();
        updateSecond2();
    }
    updateNekudotaim();
    if(userPreferences.middleLine==2) { updateYom(); }
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
            h0.newValue = _hour/10;
            h1.newValue = _hour%10;
            m0.newValue = now.minute()/10;
            m1.newValue = now.minute()%10;
            s0.newValue = now.second()/10;
            s1.newValue = now.second()%10;

            MorphMulty(&h0, &h1, &m0, &m1, &s0, &s1, mainScreen.colorMiddle);
        }
        flagNetz = true;
        updateSecond();
        currentSecond = now.second();
        currentSecond1 = now.second()/10;
        currentSecond2 = now.second()%10;
    }
    if (minuteChanged()) 
    { 
        updateMinute(); 
        currentMinute = now.minute();
    }
    if (hourChanged()) 
    { 
        updateHour(); 
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
    if (userPreferences.middleLine==1)
    {
        if (now.second()%10 != currentSecond2) { updateSecond2(); }
        if (now.second()/10 != currentSecond1) { updateSecond1(); }
    }
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
    mainScreen.updateNekudotaim1(nekudotaim);
    if(userPreferences.middleLine == 1) { mainScreen.updateNekudotaim2(nekudotaim); }
}
void MainState::updateHebDate(bool fullRow)
{
    char date2[25];
    dayInfo.hebrewDate.toString(date2, 2);
    if(fullRow)
    {
        mainScreen.updateHebDateFull(date2);
    }
    else
    {
        char date1[25];
        dayInfo.hebrewDate.toString(date1, 1);
        mainScreen.updateHebDate(date1, date2);
    }
}
void MainState::updateLoaziDate()
{
    char loaziStr[30];
    dayInfo.loaziDate.toString(loaziStr, userPreferences.dateFormat);
    mainScreen.updateLoaziDate(loaziStr);
}
void MainState::updateDayUp()
{
    char dayStr[20];
    dayInfo.getDayInWeek(dayStr, true);
    mainScreen.updateLoaziDate(dayStr);
}
void MainState::updateYom()
{
    char dayStr[20];
    dayInfo.getDayInWeek(dayStr, false);
    char yomOrLeil[5];
    dayInfo.getYomOrLeil(yomOrLeil);
    mainScreen.updateYom(yomOrLeil, dayStr);
}
void MainState::updateNetz(Zman zmanLeft)
{
    char henez[10];
    dayInfo.getZriha().toStringSec(henez);
    mainScreen.update(mainScreen.infoUpHalfRight, strings.henetzHahama, txtTitle6);
    mainScreen.update(mainScreen.infoDownHalfRight, henez, txtZman);
    mainScreen.update(mainScreen.infoUpHalfLeft, strings.hazmanShenotar, txtTitle6);
    mainScreen.update(mainScreen.infoDownHalfLeft, zmanLeft, txtZman);
}
void MainState::setText()
{
    txtTitle5 = Text(mainScreen.colorBottomT, &font5, 0, 10, CENTER);
    txtValue5 = Text(mainScreen.colorBottomT, &font5, 0, 8, CENTER);
    txtTitle6 = Text(mainScreen.colorBottomT, &font6, 0, 10, CENTER);
    txtValue6 = Text(mainScreen.colorBottomV, &font6, 0, 9, CENTER);
    txtZman = Text(mainScreen.colorBottomV, &font8, 0, 10, CENTER);
}
