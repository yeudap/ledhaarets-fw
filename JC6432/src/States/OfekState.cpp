#include "States.h"
#include "Logic.h"
#include "DB\Place.h"

extern UserPreferences userPreferences;
extern Logic bl;
extern int numOfOfeks;
extern Place places[];
extern int lonPre;
extern int lonInt;
extern int lonFrac;
extern int latPre;
extern int latInt;
extern int latFrac;
bool globalchangedOfek = false;

void irMukefetHomaStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.irMukefetHoma, ">");
    if(userPreferences.homa==1) { sprintf(buffer, "%s %s", strings.irMukefetHoma, "<"); }
}
void OfekState::showStateScreen() 
{ 
    char irMukefetHoma[50];
    irMukefetHomaStr(irMukefetHoma);
    menuScreen.printMenu(strings.ofekTitle, irMukefetHoma, strings.bharOfek, strings.hagderMakom); 
}
void CustomAreaState::showStateScreen() 
{ 
    char irMukefetHoma[50];
    irMukefetHomaStr(irMukefetHoma);
    menuScreen.printMenu(strings.ofekTitle, strings.bharOfek, strings.hagderMakom, irMukefetHoma); 
}
void SetHomaCustomState::showStateScreen() 
{ 
    char irMukefetHoma[50];
    irMukefetHomaStr(irMukefetHoma);
    menuScreen.printMenu(strings.ofekTitle, strings.hagderMakom, irMukefetHoma, strings.bharOfek); 
}
void SetHomaCustomState::okAction()
{
    if(userPreferences.homa==0) { userPreferences.homa= 1; }
    else { userPreferences.homa= 0; } 
}
void SetOfekState::showStateScreen() 
{ 
    uint8_t minus = ofekTemp+numOfOfeks-1;
    minus %= numOfOfeks;
    uint8_t plus = ofekTemp+1;
    plus %= numOfOfeks;
    menuScreen.printMenu(strings.ofekTitle, places[minus].name, places[ofekTemp].name, places[plus].name); 
}
void SetOfekState::menuAction() 
{ 
    this->ofekTemp = userPreferences.ofek;
}
void SetOfekState::upAction() 
{
    this->ofekTemp+=numOfOfeks-1;
    this->ofekTemp %= numOfOfeks;
}
void SetOfekState::downAction() 
{
    this->ofekTemp++;
    this->ofekTemp %= numOfOfeks;
}
void SetOfekState::okAction() 
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
    userPreferences.ofek= this->ofekTemp;
    userPreferences.nerot= places[this->ofekTemp].nerot;
    userPreferences.homa= places[this->ofekTemp].homa;

    initPlace();
    if(places[this->ofekTemp].hul) { userPreferences.hul= 1; }
    if(!places[this->ofekTemp].hul) { userPreferences.hul= 0; }
    bl.configStates();
    bl.mainState.updateDayValues = true;
}

void lonStr(char* buffer)
{
    sprintf(buffer, "%.2f : %s",userPreferences.nz_long.getValue() , strings.kavOrech);
}
void latStr(char* buffer)
{
    sprintf(buffer, "%.2f : %s",userPreferences.nz_lat.getValue() , strings.kavRohav);
}
void govaStr(char* buffer, int gova)
{
    sprintf(buffer, "%+d : %s", gova, strings.gova);
}
void hulStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.huzLaaretz, ">");
    if(userPreferences.nz_hul==1) { sprintf(buffer, "%s %s", strings.huzLaaretz, "<"); }
}
void tzStr(char* buffer, int tz)
{
    sprintf(buffer, "%+d : %s", tz, strings.ezorZman);
}
void LonCustomState::showStateScreen()
{
    char lon[50];
    lonStr(lon);
    char lat[50];
    latStr(lat);
    char tz[50];
    tzStr(tz, userPreferences.timezone);
    menuScreen.printMenu(strings.ofekTitle, tz, lon, lat); 
}
void LatCustomState::showStateScreen()
{
    char lon[50];
    lonStr(lon);
    char lat[50];
    latStr(lat);
    char gova[50];
    govaStr(gova, userPreferences.nz_height);
    menuScreen.printMenu(strings.ofekTitle, lon, lat, gova); 
}
void GovaCustomState::showStateScreen()
{
    char lat[50];
    latStr(lat);
    char hul[50];
    hulStr(hul);
    char gova[50];
    govaStr(gova, userPreferences.nz_height);
    menuScreen.printMenu(strings.ofekTitle, lat, gova, hul); 
}
void SetHulCustomState::showStateScreen()
{
    char tz[50];
    tzStr(tz, userPreferences.timezone);
    char hul[50];
    hulStr(hul);
    char gova[50];
    govaStr(gova, userPreferences.nz_height);
    menuScreen.printMenu(strings.ofekTitle, gova, hul, tz); 
}
void TimeZoneCustomState::showStateScreen()
{
    char lon[50];
    lonStr(lon);
    char tz[50];
    tzStr(tz, userPreferences.timezone);
    char hul[50];
    hulStr(hul);
    menuScreen.printMenu(strings.ofekTitle, hul, tz, lon); 
}

