#include "States.h"
#include "Logic.h"
#include "DB\Shita.h"

extern UserPreferences userPreferences;
extern Logic bl;
extern Shita* shitot[numOfShitot];

void ShitaState::showStateScreen() 
{ 
    uint8_t minus = shitaTemp+numOfShitot-1;
    minus %= numOfShitot;
    uint8_t plus = shitaTemp+1;
    plus %= numOfShitot;
    menuScreen.printMenu(strings.shitaTitle, shitot[minus]->name, shitot[shitaTemp]->name, shitot[plus]->name); 
}
void ShitaState::menuAction() 
{ 
    this->shitaTemp = userPreferences.shita;
}
void ShitaState::upAction() 
{
    this->shitaTemp+=(numOfShitot-1);
    this->shitaTemp %= numOfShitot;
}
void ShitaState::downAction() 
{
    this->shitaTemp++;
    this->shitaTemp %= numOfShitot;
}
void ShitaState::okAction() 
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
    userPreferences.shita= this->shitaTemp;
    shitot[this->shitaTemp]->setDefault();
    bl.configStates();
    bl.mainState.updateDayValues = true;
}
