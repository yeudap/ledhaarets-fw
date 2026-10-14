#include "Logic.h"

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
    this->colorTemp = userPreferences.colorUp;
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
    userPreferences.colorUp= this->colorTemp;
    bl.configStates();
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohgcm  ifsgn");
}
void SetColorMiddleLine::showStateScreen() { colorScreen.setColor(this->colorTemp); }
void SetColorMiddleLine::menuAction() 
{ 
    this->colorTemp = userPreferences.colorMiddle;
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
    userPreferences.colorMiddle= this->colorTemp;
    bl.configStates();
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohgcm  ifsgn");
}
void SetColorBottomTLine::showStateScreen() { colorScreen.setColor(this->colorTemp); }
void SetColorBottomTLine::menuAction() 
{ 
    this->colorTemp = userPreferences.colorBottomT;
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
    userPreferences.colorBottomT= this->colorTemp;
    bl.configStates();
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohgcm  ifsgn");
}
void SetColorBottomVLine::showStateScreen() { colorScreen.setColor(this->colorTemp); }
void SetColorBottomVLine::menuAction() 
{ 
    this->colorTemp = userPreferences.colorBottomV;
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
    userPreferences.colorBottomV= this->colorTemp;
    bl.configStates();
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohgcm  ifsgn");
}
