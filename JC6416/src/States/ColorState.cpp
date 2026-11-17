#include "Logic.h"
#include "Preferences/DisplayPrefs.h"

extern Logic bl;


void ColorUpLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    mainScreen.loaziDate->addRoundRectBorder();
    mainScreen.ivriDate->addRoundRectBorder();
}
void ColorUpLine::stateLoop() 
{ 
    bl.mainState.stateLoop(); 
    mainScreen.loaziDate->addRoundRectBorder();
    mainScreen.ivriDate->addRoundRectBorder();
}
void ColorMiddleLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    mainScreen.clockAll->addRoundRectBorder();
}
void ColorMiddleLine::stateLoop()
{
    bl.mainState.stateLoop();
    mainScreen.clockAll->addRoundRectBorder();
}
void ColorBottomTLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    bl.mainState.timeSetZrihaShkiaaCochavim();
    bl.mainState.infoSetShabatInShabatOutRabenuTam();
    mainScreen.zmanUp->addRoundRectBorder();
    mainScreen.infoUp->addRoundRectBorder();
}
void ColorBottomTLine::stateLoop()
{
    bl.mainState.stateLoop();
    bl.mainState.timeSetZrihaShkiaaCochavim();
    bl.mainState.infoSetShabatInShabatOutRabenuTam();
    mainScreen.zmanUp->addRoundRectBorder();
    mainScreen.infoUp->addRoundRectBorder();
}
void ColorBottomVLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    bl.mainState.timeSetZrihaShkiaaCochavim();
    bl.mainState.infoSetShabatInShabatOutRabenuTam();
    mainScreen.zmanDown->addRoundRectBorder();
    mainScreen.infoDown->addRoundRectBorder();
}
void ColorBottomVLine::stateLoop()
{
    bl.mainState.stateLoop();
    bl.mainState.timeSetZrihaShkiaaCochavim();
    bl.mainState.infoSetShabatInShabatOutRabenuTam();
    mainScreen.zmanDown->addRoundRectBorder();
    mainScreen.infoDown->addRoundRectBorder();
}
void ColorYomLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    mainScreen.yom->addRoundRectBorder();
}
void ColorYomLine::stateLoop()
{
    bl.mainState.stateLoop();
    mainScreen.yom->addRoundRectBorder();
}
void ColorParashaLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    mainScreen.parashaAll->addRoundRectBorder();
}
void ColorParashaLine::stateLoop()
{
    bl.mainState.stateLoop();
    mainScreen.parashaAll->addRoundRectBorder();
}
void ColorIluyLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    mainScreen.iluyNishmat->addRoundRectBorder();
}
void ColorIluyLine::stateLoop()
{
    bl.mainState.stateLoop();
    mainScreen.iluyNishmat->addRoundRectBorder();
}
 
void SetColorUpLine::showStateScreen() { colorScreen.setColor(this->colorTemp); }
void SetColorUpLine::menuAction() 
{ 
    this->colorTemp = displayPrefs.colorUp;
}
void SetColorUpLine::upAction() 
{
    this->colorTemp += 1;
    this->colorTemp %= 7;
}
void SetColorUpLine::downAction() 
{
    this->colorTemp += 6;
    this->colorTemp %= 7;
}
void SetColorUpLine::okAction() 
{
    displayPrefs.colorUp= this->colorTemp;
    bl.configStates();
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohgcm  ifsgn");
}
void SetColorMiddleLine::showStateScreen() { colorScreen.setColor(this->colorTemp); }
void SetColorMiddleLine::menuAction() 
{ 
    this->colorTemp = displayPrefs.colorMiddle;
}
void SetColorMiddleLine::upAction() 
{
    this->colorTemp += 1;
    this->colorTemp %= 7;
}
void SetColorMiddleLine::downAction() 
{
    this->colorTemp += 6;
    this->colorTemp %= 7;
}
void SetColorMiddleLine::okAction() 
{
    displayPrefs.colorMiddle= this->colorTemp;
    bl.configStates();
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohgcm  ifsgn");
}
void SetColorBottomTLine::showStateScreen() { colorScreen.setColor(this->colorTemp); }
void SetColorBottomTLine::menuAction() 
{ 
    this->colorTemp = displayPrefs.colorBottomT;
}
void SetColorBottomTLine::upAction() 
{
    this->colorTemp += 1;
    this->colorTemp %= 7;
}
void SetColorBottomTLine::downAction() 
{
    this->colorTemp += 6;
    this->colorTemp %= 7;
}
void SetColorBottomTLine::okAction() 
{
    displayPrefs.colorBottomT= this->colorTemp;
    bl.configStates();
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohgcm  ifsgn");
}
void SetColorBottomVLine::showStateScreen() { colorScreen.setColor(this->colorTemp); }
void SetColorBottomVLine::menuAction() 
{ 
    this->colorTemp = displayPrefs.colorBottomV;
}
void SetColorBottomVLine::upAction() 
{
    this->colorTemp += 1;
    this->colorTemp %= 7;
}
void SetColorBottomVLine::downAction() 
{
    this->colorTemp += 6;
    this->colorTemp %= 7;
}
void SetColorBottomVLine::okAction() 
{
    displayPrefs.colorBottomV= this->colorTemp;
    bl.configStates();
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohgcm  ifsgn");
}

void SetColorYomLine::showStateScreen() { colorScreen.setColor(this->colorTemp); }
void SetColorYomLine::menuAction() 
{ 
    this->colorTemp = displayPrefs.colorYom;
}
void SetColorYomLine::upAction() 
{
    this->colorTemp += 1;
    this->colorTemp %= 7;
}
void SetColorYomLine::downAction() 
{
    this->colorTemp += 6;
    this->colorTemp %= 7;
}
void SetColorYomLine::okAction() 
{
    displayPrefs.colorYom= this->colorTemp;
    bl.configStates();
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohgcm  ifsgn");
}

void SetColorParashaLine::showStateScreen() { colorScreen.setColor(this->colorTemp); }
void SetColorParashaLine::menuAction() 
{ 
    this->colorTemp = displayPrefs.colorParasha;
}
void SetColorParashaLine::upAction() 
{
    this->colorTemp += 1;
    this->colorTemp %= 7;
}
void SetColorParashaLine::downAction() 
{
    this->colorTemp += 6;
    this->colorTemp %= 7;
}
void SetColorParashaLine::okAction() 
{
    displayPrefs.colorParasha= this->colorTemp;
    bl.configStates();
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohgcm  ifsgn");
}

void SetColorIluyLine::showStateScreen() { colorScreen.setColor(this->colorTemp); }
void SetColorIluyLine::menuAction() 
{ 
    this->colorTemp = displayPrefs.colorIluy;
}
void SetColorIluyLine::upAction() 
{
    this->colorTemp += 1;
    this->colorTemp %= 7;
}
void SetColorIluyLine::downAction() 
{
    this->colorTemp += 6;
    this->colorTemp %= 7;
}
void SetColorIluyLine::okAction() 
{
    displayPrefs.colorIluy= this->colorTemp;
    bl.configStates();
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohgcm  ifsgn");
}
