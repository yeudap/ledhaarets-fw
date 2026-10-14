#include "Logic.h"

extern Logic bl;
extern GFXfont* font_list[numOfFonts];

void SetFontState::showStateScreen() 
{
    mainScreen.setScreen(userPreferences.screen);
    mainScreen.printScreen();
    if(this->fontTemp == 0) { bl.mainState.morphing = true; }
    else { bl.mainState.morphing = false; }
    bl.mainState.updateClockOnScreen();
    mainScreen.clockAll->addArrows();
}
void SetFontState::stateLoop()
{
    bl.mainState.checkTimeChanged();
}
void SetFontState::menuAction() 
{ 
    this->fontTemp = userPreferences.clockFont;
}
void SetFontState::upAction() 
{
    this->fontTemp += 1;
    this->fontTemp %= numOfFonts;
    mainScreen.clockFont = font_list[this->fontTemp];
}
void SetFontState::downAction() 
{
    this->fontTemp += numOfFonts-1;
    this->fontTemp %= numOfFonts;
    mainScreen.clockFont = font_list[this->fontTemp];
}
void SetFontState::okAction() 
{
    if(this->fontTemp == 0) { bl.mainState.morphing = true; }
    else { bl.mainState.morphing = false; }
    userPreferences.clockFont= this->fontTemp;
    mainScreen.clockFont = font_list[userPreferences.clockFont];
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...vdumw  ifsgn");
}
