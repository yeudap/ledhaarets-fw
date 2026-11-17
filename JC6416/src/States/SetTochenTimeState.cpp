#include "States.h"
#include "Logic.h"
#include "Preferences/LogicPrefs.h"

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
    if(logicPrefs.loazi==1) { sprintf(buffer, "%s %s", strings.taarichLoazi, "<"); }
}

// Loazi
void SetTohenBottomLineLoazi::showStateScreen()
{
    char kezev[50];
    kezevStrTime(kezev, logicPrefs.intervalTime);
    char loazi[50];
    loaziStr(loazi);
    menuScreen.printMenu(strings.zmaneHayom, strings.zrihaSkiaaZeetHacochavim, loazi, kezev);
}
void SetTohenBottomLineLoazi::okAction()
{     
    if(logicPrefs.loazi==0) { logicPrefs.loazi= 1; }
    else { logicPrefs.loazi= 0; } 
}
// kezev
void SetTohenTimeStateKezev::showStateScreen()
{
    char loazi[50];
    loaziStr(loazi);
    char kezev[50];
    kezevStrTime(kezev, logicPrefs.intervalTime);
    menuScreen.printMenu(strings.zmaneHayom, loazi, kezev, strings.talitAlotZriha);
}
void SetTohenTimeStateKezevSet::showStateScreen() 
{
    char loazi[50];
    loaziStr(loazi);
    char kezev[50];
    kezevStrTime(kezev, this->kezevTemp);
    menuScreen.printMenu(strings.zmaneHayom, loazi, kezev, strings.talitAlotZriha); 
    menuScreen.kezev->addRoundRectBorder();
    menuScreen.kezev->addArrows();
}
void SetTohenTimeStateKezevSet::menuAction() 
{ 
    this->kezevTemp = logicPrefs.intervalTime;
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
    logicPrefs.intervalTime= this->kezevTemp;
    bl.configStates();
}

