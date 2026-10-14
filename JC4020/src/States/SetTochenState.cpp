#include "States.h"
#include "Logic.h"

extern UserPreferences userPreferences;
extern Logic bl;
extern int currentHour;
extern int currentMinute;
extern int currentSecond1;
extern int currentSecond2;
extern int currentDay;
const uint8_t numOfUpLine = 3;

void TohenUpLine::showStateScreen()
{
    bl.mainState.showStateScreen();
    mainScreen.upAll->addRoundRectBorder();
}
void TohenUpLine::stateLoop() 
{ 
    bl.mainState.stateLoop(); 
    mainScreen.upAll->addRoundRectBorder();
}
void TohenMiddleLine::showStateScreen()
{
    bl.mainState.showStateScreen();
    mainScreen.middleAll->addRoundRectBorder();
}
void TohenMiddleLine::stateLoop() 
{ 
    bl.mainState.stateLoop();
    mainScreen.middleAll->addRoundRectBorder();
}
void TohenBottomLine::showStateScreen()
{
    bl.mainState.showStateScreen();
    mainScreen.infoAll->addRoundRectBorder();
}
void TohenBottomLine::stateLoop() 
{ 
    bl.mainState.stateLoop(); 
    mainScreen.infoAll->addRoundRectBorder();
}
void SetTohenUpLine::showStateScreen()
{
    display->clearScreen();
    bl.mainState.updateUpLine();
    mainScreen.fullScreenArrow->addArrows();
}
void SetTohenUpLine::menuAction() 
{ 
    userPreferences.upLine = this->currentState;
}
void SetTohenUpLine::upAction() 
{ 
    int stateTemp = userPreferences.upLine;
    stateTemp += numOfUpLine-2;    
    stateTemp %= numOfUpLine;
    stateTemp++;
    userPreferences.upLine = stateTemp;
}
void SetTohenUpLine::downAction() 
{ 
    int stateTemp = userPreferences.upLine;
    stateTemp %= numOfUpLine;
    stateTemp++;
    userPreferences.upLine = stateTemp;
}
void SetTohenUpLine::okAction() 
{ 
    this->currentState = userPreferences.upLine;
    bl.configStates();
}

void SetTohenMiddleLine::stateLoop()
{
    updateClockTime();
    if (now.hour() != currentHour) { bl.mainState.updateHour(); }
    if (now.minute() != currentMinute) 
    { 
        bl.mainState.updateMinute(); 
    }
    if (userPreferences.middleLine==1)
    {
        if (now.second()/10 != currentSecond1) { bl.mainState.updateSecond1(); }
        if (now.second()%10 != currentSecond2) { bl.mainState.updateSecond2(); }
    }
}
void SetTohenMiddleLine::showStateScreen()
{
    display->clearScreen();
    bl.mainState.updateMiddleLine();
    mainScreen.fullScreenArrow->addArrows();
}
void SetTohenMiddleLine::menuAction() 
{ 
    userPreferences.middleLine = this->currentState;
}
void SetTohenMiddleLine::upAction() 
{ 
    int stateTemp = userPreferences.middleLine;
    stateTemp %= 2;
    stateTemp++;
    userPreferences.middleLine = stateTemp;
}
void SetTohenMiddleLine::downAction() 
{ 
    int stateTemp = userPreferences.middleLine;
    stateTemp %= 2;
    stateTemp++;
    userPreferences.middleLine = stateTemp;
}
void SetTohenMiddleLine::okAction() 
{ 
    this->currentState = userPreferences.middleLine;
    bl.configStates();
}


void meidaLamitpalelStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.meidaLamitpalel, ">");
    if(userPreferences.tfila==1) { sprintf(buffer, "%s %s", strings.meidaLamitpalel, "<"); }
}
void hagimUmoadimStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.hagimUmoadim, ">");
    if(userPreferences.moadim==1) { sprintf(buffer, "%s %s", strings.hagimUmoadim, "<"); }
}
void hagimIsraelimStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.hagimIsraelim, ">");
    if(userPreferences.showIsraeli==1) { sprintf(buffer, "%s %s", strings.hagimIsraelim, "<"); }
}
void dafHayomiBavliStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.dafHayomiBavli, ">");
    if(userPreferences.dafHayomi==1) { sprintf(buffer, "%s %s", strings.dafHayomiBavli, "<"); }
}
void dafHayomiYerushalmiStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.dafHayomiYerushalmi, ">");
    if(userPreferences.yerushalmi==1) { sprintf(buffer, "%s %s", strings.dafHayomiYerushalmi, "<"); }
}
void parashatShavuaStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.parashatShavua, ">");
    if(userPreferences.parasha==1) { sprintf(buffer, "%s %s", strings.parashatShavua, "<"); }
}
void haftaraStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.haftara, ">");
    if(userPreferences.haftara==1) { sprintf(buffer, "%s %s", strings.haftara, "<"); }
}
void ivriStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.taarichIvri, ">");
    if(userPreferences.ivri==1) { sprintf(buffer, "%s %s", strings.taarichIvri, "<"); }
}
void yomBashavuaStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.yomBashavua, ">");
    if(userPreferences.yom==1) { sprintf(buffer, "%s %s", strings.yomBashavua, "<"); }
}
void yomLezadStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.yomLezadShaon, ">");
    if(userPreferences.middleLine==2) { sprintf(buffer, "%s %s", strings.yomLezadShaon, "<"); }
}
void kezevStr(char* buffer, uint16_t dakot)
{
    sprintf(buffer, "%s %4d %s",strings.shniot, dakot, strings.kezevHahlafa);
}
void netzStr(char* buffer, int dakot)
{
    sprintf(buffer, "%s %2d %s",strings.dakot, dakot, strings.sfiraLanetz);
}

