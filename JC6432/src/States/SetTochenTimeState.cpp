#include "States.h"
#include "Logic.h"

extern UserPreferences userPreferences;
extern Logic bl;


void TohenTimeState::showStateScreen()
{
    bl.mainState.showStateScreen();
    mainScreen.zmanAll->addRoundRectBorder();
}
void TohenTimeState::stateLoop() 
{ 
    bl.mainState.stateLoop();
    mainScreen.zmanAll->addRoundRectBorder();
}

void kezevStrTime(char* buffer, uint16_t dakot)
{
    sprintf(buffer, "%s %4d %s",strings.shniot, dakot, strings.kezevHahlafa);
}
void loaziStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.taarichLoazi, ">");
    if(userPreferences.loazi==1) { sprintf(buffer, "%s %s", strings.taarichLoazi, "<"); }
}

// Loazi
void SetTohenBottomLineLoazi::showStateScreen()
{
    char kezev[50];
    kezevStrTime(kezev, userPreferences.intervalTime);
    char loazi[50];
    loaziStr(loazi);
    menuScreen.printMenu(strings.zmaneHayom, strings.zrihaSkiaaZeetHacochavim, loazi, kezev);
}
void SetTohenBottomLineLoazi::okAction()
{     
    if(userPreferences.loazi==0) { userPreferences.loazi= 1; }
    else { userPreferences.loazi= 0; } 
}
// kezev
void SetTohenTimeStateKezev::showStateScreen()
{
    char kezev[50];
    kezevStrTime(kezev, userPreferences.intervalTime);
    menuScreen.printMenu(strings.zmaneHayom, strings.zrihaSkiaaZeetHacochavim, kezev, strings.talitAlotZriha);
}
void SetTohenTimeStateKezevSet::showStateScreen() 
{
    char kezev[50];
    kezevStrTime(kezev, this->kezevTemp);
    menuScreen.printMenu(strings.zmaneHayom, strings.zrihaSkiaaZeetHacochavim, kezev, strings.talitAlotZriha); 
    menuScreen.kezev->addRoundRectBorder();
    menuScreen.kezev->addArrows();
}
void SetTohenTimeStateKezevSet::menuAction() 
{ 
    this->kezevTemp = userPreferences.intervalTime;
}
void SetTohenTimeStateKezevSet::upAction() 
{
    this->kezevTemp += 5;
    if (this->kezevTemp == 605) { this->kezevTemp = 5; }
}
void SetTohenTimeStateKezevSet::downAction() 
{
    this->kezevTemp -= 5;
    if (this->kezevTemp == 0) { this->kezevTemp = 600; }
}
void SetTohenTimeStateKezevSet::okAction() 
{
    userPreferences.intervalTime= this->kezevTemp;
    bl.configStates();
}

void SetTohenBottomLineTAZ::showStateScreen() 
{  
    char kezev[50];
    kezevStrTime(kezev, userPreferences.intervalTime);
    menuScreen.printMenu(strings.zmaneHayom, kezev, strings.talitAlotZriha, strings.sofZmanTfila); 
}
void SetTohenBottomLineSOZT::showStateScreen() {  menuScreen.printMenu(strings.zmaneHayom, strings.talitAlotZriha, strings.sofZmanTfila, strings.sofZmanKriatShma); }
void SetTohenBottomLineSOZKS::showStateScreen() {  menuScreen.printMenu(strings.zmaneHayom, strings.sofZmanTfila, strings.sofZmanKriatShma, strings.hazotMinhaGMinhaK); }
void SetTohenBottomLineHMM::showStateScreen() {  menuScreen.printMenu(strings.zmaneHayom, strings.sofZmanKriatShma, strings.hazotMinhaGMinhaK, strings.plagShkiaaCochavim); }
void SetTohenBottomLinePSZ::showStateScreen() {  menuScreen.printMenu(strings.zmaneHayom, strings.hazotMinhaGMinhaK, strings.plagShkiaaCochavim, strings.hazotAlotZriha); }
void SetTohenBottomLineHAZ::showStateScreen() {  menuScreen.printMenu(strings.zmaneHayom, strings.plagShkiaaCochavim, strings.hazotAlotZriha, strings.zrihaSkiaaZeetHacochavim); }
void SetTohenBottomLineZSZ::showStateScreen()
{
    char kezev[50];
    kezevStrTime(kezev, userPreferences.intervalTime);
    menuScreen.printMenu(strings.zmaneHayom, strings.hazotMinhaGMinhaK, strings.zrihaSkiaaZeetHacochavim, kezev);
}

