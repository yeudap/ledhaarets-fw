#include "Logic.h"
#include "Preferences/DataPrefs.h"

extern Logic bl;

void chabadStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.chabad, "#");
    if(dataPrefs.minhagim.eda==6) { sprintf(buffer, "%s %s", strings.chabad, "*"); }
}
void teimanShamiStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.teimanShami, "#");
    if(dataPrefs.minhagim.eda==5) { sprintf(buffer, "%s %s", strings.teimanShami, "*"); }
}
void teimanBaladiStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.teimanBaladi, "#");
    if(dataPrefs.minhagim.eda==4) { sprintf(buffer, "%s %s", strings.teimanBaladi, "*"); }
}
void ashkenazStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.ashkenaz, "#");
    if(dataPrefs.minhagim.eda==3) { sprintf(buffer, "%s %s", strings.ashkenaz, "*"); }
}
void sfaradStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.sfarad, "#");
    if(dataPrefs.minhagim.eda==2) { sprintf(buffer, "%s %s", strings.sfarad, "*"); }
}
void edotMizrahStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.edotHamizrah, "#");
    if(dataPrefs.minhagim.eda==1) { sprintf(buffer, "%s %s", strings.edotHamizrah, "*"); }
}
void bneiHulStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.bneiHul, ">");
    if(dataPrefs.minhagim.hul==1) { sprintf(buffer, "%s %s", strings.bneiHul, "<"); }
}

void EdotMizrah::showStateScreen() 
{
    char bneiHul[20];
    bneiHulStr(bneiHul);
    char edotMizrah[20];
    edotMizrahStr(edotMizrah);
    char sfarad[20];
    sfaradStr(sfarad);
    menuScreen.printMenu(strings.minhagTitle, bneiHul, edotMizrah, sfarad); 
}
void EdotMizrah::okAction() { dataPrefs.minhagim.eda= 1;}

void EdotSfarad::showStateScreen() 
{ 
    char edotMizrah[20];
    edotMizrahStr(edotMizrah);
    char sfarad[20];
    sfaradStr(sfarad);
    char ashkenaz[20];
    ashkenazStr(ashkenaz);
    menuScreen.printMenu(strings.minhagTitle, edotMizrah, sfarad, ashkenaz); 
}
void EdotSfarad::okAction() { dataPrefs.minhagim.eda= 2;}

void EdotAshkenaz::showStateScreen() 
{ 
    char sfarad[20];
    sfaradStr(sfarad);
    char teimanBaladi[20];
    teimanBaladiStr(teimanBaladi);
    char ashkenaz[20];
    ashkenazStr(ashkenaz);
    menuScreen.printMenu(strings.minhagTitle, sfarad, ashkenaz, teimanBaladi); 
}
void EdotAshkenaz::okAction() { dataPrefs.minhagim.eda= 3;}

void EdotTeimanBaladi::showStateScreen() 
{ 
    char teimanBaladi[20];
    teimanBaladiStr(teimanBaladi);
    char teimanShami[20];
    teimanShamiStr(teimanShami);
    char ashkenaz[20];
    ashkenazStr(ashkenaz);
    menuScreen.printMenu(strings.minhagTitle, ashkenaz, teimanBaladi, teimanShami); 
}
void EdotTeimanBaladi::okAction() { dataPrefs.minhagim.eda= 4;}

void EdotTeimanShami::showStateScreen() 
{ 
    char chabad[20];
    chabadStr(chabad);
    char teimanBaladi[20];
    teimanBaladiStr(teimanBaladi);
    char teimanShami[20];
    teimanShamiStr(teimanShami);
    menuScreen.printMenu(strings.minhagTitle, teimanBaladi, teimanShami, chabad); 
}
void EdotTeimanShami::okAction() { dataPrefs.minhagim.eda= 5;}

void EdotChabad::showStateScreen() 
{
    char bneiHul[20];
    bneiHulStr(bneiHul);
    char chabad[20];
    chabadStr(chabad);
    char teimanShami[20];
    teimanShamiStr(teimanShami);
    menuScreen.printMenu(strings.minhagTitle, teimanShami, chabad, bneiHul); 
}
void EdotChabad::okAction() 
{ 
    dataPrefs.zmanim.shita= 4; 
    dataPrefs.minhagim.eda= 6; 
}

void EdotBneiHul::showStateScreen() 
{ 
    char bneiHul[20];
    bneiHulStr(bneiHul);
    char edotMizrah[20];
    edotMizrahStr(edotMizrah);
    char chabad[20];
    chabadStr(chabad);
    menuScreen.printMenu(strings.minhagTitle, chabad, bneiHul, edotMizrah); 
}
void EdotBneiHul::okAction()
{
    if(dataPrefs.minhagim.hul==0) { dataPrefs.minhagim.hul= 1; }
    else { dataPrefs.minhagim.hul= 0; } 
    bl.mainState.updateDayValues = true;
}