void SetTohenBottomLineKezevSet::showStateScreen() 
{
    char kezev[50];
    kezevStr(kezev, this->kezevTemp);
    char yomLezad[50];
    yomLezadStr(yomLezad);
    char netz[50];
    netzStr(netz, userPreferences.netzClk);
    menuScreen.printMenu(strings.setTohenTitle, yomLezad, kezev, netz); 
    menuScreen.kezev->addRoundRect(false, menuScreen.kezev->x, menuScreen.kezev->y+1, menuScreen.kezev->width, menuScreen.kezev->high-1, 3, YELLOW);
    menuScreen.kezev->addArrows();
}
void SetTohenBottomLineKezevSet::menuAction() 
{ 
    this->kezevTemp = userPreferences.interval;
}
void SetTohenBottomLineKezevSet::upAction() 
{
    this->kezevTemp += 5;
    if (this->kezevTemp == 605) { this->kezevTemp = 5; }
}
void SetTohenBottomLineKezevSet::downAction() 
{
    this->kezevTemp -= 5;
    if (this->kezevTemp == 0) { this->kezevTemp = 600; }
}
void SetTohenBottomLineKezevSet::okAction() 
{
    userPreferences.interval = this->kezevTemp;
    bl.configStates();
}
void SetTohenBottomLineKezev::showStateScreen() 
{ 
    char kezev[50];
    kezevStr(kezev, userPreferences.interval);
    char yomLezad[50];
    yomLezadStr(yomLezad);
    char netz[50];
    netzStr(netz, userPreferences.netzClk);
    menuScreen.printMenu(strings.menuTohen, yomLezad, kezev, netz);
}
void SetTohenBottomLineNetzSet::showStateScreen() 
{
    char kezev[50];
    kezevStr(kezev, userPreferences.interval);
    char netz[50];
    netzStr(netz, this->netzTemp);
    menuScreen.printMenu(strings.menuMinag, kezev, netz, strings.talitAlotZriha); 
    menuScreen.netz->addRoundRect(false, menuScreen.netz->x, menuScreen.netz->y+1, menuScreen.netz->width, menuScreen.netz->high-1, 3, YELLOW);
    menuScreen.netz->addArrows();
}
void SetTohenBottomLineNetzSet::menuAction() 
{ 
    this->netzTemp = userPreferences.netzClk;
}
void SetTohenBottomLineNetzSet::upAction() 
{
    this->netzTemp += 1;
    if (this->netzTemp == 61) { this->netzTemp = 0; }
}
void SetTohenBottomLineNetzSet::downAction() 
{
    this->netzTemp -= 1;
    if (this->netzTemp == -1) { this->netzTemp = 60; }
}
void SetTohenBottomLineNetzSet::okAction() 
{
    userPreferences.netzClk = this->netzTemp;
    bl.configStates();
}
void SetTohenBottomLineNetz::showStateScreen()
{ 
    char kezev[50];
    kezevStr(kezev, userPreferences.interval);
    char netz[50];
    netzStr(netz, userPreferences.netzClk);
    menuScreen.printMenu(strings.menuMinag, kezev, netz, strings.talitAlotZriha); 
}
void SetTohenBottomLineTAZ::showStateScreen() 
{ 
    char netz[50];
    netzStr(netz, userPreferences.netzClk);
    menuScreen.printMenu(strings.setTohenTitle, netz, strings.talitAlotZriha, strings.sofZmanTfila); 
}
void SetTohenBottomLineSOZT::showStateScreen() 
{  
    menuScreen.printMenu(strings.setTohenTitle, strings.talitAlotZriha, strings.sofZmanTfila, strings.sofZmanKriatShma); 
}
void SetTohenBottomLineSOZKS::showStateScreen() 
{  
    menuScreen.printMenu(strings.setTohenTitle, strings.sofZmanTfila, strings.sofZmanKriatShma, strings.hazotMinhaGMinhaK); 
}
void SetTohenBottomLineHMM::showStateScreen() 
{  
    menuScreen.printMenu(strings.setTohenTitle, strings.sofZmanKriatShma, strings.hazotMinhaGMinhaK, strings.plagShkiaaCochavim); 
}
void SetTohenBottomLinePSZ::showStateScreen() 
{  
    menuScreen.printMenu(strings.setTohenTitle, strings.hazotMinhaGMinhaK, strings.plagShkiaaCochavim, strings.hazotAlotZriha); 
}
void SetTohenBottomLineHAZ::showStateScreen() 
{  
    menuScreen.printMenu(strings.setTohenTitle, strings.plagShkiaaCochavim, strings.hazotAlotZriha, strings.zrihaSkiaaZeetHacochavim); 
}
void SetTohenBottomLineZSZ::showStateScreen() 
{  
    menuScreen.printMenu(strings.setTohenTitle, strings.hazotAlotZriha, strings.zrihaSkiaaZeetHacochavim, strings.zrihaYomShkiaa); 
}
void SetTohenBottomLineZYS::showStateScreen() 
{  
    menuScreen.printMenu(strings.setTohenTitle, strings.zrihaSkiaaZeetHacochavim, strings.zrihaYomShkiaa, strings.zrihaParashaShkiaa); 
}
void SetTohenBottomLineZPS::showStateScreen() 
{  
    menuScreen.printMenu(strings.setTohenTitle, strings.zrihaYomShkiaa, strings.zrihaParashaShkiaa, strings.cShabatParashaZShabat); 
}
void SetTohenBottomLineIPO::showStateScreen() 
{  
    menuScreen.printMenu(strings.setTohenTitle, strings.zrihaParashaShkiaa, strings.cShabatParashaZShabat, strings.cShabatZShabatRT); 
}
void SetTohenBottomLineIORT::showStateScreen() 
{ 
    char meidaLamitpalel[50];
    meidaLamitpalelStr(meidaLamitpalel);
    menuScreen.printMenu(strings.setTohenTitle, strings.cShabatParashaZShabat, strings.cShabatZShabatRT, meidaLamitpalel);
}
void SetTohenBottomLineTfila::showStateScreen() 
{ 
    char meidaLamitpalel[50];
    meidaLamitpalelStr(meidaLamitpalel);
    char hagimUmoadim[50];
    hagimUmoadimStr(hagimUmoadim);
    menuScreen.printMenu(strings.setTohenTitle, strings.cShabatZShabatRT, meidaLamitpalel, hagimUmoadim); 
}
void SetTohenBottomLineHagim::showStateScreen() 
{ 
    char meidaLamitpalel[50];
    meidaLamitpalelStr(meidaLamitpalel);
    char hagimUmoadim[50];
    hagimUmoadimStr(hagimUmoadim);
    char hagimIsraelim[50];
    hagimIsraelimStr(hagimIsraelim);
    menuScreen.printMenu(strings.setTohenTitle, meidaLamitpalel, hagimUmoadim, hagimIsraelim); 
}
void SetTohenBottomLineIsraeli::showStateScreen() 
{ 
    char hagimUmoadim[50];
    hagimUmoadimStr(hagimUmoadim);
    char hagimIsraelim[50];
    hagimIsraelimStr(hagimIsraelim);
    char dafHayomiBavli[50];
    dafHayomiBavliStr(dafHayomiBavli);
    menuScreen.printMenu(strings.setTohenTitle, hagimUmoadim, hagimIsraelim, dafHayomiBavli); 
}
void SetTohenBottomLineDafYomi::showStateScreen() 
{ 
    char hagimIsraelim[50];
    hagimIsraelimStr(hagimIsraelim);
    char dafHayomiBavli[50];
    dafHayomiBavliStr(dafHayomiBavli);
    char dafHayomiYerushalmi[50];
    dafHayomiYerushalmiStr(dafHayomiYerushalmi);
    menuScreen.printMenu(strings.setTohenTitle, hagimIsraelim, dafHayomiBavli, dafHayomiYerushalmi); 
}
void SetTohenBottomLineYerushalmi::showStateScreen() 
{ 
    char dafHayomiBavli[50];
    dafHayomiBavliStr(dafHayomiBavli);
    char dafHayomiYerushalmi[50];
    dafHayomiYerushalmiStr(dafHayomiYerushalmi);
    char parashatShavua[50];
    parashatShavuaStr(parashatShavua);
    menuScreen.printMenu(strings.setTohenTitle, dafHayomiBavli, dafHayomiYerushalmi, parashatShavua); 
}
void SetTohenBottomLineParasha::showStateScreen() 
{ 
    char parashatShavua[50];
    parashatShavuaStr(parashatShavua);
    char dafHayomiYerushalmi[50];
    dafHayomiYerushalmiStr(dafHayomiYerushalmi);
    char haftara[50];
    haftaraStr(haftara);
    menuScreen.printMenu(strings.setTohenTitle, dafHayomiYerushalmi, parashatShavua, haftara); 
}
void SetTohenBottomLineHaftara::showStateScreen() 
{ 
    char parashatShavua[50];
    parashatShavuaStr(parashatShavua);
    char haftara[50];
    haftaraStr(haftara);
    char ivri[50];
    ivriStr(ivri);
    menuScreen.printMenu(strings.setTohenTitle, parashatShavua, haftara, ivri); 
}
void SetTohenBottomLineIvri::showStateScreen() 
{ 
    char yomBashavua[50];
    yomBashavuaStr(yomBashavua);
    char ivri[50];
    ivriStr(ivri);
    char haftara[50];
    haftaraStr(haftara);
    menuScreen.printMenu(strings.setTohenTitle, haftara, ivri, yomBashavua); 
}
void SetTohenBottomLineYom::showStateScreen() 
{ 
    char ivri[50];
    ivriStr(ivri);
    char yomBashavua[50];
    yomBashavuaStr(yomBashavua);
    char yomLezad[50];
    yomLezadStr(yomLezad);
    menuScreen.printMenu(strings.setTohenTitle, ivri, yomBashavua, yomLezad); 
}
void SetTohenBottomLineYomLezad::showStateScreen() 
{ 
    char yomLezad[50];
    yomLezadStr(yomLezad);
    char yomBashavua[50];
    yomBashavuaStr(yomBashavua);
    char kezev[50];
    kezevStr(kezev, userPreferences.interval);
    menuScreen.printMenu(strings.setTohenTitle, yomBashavua, yomLezad, kezev); 
}
void SetTohenBottomLineTfila::okAction()
{     
    if(userPreferences.tfila==0) { userPreferences.tfila=1; }
    else { userPreferences.tfila=0; } 
}
void SetTohenBottomLineHagim::okAction()
{     
    if(userPreferences.moadim==0) { userPreferences.moadim=1; }
    else { userPreferences.moadim = 0; } 
}
void SetTohenBottomLineIsraeli::okAction()
{     
    if(userPreferences.showIsraeli==0) { userPreferences.showIsraeli = 1; }
    else { userPreferences.showIsraeli = 0; } 
}
void SetTohenBottomLineDafYomi::okAction()
{     
    if(userPreferences.dafHayomi==0) { userPreferences.dafHayomi = 1; }
    else { userPreferences.dafHayomi = 0; } 
}
void SetTohenBottomLineYerushalmi::okAction()
{     
    if(userPreferences.yerushalmi==0) { userPreferences.yerushalmi = 1; }
    else { userPreferences.yerushalmi = 0; } 
}
void SetTohenBottomLineParasha::okAction()
{     
    if(userPreferences.parasha==0) { userPreferences.parasha = 1; }
    else { userPreferences.parasha = 0; } 
}
void SetTohenBottomLineHaftara::okAction()
{     
    if(userPreferences.haftara==0) { userPreferences.haftara = 1; }
    else { userPreferences.haftara = 0; } 
}
void SetTohenBottomLineIvri::okAction()
{     
    if(userPreferences.ivri==0) { userPreferences.ivri = 1; }
    else { userPreferences.ivri = 0; } 
}
void SetTohenBottomLineYom::okAction()
{     
    if(userPreferences.yom==0) { userPreferences.yom = 1; }
    else { userPreferences.yom = 0; } 
}
void SetTohenBottomLineYomLezad::okAction()
{     
    if(userPreferences.middleLine==1) { userPreferences.middleLine = 2; }
    else { userPreferences.middleLine = 1; } 
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
void SetTohenBottomLineTAZ0::okAction() { userPreferences.TAZ = 0; }
void SetTohenBottomLineSOZT0::showStateScreen() { zmaneiHayomMenu(strings.sofZmanTfila, userPreferences.SOZT, 0); }
void SetTohenBottomLineSOZT0::okAction() { userPreferences.SOZT = 0; }
void SetTohenBottomLineSOZKS0::showStateScreen() { zmaneiHayomMenu(strings.sofZmanKriatShma, userPreferences.SOZKS, 0); }
void SetTohenBottomLineSOZKS0::okAction() { userPreferences.SOZKS = 0; }
void SetTohenBottomLineHMM0::showStateScreen() { zmaneiHayomMenu(strings.hazotMinhaGMinhaK, userPreferences.HMM, 0); }
void SetTohenBottomLineHMM0::okAction() { userPreferences.HMM = 0; }
void SetTohenBottomLinePSZ0::showStateScreen() { zmaneiHayomMenu(strings.plagShkiaaCochavim, userPreferences.PSZ, 0); }
void SetTohenBottomLinePSZ0::okAction() { userPreferences.PSZ = 0; }
void SetTohenBottomLineHAZ0::showStateScreen() { zmaneiHayomMenu(strings.hazotAlotZriha, userPreferences.HAZ, 0); }
void SetTohenBottomLineHAZ0::okAction() { userPreferences.HAZ = 0; }
void SetTohenBottomLineZSZh0::showStateScreen() { zmaneiHayomMenu(strings.zrihaSkiaaZeetHacochavim, userPreferences.ZSZ, 0); }
void SetTohenBottomLineZSZh0::okAction() { userPreferences.ZSZ = 0; }
void SetTohenBottomLineZSZs0::showStateScreen() { zmaneiHayomMenu(strings.zrihaSkiaaZeetHacochavim, userPreferences.ZSZ1, 0); }
void SetTohenBottomLineZSZs0::okAction() { userPreferences.ZSZ1 = 0; }

void SetTohenBottomLineTAZ1::showStateScreen() { zmaneiHayomMenu(strings.talitAlotZriha, userPreferences.TAZ, 1); }
void SetTohenBottomLineTAZ1::okAction() { userPreferences.TAZ = 1; }
void SetTohenBottomLineSOZT1::showStateScreen() { zmaneiHayomMenu(strings.sofZmanTfila, userPreferences.SOZT, 1); }
void SetTohenBottomLineSOZT1::okAction() { userPreferences.SOZT = 1;  }
void SetTohenBottomLineSOZKS1::showStateScreen() { zmaneiHayomMenu(strings.sofZmanKriatShma, userPreferences.SOZKS, 1); }
void SetTohenBottomLineSOZKS1::okAction() { userPreferences.SOZKS = 1; }
void SetTohenBottomLineHMM1::showStateScreen() { zmaneiHayomMenu(strings.hazotMinhaGMinhaK, userPreferences.HMM, 1); }
void SetTohenBottomLineHMM1::okAction() { userPreferences.HMM = 1; }
void SetTohenBottomLinePSZ1::showStateScreen() { zmaneiHayomMenu(strings.plagShkiaaCochavim, userPreferences.PSZ, 1); }
void SetTohenBottomLinePSZ1::okAction() { userPreferences.HAZ = 1; }
void SetTohenBottomLineHAZ1::showStateScreen() { zmaneiHayomMenu(strings.hazotAlotZriha, userPreferences.HAZ, 1); }
void SetTohenBottomLineHAZ1::okAction() { userPreferences.HAZ = 1; }
void SetTohenBottomLineZSZh1::showStateScreen() { zmaneiHayomMenu(strings.zrihaSkiaaZeetHacochavim, userPreferences.ZSZ, 1); }
void SetTohenBottomLineZSZh1::okAction() { userPreferences.ZSZ = 1; }
void SetTohenBottomLineZSZs1::showStateScreen() { zmaneiHayomMenu(strings.zrihaSkiaaZeetHacochavim, userPreferences.ZSZ1, 1); }
void SetTohenBottomLineZSZs1::okAction() { userPreferences.ZSZ1 = 1; }

void SetTohenBottomLineTAZ2::showStateScreen() { zmaneiHayomMenu(strings.talitAlotZriha, userPreferences.TAZ, 2); }
void SetTohenBottomLineTAZ2::okAction() { userPreferences.TAZ = 2; }
void SetTohenBottomLineSOZT2::showStateScreen() { zmaneiHayomMenu(strings.sofZmanTfila, userPreferences.SOZT, 2); }
void SetTohenBottomLineSOZT2::okAction() { userPreferences.SOZT = 2; }
void SetTohenBottomLineSOZKS2::showStateScreen() { zmaneiHayomMenu(strings.sofZmanKriatShma, userPreferences.SOZKS, 2); }
void SetTohenBottomLineSOZKS2::okAction() { userPreferences.SOZKS = 2; }
void SetTohenBottomLineHMM2::showStateScreen() { zmaneiHayomMenu(strings.hazotMinhaGMinhaK, userPreferences.HMM, 2); }
void SetTohenBottomLineHMM2::okAction() { userPreferences.HMM = 2; }
void SetTohenBottomLinePSZ2::showStateScreen() { zmaneiHayomMenu(strings.plagShkiaaCochavim, userPreferences.PSZ, 2); }
void SetTohenBottomLinePSZ2::okAction() { userPreferences.PSZ = 2;}
void SetTohenBottomLineHAZ2::showStateScreen() { zmaneiHayomMenu(strings.hazotAlotZriha, userPreferences.HAZ, 2); }
void SetTohenBottomLineHAZ2::okAction() { userPreferences.HAZ = 2;}
void SetTohenBottomLineZSZh2::showStateScreen() { zmaneiHayomMenu(strings.zrihaSkiaaZeetHacochavim, userPreferences.ZSZ, 2); }
void SetTohenBottomLineZSZh2::okAction() { userPreferences.ZSZ = 2; }
void SetTohenBottomLineZSZs2::showStateScreen() { zmaneiHayomMenu(strings.zrihaSkiaaZeetHacochavim, userPreferences.ZSZ1, 2); }
void SetTohenBottomLineZSZs2::okAction() { userPreferences.ZSZ1 = 2; }

void yemeHolStr(char* buffer, int userPreferencesZman)
{
    sprintf(buffer, "%s %s", strings.yemeHol, ">");
    if(userPreferencesZman==1 || userPreferencesZman==3) { sprintf(buffer, "%s %s", strings.yemeHol, "<"); }
}
void shishiShabatStr(char* buffer, int userPreferencesZman)
{
    sprintf(buffer, "%s %s", strings.shishiShabat, ">");
    if(userPreferencesZman==2 || userPreferencesZman==3) { sprintf(buffer, "%s %s", strings.shishiShabat, "<"); }
}
void meidaHayomMenu(const char* title, int userPreferencesZman, uint8_t select)
{
    char shishiShabat[50];
    shishiShabatStr(shishiShabat, userPreferencesZman);
    char yemeHol[50];
    yemeHolStr(yemeHol, userPreferencesZman);
    if(select==0) { menuScreen.printMenu(title, shishiShabat, yemeHol, strings.empty); }
    if(select==1) { menuScreen.printMenu(title, yemeHol, shishiShabat, strings.empty); }
}
void SetTohenBottomLineZSZh::showStateScreen() { menuScreen.printMenu(strings.zrihaSkiaaZeetHacochavim, strings.shishiShabat, strings.yemeHol, strings.empty);}
void SetTohenBottomLineZSZs::showStateScreen() { menuScreen.printMenu(strings.zrihaSkiaaZeetHacochavim, strings.yemeHol, strings.shishiShabat, strings.empty);}
void SetTohenBottomLineZYS0::showStateScreen() { meidaHayomMenu(strings.zrihaYomShkiaa, userPreferences.ZYS, 0); }
void SetTohenBottomLineZYS0::okAction() 
{
    int x = userPreferences.ZYS +1;
    if(x%2==0) x-=2;
    userPreferences.ZYS = x;
}
void SetTohenBottomLineZPS0::showStateScreen() { meidaHayomMenu(strings.zrihaParashaShkiaa, userPreferences.ZPS, 0); }
void SetTohenBottomLineZPS0::okAction() 
{
    int x = userPreferences.ZPS +1;
    if(x%2==0) x-=2;
    userPreferences.ZPS = x;
}
void SetTohenBottomLineIPO0::showStateScreen() { meidaHayomMenu(strings.cShabatParashaZShabat, userPreferences.IPO, 0); }
void SetTohenBottomLineIPO0::okAction() 
{
    int x = userPreferences.IPO +1;
    if(x%2==0) x-=2;
    userPreferences.IPO = x;
}
void SetTohenBottomLineIORT0::showStateScreen() { meidaHayomMenu(strings.cShabatZShabatRT, userPreferences.IORT, 0); }
void SetTohenBottomLineIORT0::okAction() 
{
    int x = userPreferences.IORT +1;
    if(x%2==0) x-=2;
    userPreferences.IORT = x;
}
void SetTohenBottomLineZYS1::showStateScreen() { meidaHayomMenu(strings.zrihaYomShkiaa, userPreferences.ZYS, 1); }
void SetTohenBottomLineZYS1::okAction() 
{
    int x = userPreferences.ZYS +2;
    if(x>3) x-=4;
    userPreferences.ZYS = x;
}
void SetTohenBottomLineZPS1::showStateScreen() { meidaHayomMenu(strings.zrihaParashaShkiaa, userPreferences.ZPS, 1); }
void SetTohenBottomLineZPS1::okAction() 
{
    int x = userPreferences.ZPS +2;
    if(x>3) x-=4;
    userPreferences.ZPS = x;
}
void SetTohenBottomLineIPO1::showStateScreen() { meidaHayomMenu(strings.cShabatParashaZShabat, userPreferences.IPO, 1); }
void SetTohenBottomLineIPO1::okAction() 
{
    int x = userPreferences.IPO +2;
    if(x>3) x-=4;
    userPreferences.IPO = x;
}
void SetTohenBottomLineIORT1::showStateScreen() { meidaHayomMenu(strings.cShabatZShabatRT, userPreferences.IORT, 1); }
void SetTohenBottomLineIORT1::okAction() 
{
    int x = userPreferences.IORT +2;
    if(x>3) x-=4;
    userPreferences.IORT = x;
}