void colHayomStr(char* buffer, int userPreferencesZman)
{
    sprintf(buffer, "%s %s", strings.colHayom, "#");
    if(userPreferencesZman==2) { sprintf(buffer, "%s %s", strings.colHayom, "*"); }
}
void bazmanHamatimStr(char* buffer, int userPreferencesZman)
{
    sprintf(buffer, "%s %s", strings.bazmanHamatim, "#");
    if(userPreferencesZman==1) { sprintf(buffer, "%s %s", strings.bazmanHamatim, "*"); }
}
void bichlalLoStr(char* buffer, int userPreferencesZman)
{
    sprintf(buffer, "%s %s", strings.bichlalLo, "#");
    if(userPreferencesZman==0) { sprintf(buffer, "%s %s", strings.bichlalLo, "*"); }
}
void zmaneiHayomMenu(const char* title, int userPreferencesZman, uint8_t select)
{
    char bazmanHamatim[50];
    bazmanHamatimStr(bazmanHamatim, userPreferencesZman);
    char bichlalLo[50];
    bichlalLoStr(bichlalLo, userPreferencesZman);
    char colHayom[50];
    colHayomStr(colHayom, userPreferencesZman);
    if(select==0) { menuScreen.printMenu(title, bazmanHamatim, bichlalLo, colHayom); }
    if(select==1) { menuScreen.printMenu(title, colHayom, bazmanHamatim, bichlalLo); }
    if(select==2) { menuScreen.printMenu(title, bichlalLo, colHayom, bazmanHamatim); }
}
void SetTohenBottomLineTAZ0::showStateScreen() { zmaneiHayomMenu(strings.talitAlotZriha, userPreferences.TAZ, 0); }
void SetTohenBottomLineTAZ0::okAction() { userPreferences.TAZ= 0; }
void SetTohenBottomLineSOZT0::showStateScreen() { zmaneiHayomMenu(strings.sofZmanTfila, userPreferences.SOZT, 0); }
void SetTohenBottomLineSOZT0::okAction() { userPreferences.SOZT= 0; }
void SetTohenBottomLineSOZKS0::showStateScreen() { zmaneiHayomMenu(strings.sofZmanKriatShma, userPreferences.SOZKS, 0); }
void SetTohenBottomLineSOZKS0::okAction() { userPreferences.SOZKS= 0; }
void SetTohenBottomLineHMM0::showStateScreen() { zmaneiHayomMenu(strings.hazotMinhaGMinhaK, userPreferences.HMM, 0); }
void SetTohenBottomLineHMM0::okAction() { userPreferences.HMM= 0; }
void SetTohenBottomLinePSZ0::showStateScreen() { zmaneiHayomMenu(strings.plagShkiaaCochavim, userPreferences.PSZ, 0); }
void SetTohenBottomLinePSZ0::okAction() { userPreferences.PSZ= 0; }
void SetTohenBottomLineHAZ0::showStateScreen() { zmaneiHayomMenu(strings.hazotAlotZriha, userPreferences.HAZ, 0); }
void SetTohenBottomLineHAZ0::okAction() { userPreferences.HAZ= 0; }
void SetTohenBottomLineZSZ0::showStateScreen() { zmaneiHayomMenu(strings.zrihaSkiaaZeetHacochavim, userPreferences.ZSZ1, 0); }
void SetTohenBottomLineZSZ0::okAction() { userPreferences.ZSZ1= 0; }

