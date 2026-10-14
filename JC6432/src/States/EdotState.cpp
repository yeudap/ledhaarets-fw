#include "Logic.h"

extern Logic bl;

void chabadStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.chabad, "#");
    if(userPreferences.eda==6) { sprintf(buffer, "%s %s", strings.chabad, "*"); }
}
void teimanShamiStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.teimanShami, "#");
    if(userPreferences.eda==5) { sprintf(buffer, "%s %s", strings.teimanShami, "*"); }
}
void teimanBaladiStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.teimanBaladi, "#");
    if(userPreferences.eda==4) { sprintf(buffer, "%s %s", strings.teimanBaladi, "*"); }
}
void ashkenazStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.ashkenaz, "#");
    if(userPreferences.eda==3) { sprintf(buffer, "%s %s", strings.ashkenaz, "*"); }
}
void sfaradStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.sfarad, "#");
    if(userPreferences.eda==2) { sprintf(buffer, "%s %s", strings.sfarad, "*"); }
}
void edotMizrahStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.edotHamizrah, "#");
    if(userPreferences.eda==1) { sprintf(buffer, "%s %s", strings.edotHamizrah, "*"); }
}
void bneiHulStr(char* buffer)
{
    sprintf(buffer, "%s %s", strings.bneiHul, ">");
    if(userPreferences.hul==1) { sprintf(buffer, "%s %s", strings.bneiHul, "<"); }
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
void EdotMizrah::okAction() { userPreferences.eda= 1;}

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
void EdotSfarad::okAction() { userPreferences.eda= 2;}

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
void EdotAshkenaz::okAction() { userPreferences.eda= 3;}

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
void EdotTeimanBaladi::okAction() { userPreferences.eda= 4;}

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
void EdotTeimanShami::okAction() { userPreferences.eda= 5;}

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
    userPreferences.shita= 4; 
    userPreferences.eda= 6; 
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
    if(userPreferences.hul==0) { userPreferences.hul= 1; }
    else { userPreferences.hul= 0; } 
    bl.mainState.updateDayValues = true;
}