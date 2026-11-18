#include "Logic.h"
#include "Preferences/DisplayPrefs.h"

extern Logic bl;

void ColorUpLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    mainScreen.infoAll->addRoundRectBorder();
}
void ColorUpLine::stateLoop() 
{ 
    bl.mainState.stateLoop(); 
    mainScreen.infoAll->addRoundRectBorder();
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
    mainScreen.zmanUp->addRoundRectBorder();
}
void ColorBottomTLine::stateLoop()
{
    bl.mainState.stateLoop();
    mainScreen.zmanUp->addRoundRectBorder();
}
void ColorBottomVLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    mainScreen.zmanDown->addRoundRectBorder();
}
void ColorBottomVLine::stateLoop()
{
    bl.mainState.stateLoop();
    mainScreen.zmanDown->addRoundRectBorder();
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
void ColorIvriLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    mainScreen.ivriDate->addRoundRectBorder();
}
void ColorIvriLine::stateLoop()
{
    bl.mainState.stateLoop();
    mainScreen.ivriDate->addRoundRectBorder();
}
void ColorLoaziLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    mainScreen.loaziDate->addRoundRectBorder();
}
void ColorLoaziLine::stateLoop()
{
    bl.mainState.stateLoop();
    mainScreen.loaziDate->addRoundRectBorder();
}
void ColorParashaLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    mainScreen.parashaTitle->addRoundRectBorder();
}
void ColorParashaLine::stateLoop()
{
    bl.mainState.stateLoop();
    mainScreen.parashaTitle->addRoundRectBorder();
}
void ColorParashaVLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    mainScreen.parashaValue->addRoundRectBorder();
}
void ColorParashaVLine::stateLoop()
{
    bl.mainState.stateLoop();
    mainScreen.parashaValue->addRoundRectBorder();
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

void SetColorIvriLine::showStateScreen() { colorScreen.setColor(this->colorTemp); }
void SetColorIvriLine::menuAction() 
{ 
    this->colorTemp = displayPrefs.colorIvri;
}
void SetColorIvriLine::upAction() 
{
    this->colorTemp += 1;
    this->colorTemp %= 7;
}
void SetColorIvriLine::downAction() 
{
    this->colorTemp += 6;
    this->colorTemp %= 7;
}
void SetColorIvriLine::okAction() 
{
    displayPrefs.colorIvri= this->colorTemp;
    bl.configStates();
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohgcm  ifsgn");
}

void SetColorLoaziLine::showStateScreen() { colorScreen.setColor(this->colorTemp); }
void SetColorLoaziLine::menuAction() 
{ 
    this->colorTemp = displayPrefs.colorLoazi;
}
void SetColorLoaziLine::upAction() 
{
    this->colorTemp += 1;
    this->colorTemp %= 7;
}
void SetColorLoaziLine::downAction() 
{
    this->colorTemp += 6;
    this->colorTemp %= 7;
}
void SetColorLoaziLine::okAction() 
{
    displayPrefs.colorLoazi= this->colorTemp;
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

void SetColorParashaVLine::showStateScreen() { colorScreen.setColor(this->colorTemp); }
void SetColorParashaVLine::menuAction() 
{ 
    this->colorTemp = displayPrefs.colorParashaV;
}
void SetColorParashaVLine::upAction() 
{
    this->colorTemp += 1;
    this->colorTemp %= 7;
}
void SetColorParashaVLine::downAction() 
{
    this->colorTemp += 6;
    this->colorTemp %= 7;
}
void SetColorParashaVLine::okAction() 
{
    displayPrefs.colorParashaV = this->colorTemp;
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