void SetTohenBottomLineTAZ1::showStateScreen() { zmaneiHayomMenu(strings.talitAlotZriha, userPreferences.TAZ, 1); }
void SetTohenBottomLineTAZ1::okAction() { userPreferences.TAZ= 1; }
void SetTohenBottomLineSOZT1::showStateScreen() { zmaneiHayomMenu(strings.sofZmanTfila, userPreferences.SOZT, 1); }
void SetTohenBottomLineSOZT1::okAction() { userPreferences.SOZT= 1;  }
void SetTohenBottomLineSOZKS1::showStateScreen() { zmaneiHayomMenu(strings.sofZmanKriatShma, userPreferences.SOZKS, 1); }
void SetTohenBottomLineSOZKS1::okAction() { userPreferences.SOZKS= 1; }
void SetTohenBottomLineHMM1::showStateScreen() { zmaneiHayomMenu(strings.hazotMinhaGMinhaK, userPreferences.HMM, 1); }
void SetTohenBottomLineHMM1::okAction() { userPreferences.HMM= 1; }
void SetTohenBottomLinePSZ1::showStateScreen() { zmaneiHayomMenu(strings.plagShkiaaCochavim, userPreferences.PSZ, 1); }
void SetTohenBottomLinePSZ1::okAction() { userPreferences.PSZ= 1; }
void SetTohenBottomLineHAZ1::showStateScreen() { zmaneiHayomMenu(strings.hazotAlotZriha, userPreferences.HAZ, 1); }
void SetTohenBottomLineHAZ1::okAction() { userPreferences.HAZ= 1; }
void SetTohenBottomLineZSZ1::showStateScreen() { zmaneiHayomMenu(strings.zrihaSkiaaZeetHacochavim, userPreferences.ZSZ1, 1); }
void SetTohenBottomLineZSZ1::okAction() { userPreferences.ZSZ1= 1; }

void SetTohenBottomLineTAZ2::showStateScreen() { zmaneiHayomMenu(strings.talitAlotZriha, userPreferences.TAZ, 2); }
void SetTohenBottomLineTAZ2::okAction() { userPreferences.TAZ= 2; }
void SetTohenBottomLineSOZT2::showStateScreen() { zmaneiHayomMenu(strings.sofZmanTfila, userPreferences.SOZT, 2); }
void SetTohenBottomLineSOZT2::okAction() { userPreferences.SOZT= 2; }
void SetTohenBottomLineSOZKS2::showStateScreen() { zmaneiHayomMenu(strings.sofZmanKriatShma, userPreferences.SOZKS, 2); }
void SetTohenBottomLineSOZKS2::okAction() { userPreferences.SOZKS= 2; }
void SetTohenBottomLineHMM2::showStateScreen() { zmaneiHayomMenu(strings.hazotMinhaGMinhaK, userPreferences.HMM, 2); }
void SetTohenBottomLineHMM2::okAction() { userPreferences.HMM= 2; }
void SetTohenBottomLinePSZ2::showStateScreen() { zmaneiHayomMenu(strings.plagShkiaaCochavim, userPreferences.PSZ, 2); }
void SetTohenBottomLinePSZ2::okAction() { userPreferences.PSZ= 2;}
void SetTohenBottomLineHAZ2::showStateScreen() { zmaneiHayomMenu(strings.hazotAlotZriha, userPreferences.HAZ, 2); }
void SetTohenBottomLineHAZ2::okAction() { userPreferences.HAZ= 2;}
void SetTohenBottomLineZSZ2::showStateScreen() { zmaneiHayomMenu(strings.zrihaSkiaaZeetHacochavim, userPreferences.ZSZ1, 2); }
void SetTohenBottomLineZSZ2::okAction() { userPreferences.ZSZ1= 2; }
