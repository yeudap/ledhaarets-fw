#include "Logic.h"
#include "Preferences/DisplayPrefs.h"

extern Logic bl;


void ColorUpLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    bl.mainState.timeSetZrihaShkiaaCochavim();
    mainScreen.upAll->addRoundRectBorder();
}
void ColorMiddleLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    bl.mainState.timeSetZrihaShkiaaCochavim();
    mainScreen.middleAll->addRoundRectBorder();
}
void ColorBottomTLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    bl.mainState.timeSetZrihaShkiaaCochavim();
    mainScreen.infoUp->addRoundRectBorder();
}
void ColorBottomVLine::showStateScreen() 
{
    bl.mainState.showStateScreen();
    bl.mainState.timeSetZrihaShkiaaCochavim();
    mainScreen.infoDown->addRoundRectBorder();
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