// hul
void SetHulCustomState::okAction()
{
    if(userPreferences.nz_hul==0) { userPreferences.nz_hul= 1; userPreferences.hul= 1; }
    else { userPreferences.nz_hul= 0; userPreferences.hul= 0; } 
}

// Gova
void SetGovaCustomState::showStateScreen() 
{
    char lat[50];
    latStr(lat);
    char hul[50];
    hulStr(hul);
    char gova[50];
    govaStr(gova, this->tempGova);
    menuScreen.printMenu(strings.ofekTitle, lat, gova, hul); 
    menuScreen.timeZone->addRoundRectBorder();
    menuScreen.timeZone->addArrows();
}
void SetGovaCustomState::menuAction() 
{ 
    this->tempGova = userPreferences.nz_height;
}
void SetGovaCustomState::upAction() 
{
    this->tempGova += 10;
    if (this->tempGova == 1000) { this->tempGova = -200; }
}
void SetGovaCustomState::downAction() 
{
    this->tempGova -= 10;
    if (this->tempGova == -210) { this->tempGova = 990; }
}
void SetGovaCustomState::okAction() 
{
    if(this->tempGova != userPreferences.nz_height)
    {
        userPreferences.nz_height= this->tempGova;
        initPlace();
        bl.configStates();
        bl.mainState.updateDayValues = true;   
    }
}

// TimeZone
void SetTimeZoneCustomState::showStateScreen() 
{
    char lon[50];
    lonStr(lon);
    char lat[50];
    latStr(lat);
    char tz[50];
    tzStr(tz, this->tempTZ);
    menuScreen.printMenu(strings.ofekTitle, lon, tz, lat); 
    menuScreen.timeZone->addRoundRectBorder();
    menuScreen.timeZone->addArrows();
}
void SetTimeZoneCustomState::menuAction() 
{ 
    this->tempTZ = userPreferences.timezone;
}
void SetTimeZoneCustomState::upAction() 
{
    this->tempTZ++;
    if (this->tempTZ == 13) { this->tempTZ = -12; }
}
void SetTimeZoneCustomState::downAction() 
{
    this->tempTZ--;
    if (this->tempTZ == -13) { this->tempTZ = 12; }
}
void SetTimeZoneCustomState::okAction() 
{
    if(this->tempTZ != userPreferences.timezone)
    {
        userPreferences.timezone= this->tempTZ;
        initPlace();
        bl.configStates();
        bl.mainState.updateDayValues = true;   
    }
}

// Long
void SetLonPre::focusArea() { setClockScreen.hour->addArrows(); }
void SetLonInt::focusArea() { setClockScreen.minute->addArrows(); }
void SetLonFrac::focusArea() { setClockScreen.second->addArrows(); }


void SetLonCustomState::updatePre()
{
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%s", "+");
    if (lonPre<0) { snprintf(buffer, sizeof(buffer), "%s", "-"); }
    setClockScreen.updateHour(buffer);
}
void SetLonCustomState::updateInt()
{
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%d", lonInt);
    setClockScreen.updateMinute(buffer);
}
void SetLonCustomState::updateFrac()
{
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%02d", lonFrac);
    setClockScreen.updateSecond(buffer);
}
void SetLonCustomState::showStateScreen() 
{ 
    // Serial.println("");
    // Serial.println("==================================");
    // Serial.println("showStateScreen");
    // Serial.print("userPreferences.nz_long: ");
    // Serial.println(userPreferences.nz_long);
    // Serial.print("lon: ");
    // if (lonPre <0) { Serial.print("-"); }
    // else { Serial.print("+"); }
    // Serial.print(lonInt);
    // Serial.print(".");
    // Serial.print(lonFrac);
    // Serial.println("");
    display->clearScreen();
    updatePre();
    updateInt();
    updateFrac();
    focusArea();
    setClockScreen.printPoint();
}