void SetTohenBottomLineTAZ::showStateScreen() 
{  
    char kezev[50];
    kezevStrTime(kezev, logicPrefs.intervalTime);
    menuScreen.printMenu(strings.zmaneHayom, kezev, strings.talitAlotZriha, strings.sofZmanTfila); 
}
void SetTohenBottomLineSOZT::showStateScreen() {  menuScreen.printMenu(strings.zmaneHayom, strings.talitAlotZriha, strings.sofZmanTfila, strings.sofZmanKriatShma); }
void SetTohenBottomLineSOZKS::showStateScreen() {  menuScreen.printMenu(strings.zmaneHayom, strings.sofZmanTfila, strings.sofZmanKriatShma, strings.hazotMinhaGMinhaK); }
void SetTohenBottomLineHMM::showStateScreen() {  menuScreen.printMenu(strings.zmaneHayom, strings.sofZmanKriatShma, strings.hazotMinhaGMinhaK, strings.plagShkiaaCochavim); }
void SetTohenBottomLinePSZ::showStateScreen() {  menuScreen.printMenu(strings.zmaneHayom, strings.hazotMinhaGMinhaK, strings.plagShkiaaCochavim, strings.hazotAlotZriha); }
void SetTohenBottomLineHAZ::showStateScreen() {  menuScreen.printMenu(strings.zmaneHayom, strings.plagShkiaaCochavim, strings.hazotAlotZriha, strings.zrihaSkiaaZeetHacochavim); }
void SetTohenBottomLineZSZ::showStateScreen()
{
    char loazi[50];
    loaziStr(loazi);
    menuScreen.printMenu(strings.zmaneHayom, strings.hazotMinhaGMinhaK, strings.zrihaSkiaaZeetHacochavim, loazi);
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
void SetTohenBottomLineTAZ0::showStateScreen() { zmaneiHayomMenu(strings.talitAlotZriha, logicPrefs.TAZ, 0); }
void SetTohenBottomLineTAZ0::okAction() { logicPrefs.TAZ= 0; }
void SetTohenBottomLineSOZT0::showStateScreen() { zmaneiHayomMenu(strings.sofZmanTfila, logicPrefs.SOZT, 0); }
void SetTohenBottomLineSOZT0::okAction() { logicPrefs.SOZT= 0; }
void SetTohenBottomLineSOZKS0::showStateScreen() { zmaneiHayomMenu(strings.sofZmanKriatShma, logicPrefs.SOZKS, 0); }
void SetTohenBottomLineSOZKS0::okAction() { logicPrefs.SOZKS= 0; }
void SetTohenBottomLineHMM0::showStateScreen() { zmaneiHayomMenu(strings.hazotMinhaGMinhaK, logicPrefs.HMM, 0); }
void SetTohenBottomLineHMM0::okAction() { logicPrefs.HMM= 0; }
void SetTohenBottomLinePSZ0::showStateScreen() { zmaneiHayomMenu(strings.plagShkiaaCochavim, logicPrefs.PSZ, 0); }
void SetTohenBottomLinePSZ0::okAction() { logicPrefs.PSZ= 0; }
void SetTohenBottomLineHAZ0::showStateScreen() { zmaneiHayomMenu(strings.hazotAlotZriha, logicPrefs.HAZ, 0); }
void SetTohenBottomLineHAZ0::okAction() { logicPrefs.HAZ= 0; }
void SetTohenBottomLineZSZ0::showStateScreen() { zmaneiHayomMenu(strings.zrihaSkiaaZeetHacochavim, logicPrefs.ZSZ1, 0); }
void SetTohenBottomLineZSZ0::okAction() { logicPrefs.ZSZ1= 0; }

void SetTohenBottomLineTAZ1::showStateScreen() { zmaneiHayomMenu(strings.talitAlotZriha, logicPrefs.TAZ, 1); }
void SetTohenBottomLineTAZ1::okAction() { logicPrefs.TAZ= 1; }
void SetTohenBottomLineSOZT1::showStateScreen() { zmaneiHayomMenu(strings.sofZmanTfila, logicPrefs.SOZT, 1); }
void SetTohenBottomLineSOZT1::okAction() { logicPrefs.SOZT= 1;  }
void SetTohenBottomLineSOZKS1::showStateScreen() { zmaneiHayomMenu(strings.sofZmanKriatShma, logicPrefs.SOZKS, 1); }
void SetTohenBottomLineSOZKS1::okAction() { logicPrefs.SOZKS= 1; }
void SetTohenBottomLineHMM1::showStateScreen() { zmaneiHayomMenu(strings.hazotMinhaGMinhaK, logicPrefs.HMM, 1); }
void SetTohenBottomLineHMM1::okAction() { logicPrefs.HMM= 1; }
void SetTohenBottomLinePSZ1::showStateScreen() { zmaneiHayomMenu(strings.plagShkiaaCochavim, logicPrefs.PSZ, 1); }
void SetTohenBottomLinePSZ1::okAction() { logicPrefs.PSZ= 1; }
void SetTohenBottomLineHAZ1::showStateScreen() { zmaneiHayomMenu(strings.hazotAlotZriha, logicPrefs.HAZ, 1); }
void SetTohenBottomLineHAZ1::okAction() { logicPrefs.HAZ= 1; }
void SetTohenBottomLineZSZ1::showStateScreen() { zmaneiHayomMenu(strings.zrihaSkiaaZeetHacochavim, logicPrefs.ZSZ1, 1); }
void SetTohenBottomLineZSZ1::okAction() { logicPrefs.ZSZ1= 1; }

void SetTohenBottomLineTAZ2::showStateScreen() { zmaneiHayomMenu(strings.talitAlotZriha, logicPrefs.TAZ, 2); }
void SetTohenBottomLineTAZ2::okAction() { logicPrefs.TAZ= 2; }
void SetTohenBottomLineSOZT2::showStateScreen() { zmaneiHayomMenu(strings.sofZmanTfila, logicPrefs.SOZT, 2); }
void SetTohenBottomLineSOZT2::okAction() { logicPrefs.SOZT= 2; }
void SetTohenBottomLineSOZKS2::showStateScreen() { zmaneiHayomMenu(strings.sofZmanKriatShma, logicPrefs.SOZKS, 2); }
void SetTohenBottomLineSOZKS2::okAction() { logicPrefs.SOZKS= 2; }
void SetTohenBottomLineHMM2::showStateScreen() { zmaneiHayomMenu(strings.hazotMinhaGMinhaK, logicPrefs.HMM, 2); }
void SetTohenBottomLineHMM2::okAction() { logicPrefs.HMM= 2; }
void SetTohenBottomLinePSZ2::showStateScreen() { zmaneiHayomMenu(strings.plagShkiaaCochavim, logicPrefs.PSZ, 2); }
void SetTohenBottomLinePSZ2::okAction() { logicPrefs.PSZ= 2;}
void SetTohenBottomLineHAZ2::showStateScreen() { zmaneiHayomMenu(strings.hazotAlotZriha, logicPrefs.HAZ, 2); }
void SetTohenBottomLineHAZ2::okAction() { logicPrefs.HAZ= 2;}
void SetTohenBottomLineZSZ2::showStateScreen() { zmaneiHayomMenu(strings.zrihaSkiaaZeetHacochavim, logicPrefs.ZSZ1, 2); }
void SetTohenBottomLineZSZ2::okAction() { logicPrefs.ZSZ1= 2; }
