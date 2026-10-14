#include "States.h"
#include "Logic.h"

extern UserPreferences userPreferences;
extern Logic bl;


void TohenInfoState::showStateScreen()
{
    bl.mainState.showStateScreen();
    mainScreen.infoAll->addRoundRectBorder();
}
void TohenInfoState::stateLoop() 
{ 
    bl.mainState.stateLoop(); 
    mainScreen.infoAll->addRoundRectBorder();
}


void netzStr(char* buffer, int dakot)
{
    sprintf(buffer, "%s %2d %s",strings.dakot, dakot, strings.sfiraLanetz);
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

// kazev
void kezevStrInfo(char* buffer, uint16_t dakot)
{
    sprintf(buffer, "%s %4d %s",strings.shniot, dakot, strings.kezevHahlafa);
}
void SetTohenInfoStateKezev::showStateScreen()
{
    char kezev[50];
    kezevStrInfo(kezev, userPreferences.intervalInfo);
    char netz[50];
    netzStr(netz, userPreferences.netzClk);
    menuScreen.printMenu(strings.meidaHayom, strings.cShabatZShabatRT, kezev, netz); 
};
void SetTohenInfoStateKezevSet::showStateScreen() 
{
    char kezev[50];
    kezevStrInfo(kezev, this->kezevTemp);
    char netz[50];
    netzStr(netz, userPreferences.netzClk);
    menuScreen.printMenu(strings.meidaHayom, strings.cShabatZShabatRT, kezev, netz); 
    menuScreen.kezev->addRoundRect(false, menuScreen.kezev->x, menuScreen.kezev->y+1, menuScreen.kezev->width, menuScreen.kezev->high-1, 3, YELLOW);
    menuScreen.kezev->addArrows();
}
void SetTohenInfoStateKezevSet::menuAction() 
{ 
    this->kezevTemp = userPreferences.intervalInfo;
}
void SetTohenInfoStateKezevSet::upAction() 
{
    this->kezevTemp += 5;
    if (this->kezevTemp == 605) { this->kezevTemp = 5; }
}
void SetTohenInfoStateKezevSet::downAction() 
{
    this->kezevTemp -= 5;
    if (this->kezevTemp == 0) { this->kezevTemp = 600; }
}
void SetTohenInfoStateKezevSet::okAction() 
{
    userPreferences.intervalInfo= this->kezevTemp;
    bl.configStates();
}

// netz
void SetTohenBottomLineNetz::showStateScreen()
{ 
    char kezev[50];
    kezevStrInfo(kezev, userPreferences.intervalInfo);
    char netz[50];
    netzStr(netz, userPreferences.netzClk);
    char meidaLamitpalel[50];
    meidaLamitpalelStr(meidaLamitpalel);
    menuScreen.printMenu(strings.meidaHayom, kezev, netz, meidaLamitpalel); 
}
void SetTohenBottomLineNetzSet::showStateScreen() 
{
    char kezev[50];
    kezevStrInfo(kezev, userPreferences.intervalInfo);
    char netz[50];
    netzStr(netz, this->netzTemp);
    char meidaLamitpalel[50];
    meidaLamitpalelStr(meidaLamitpalel);
    menuScreen.printMenu(strings.meidaHayom, kezev, netz, meidaLamitpalel); 
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
    userPreferences.netzClk= this->netzTemp;
    bl.configStates();
}

void SetTohenBottomLineTfila::showStateScreen()
{
    char netz[50];
    netzStr(netz, userPreferences.netzClk);
    char meidaLamitpalel[50];
    meidaLamitpalelStr(meidaLamitpalel);
    char hagimUmoadim[50];
    hagimUmoadimStr(hagimUmoadim);
    menuScreen.printMenu(strings.meidaHayom, netz, meidaLamitpalel, hagimUmoadim);
}
void SetTohenBottomLineHagim::showStateScreen()
{
    char meidaLamitpalel[50];
    meidaLamitpalelStr(meidaLamitpalel);
    char hagimUmoadim[50];
    hagimUmoadimStr(hagimUmoadim);
    char hagimIsraelim[50];
    hagimIsraelimStr(hagimIsraelim);
    menuScreen.printMenu(strings.meidaHayom, meidaLamitpalel, hagimUmoadim, hagimIsraelim); 
}
void SetTohenBottomLineIsraeli::showStateScreen()
{
    char hagimUmoadim[50];
    hagimUmoadimStr(hagimUmoadim);
    char hagimIsraelim[50];
    hagimIsraelimStr(hagimIsraelim);
    char dafHayomiBavli[50];
    dafHayomiBavliStr(dafHayomiBavli);
    menuScreen.printMenu(strings.meidaHayom, hagimUmoadim, hagimIsraelim, dafHayomiBavli); 
}
void SetTohenBottomLineDafYomi::showStateScreen()
{
    char hagimIsraelim[50];
    hagimIsraelimStr(hagimIsraelim);
    char dafHayomiBavli[50];
    dafHayomiBavliStr(dafHayomiBavli);
    char dafHayomiYerushalmi[50];
    dafHayomiYerushalmiStr(dafHayomiYerushalmi);
    menuScreen.printMenu(strings.meidaHayom, hagimIsraelim, dafHayomiBavli, dafHayomiYerushalmi);
}
void SetTohenBottomLineYerushalmi::showStateScreen()
{
    char dafHayomiBavli[50];
    dafHayomiBavliStr(dafHayomiBavli);
    char dafHayomiYerushalmi[50];
    dafHayomiYerushalmiStr(dafHayomiYerushalmi);
    char haftara[50];
    haftaraStr(haftara);
    menuScreen.printMenu(strings.meidaHayom, dafHayomiBavli, dafHayomiYerushalmi, haftara);
}
void SetTohenBottomLineHaftara::showStateScreen() 
{ 
    char dafHayomiYerushalmi[50];
    dafHayomiYerushalmiStr(dafHayomiYerushalmi);
    char haftara[50];
    haftaraStr(haftara);
    char ivri[50];
    ivriStr(ivri);
    menuScreen.printMenu(strings.meidaHayom, dafHayomiYerushalmi, haftara, ivri); 
}
void SetTohenBottomLineIvri::showStateScreen()
{
    char haftara[50];
    haftaraStr(haftara);
    char ivri[50];
    ivriStr(ivri);
    menuScreen.printMenu(strings.meidaHayom, haftara, ivri, strings.cShabatZShabatRT);
}
void SetTohenBottomLineIORT::showStateScreen()
{
    char ivri[50];
    ivriStr(ivri);
    char kezev[50];
    kezevStrInfo(kezev, userPreferences.intervalInfo);
    menuScreen.printMenu(strings.meidaHayom, ivri, strings.cShabatZShabatRT, kezev);
}
void SetTohenBottomLineTfila::okAction()
{     
    if(userPreferences.tfila==0) { userPreferences.tfila= 1; }
    else { userPreferences.tfila= 0; } 
}
void SetTohenBottomLineHagim::okAction()
{     
    if(userPreferences.moadim==0) { userPreferences.moadim= 1; }
    else { userPreferences.moadim= 0; } 
}
void SetTohenBottomLineIsraeli::okAction()
{     
    if(userPreferences.showIsraeli==0) { userPreferences.showIsraeli= 1; }
    else { userPreferences.showIsraeli= 0; } 
}
void SetTohenBottomLineDafYomi::okAction()
{     
    if(userPreferences.dafHayomi==0) { userPreferences.dafHayomi= 1; }
    else { userPreferences.dafHayomi= 0; } 
}
void SetTohenBottomLineYerushalmi::okAction()
{     
    if(userPreferences.yerushalmi==0) { userPreferences.yerushalmi= 1; }
    else { userPreferences.yerushalmi= 0; } 
}
void SetTohenBottomLineHaftara::okAction()
{     
    if(userPreferences.haftara==0) { userPreferences.haftara= 1; }
    else { userPreferences.haftara= 0; } 
}
void SetTohenBottomLineIvri::okAction()
{     
    if(userPreferences.ivri==0) { userPreferences.ivri= 1; }
    else { userPreferences.ivri= 0; } 
}

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
void SetTohenBottomLineIORT0::showStateScreen() { meidaHayomMenu(strings.cShabatZShabatRT, userPreferences.IORT, 0); }
void SetTohenBottomLineIORT0::okAction() 
{
    int x = userPreferences.IORT +1;
    if(x%2==0) x-=2;
    userPreferences.IORT= x;
}
void SetTohenBottomLineIORT1::showStateScreen() { meidaHayomMenu(strings.cShabatZShabatRT, userPreferences.IORT, 1); }
void SetTohenBottomLineIORT1::okAction() 
{
    int x = userPreferences.IORT +2;
    if(x>3) x-=4;
    userPreferences.IORT= x;
}