void SetLonCustomState::menuAction()
{
    if(globalchangedOfek==true)
    { 
        double_t lon = lonInt;
        lon += double(lonFrac)/100;
        lon *= lonPre;
        userPreferences.nz_long= lon;
        printUpdateSucc();
    }
    this->changed=false;
    this->tempPre = lonPre;
    this->tempInt = lonInt;
    this->tempFrac = lonFrac;
}
void SetLonPre::updatePre()
{
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%s", "+");
    if (tempPre<0) { snprintf(buffer, sizeof(buffer), "%s", "-"); }
    setClockScreen.updateHour(buffer);
}
void SetLonPre::upAction()
{
    this->changed=true;
    tempPre *= -1;
}
void SetLonPre::downAction()
{
    this->changed=true;
    tempPre *= -1;
}
void SetLonPre::okAction()
{
    if(this->changed==true && tempPre!=lonPre)
    { 
        globalchangedOfek=true;
        lonPre = tempPre;
        this->changed=false;
    }
}
void SetLonInt::updateInt()
{
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%d", tempInt);
    setClockScreen.updateMinute(buffer);
}
void SetLonInt::upAction()
{
    this->changed=true;
    this->tempInt += 1;
    if (this->tempInt == 180) { this->tempInt = 0; }
}
void SetLonInt::downAction()
{
    this->changed=true;
    this->tempInt -= 1;
    if (this->tempInt == -1) { this->tempInt = 179; }
}
void SetLonInt::okAction()
{
    if(this->changed==true && tempInt!=lonInt)
    { 
        globalchangedOfek=true;
        lonInt = tempInt;
        this->changed=false;
    }
}
void SetLonFrac::updateFrac()
{
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%02d", tempFrac);
    setClockScreen.updateSecond(buffer);
}
void SetLonFrac::upAction()
{
    this->changed=true;
    this->tempFrac++;
    if (this->tempFrac == 100) { this->tempFrac = 0; }
}
void SetLonFrac::downAction()
{
    this->changed=true;
    this->tempFrac--;
    if (this->tempFrac == -1) { this->tempFrac = 99; }
}
void SetLonFrac::okAction()
{
    if(this->changed==true && tempFrac!=lonFrac)
    { 
        globalchangedOfek=true;
        lonFrac = tempFrac;
        this->changed=false;
    }
}

// Lat
void SetLatPre::focusArea() { setClockScreen.hour->addArrows(); }
void SetLatInt::focusArea() { setClockScreen.minute->addArrows(); }
void SetLatFrac::focusArea() { setClockScreen.second->addArrows(); }


void SetLatCustomState::updatePre()
{
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%s", "+");
    if (latPre<0) { snprintf(buffer, sizeof(buffer), "%s", "-"); }
    setClockScreen.updateHour(buffer);
}
void SetLatCustomState::updateInt()
{
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%d", latInt);
    setClockScreen.updateMinute(buffer);
}
void SetLatCustomState::updateFrac()
{
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%02d", latFrac);
    setClockScreen.updateSecond(buffer);
}
void SetLatCustomState::showStateScreen() 
{ 
    display->clearScreen();
    updatePre();
    updateInt();
    updateFrac();
    focusArea();
    setClockScreen.printPoint();
}

void SetLatCustomState::menuAction()
{
    if(globalchangedOfek==true)
    { 
        double_t lat = latInt;
        lat += double(latFrac)/100;
        lat *= latPre;
        userPreferences.nz_lat= lat;
        printUpdateSucc();
    }
    this->changed=false;
    this->tempPre = latPre;
    this->tempInt = latInt;
    this->tempFrac = latFrac;
}
void SetLatPre::updatePre()
{
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%s", "+");
    if (tempPre<0) { snprintf(buffer, sizeof(buffer), "%s", "-"); }
    setClockScreen.updateHour(buffer);
}
void SetLatPre::upAction()
{
    this->changed=true;
    tempPre *= -1;
}
void SetLatPre::downAction()
{
    this->changed=true;
    tempPre *= -1;
}
void SetLatPre::okAction()
{
    if(this->changed==true && tempPre!=latPre)
    { 
        globalchangedOfek=true;
        latPre = tempPre;
        this->changed=false;
    }
}
void SetLatInt::updateInt()
{
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%d", tempInt);
    setClockScreen.updateMinute(buffer);
}
void SetLatInt::upAction()
{
    this->changed=true;
    this->tempInt += 1;
    if (this->tempInt == 90) { this->tempInt = 0; }
}
void SetLatInt::downAction()
{
    this->changed=true;
    this->tempInt -= 1;
    if (this->tempInt == -1) { this->tempInt = 89; }
}
void SetLatInt::okAction()
{
    if(this->changed==true && tempInt!=latInt)
    { 
        globalchangedOfek=true;
        latInt = tempInt;
        this->changed=false;
    }
}
void SetLatFrac::updateFrac()
{
    char buffer[5];
    snprintf(buffer, sizeof(buffer), "%02d", tempFrac);
    setClockScreen.updateSecond(buffer);
}
void SetLatFrac::upAction()
{
    this->changed=true;
    this->tempFrac++;
    if (this->tempFrac == 100) { this->tempFrac = 0; }
}
void SetLatFrac::downAction()
{
    this->changed=true;
    this->tempFrac--;
    if (this->tempFrac == -1) { this->tempFrac = 99; }
}
void SetLatFrac::okAction()
{
    if(this->changed==true && tempFrac!=latFrac)
    { 
        globalchangedOfek=true;
        latFrac = tempFrac;
        this->changed=false;
    }
}

void SetCustomState::printUpdateSucc()
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbuwb  ifsgn"); 
    globalchangedOfek = false;
    initPlace();
    userPreferences.ofek= 0;
    bl.configStates();
    bl.mainState.updateDayValues = true;
}