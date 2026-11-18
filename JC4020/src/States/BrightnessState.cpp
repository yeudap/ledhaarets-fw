#include "States.h"
#include "Preferences/DisplayPrefs.h"


void SetBrightnessState::showStateScreen() { brightnessScreen.setBrightness(this->brightTemp); }
void SetBrightnessState::menuAction() 
{ 
    this->brightTemp = displayPrefs.brightness;
    display->setBrightness8(this->brightTemp); 
}
void SetBrightnessState::upAction() 
{
    this->brightTemp += 20;
    if (this->brightTemp == 240)
    {
        this->brightTemp = 220;
    }
    display->setBrightness8(this->brightTemp);
}
void SetBrightnessState::downAction() 
{
    this->brightTemp -= 20;
    if (this->brightTemp == 0)
    {
      this->brightTemp = 20;
    }
    display->setBrightness8(this->brightTemp);
}
void SetBrightnessState::okAction() 
{
    display->setBrightness8(this->brightTemp);
    displayPrefs.brightness= this->brightTemp;
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohhubha  ifsgn");
}