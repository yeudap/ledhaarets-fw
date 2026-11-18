#include "States.h"
#include "Logic.h"
#include "DB\Shita.h"
#include "Preferences/DataPrefs.h"

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
    this->shitaTemp = dataPrefs.zmanim.shita;
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
    dataPrefs.zmanim.shita= this->shitaTemp;
    setShita();
}
void ShitaState::setShita() 
{
    shitot[dataPrefs.zmanim.shita]->setDefault();
    bl.configStates();
    bl.mainState.updateDayValues = true;
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
}

