#include "Logic.h"
#include "BLE/Authmanager.h"
#include "BLE/Bluetooth.h"

extern Logic bl;
void setBTStr(char* buffer, uint8_t val)
{
    sprintf(buffer, "%s :%s", strings.on, strings.bt);
    if(val==0) { sprintf(buffer, "%s :%s", strings.off, strings.bt); }
}

void BTState::showStateScreen()          
{ 
    char btStr[50];
    setBTStr(btStr, userPreferences.bt);
    menuScreen.printMenu(strings.menuConfg, strings.MemoryMap, btStr, strings.resetPref); 
}
void ResetPrefState::showStateScreen()   
{ 
    char btStr[50];
    setBTStr(btStr, userPreferences.bt);
    menuScreen.printMenu(strings.menuConfg, btStr, strings.resetPref, strings.resetText); 
}
void ResetTextState::showStateScreen()   { menuScreen.printMenu(strings.menuConfg, strings.resetPref, strings.resetText, strings.resetPass); }
void ResetPassState::showStateScreen()   { menuScreen.printMenu(strings.menuConfg, strings.resetText, strings.resetPass, strings.aboutUs); }
void AboutUsState::showStateScreen()     { menuScreen.printMenu(strings.menuConfg, strings.resetPass, strings.aboutUs, strings.aboutDevice); }
void AboutDeviceState::showStateScreen() { menuScreen.printMenu(strings.menuConfg, strings.aboutUs, strings.aboutDevice, strings.MemoryMap); }
void MemoryMapState::showStateScreen()   
{ 
    char btStr[50];
    setBTStr(btStr, userPreferences.bt);
    menuScreen.printMenu(strings.menuConfg, strings.aboutDevice, strings.MemoryMap, btStr); 
}

// bluetooth
void SetBTState::showStateScreen() 
{ 
    char btStr[50];
    setBTStr(btStr, this->btTemp);
    menuScreen.printMenu(strings.menuConfg, strings.MemoryMap, btStr, strings.resetPref); 
    menuScreen.bt->addRoundRectBorder();
    menuScreen.bt->addArrows();
}
void SetBTState::menuAction() 
{ 
    this->btTemp = userPreferences.bt;
}
void SetBTState::upAction() 
{
    if(this->btTemp == 1) this->btTemp = 0;
    else { this->btTemp = 1; }
}
void SetBTState::downAction() 
{
    if(this->btTemp == 1) this->btTemp = 0;
    else { this->btTemp = 1; }
}
void SetBTState::okAction() 
{
    userPreferences.bt= this->btTemp;
    if(this->btTemp == 1) bleApplyState(true);
    else bleApplyState(false);
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbuwb  ifsgn");
}

// reset pref
void SetResetPrefState::showStateScreen() 
{ 
    resetScreen.SetScreen(strings.resetPrefRequest); 
    resetScreen.focusArea(reset);
}
void SetResetPrefState::menuAction() {  reset = false; }
void SetResetPrefState::upAction() { reset = !reset; }
void SetResetPrefState::downAction() { reset = !reset; }
void SetResetPrefState::okAction() 
{
    if (reset)
    {
        userPreferences.resetSimplePrefs();
        messageScreen.printMessages(messageScreen.INFO, "! ck oha", "kujwt  gmch  iugav", 3000);
        ESP.restart();
    }
    reset = false;
}

// reset text
void SetResetTextState::showStateScreen() 
{ 
    resetScreen.SetScreen(strings.resetTextRequest); 
    resetScreen.focusArea(reset);
}
void SetResetTextState::menuAction() {  reset = false; }
void SetResetTextState::upAction() { reset = !reset; }
void SetResetTextState::downAction() { reset = !reset; }
void SetResetTextState::okAction() 
{
    if (reset)
    {
        userPreferences.resetTextPrefs();
        messageScreen.printMessages(messageScreen.INFO, "! ck oha", "kujwt  gmch  iugav", 3000);
        ESP.restart();
    }
    reset = false;
}

// reset pass
void SetResetPassState::showStateScreen() 
{ 
    resetScreen.SetScreen(strings.resetPassRequest); 
    resetScreen.focusArea(reset);
}
void SetResetPassState::menuAction() {  reset = false; }
void SetResetPassState::upAction() { reset = !reset; }
void SetResetPassState::downAction() { reset = !reset; }
void SetResetPassState::okAction() 
{
    if (reset)
    {
        authManager.factoryReset();
        messageScreen.printMessages(messageScreen.INFO, "! ck oha", "kujwt  gmch  iugav", 3000);
        ESP.restart();
    }
    reset = false;
}

void ViewAboutUsState::showStateScreen() { aboutScreen.printScreen(); }
void ViewDeviceState::showStateScreen() { versionScreen.printScreen(); }
void ViewMemoryMapState::showStateScreen() { regsScreen.printScreen(); }

