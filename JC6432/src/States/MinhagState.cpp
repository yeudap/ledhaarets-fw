#include "Logic.h"
#include "DB\Shita.h"

extern Shita* shitot[numOfShitot];
extern Logic bl;
extern DayInfo dayInfo;

void nerotStr(char* buffer, uint8_t dakot)
{
    sprintf(buffer, "%s %2d %s", strings.dakot, dakot, strings.hadlakatNerotMukdemet);
}
void shanimStr(char* buffer, uint8_t plusShana)
{
    int shanim = dayInfo.loaziDate.year-69 + plusShana;
    sprintf(buffer, "%d %s %s", shanim, ":", strings.shanimLahurban);
}
void MinhagShitaState::showStateScreen()
{
    menuScreen.printMenu(strings.menuMinag, strings.hazegColZmaneiHayom, strings.shitaTitle, strings.zriha); 
}
void MinhagZrihaState::showStateScreen() 
{ 
    menuScreen.printMenu(strings.menuMinag, strings.shitaTitle, strings.zriha, strings.shkiaa); 
}
void MinhagShkiaaState::showStateScreen() 
{ 
    menuScreen.printMenu(strings.menuMinag, strings.zriha, strings.shkiaa, strings.zeetHacochavim); 
}
void ZeetCochavimState::showStateScreen() 
{ 
    menuScreen.printMenu(strings.menuMinag, strings.shkiaa, strings.zeetHacochavim, strings.rabenuTam); 
}
void RabenuTamState::showStateScreen() 
{ 
    menuScreen.printMenu(strings.menuMinag, strings.zeetHacochavim, strings.rabenuTam, strings.alotHashahar); 
}
void AlotState::showStateScreen() 
{ 
    menuScreen.printMenu(strings.menuMinag, strings.rabenuTam, strings.alotHashahar, strings.zmanTalitUtfilin); 
}
void TalitState::showStateScreen() 
{ 
    menuScreen.printMenu(strings.menuMinag, strings.alotHashahar, strings.zmanTalitUtfilin, strings.sozksMga); 
}
void SozksMgaState::showStateScreen() 
{ 
    menuScreen.printMenu(strings.menuMinag, strings.zmanTalitUtfilin, strings.sozksMga, strings.soztMga); 
}
void SoztMgaState::showStateScreen() 
{ 
    menuScreen.printMenu(strings.menuMinag, strings.sozksMga, strings.soztMga, strings.minhaGdola); 
}
void MinhaGState::showStateScreen() 
{ 
    menuScreen.printMenu(strings.menuMinag, strings.soztMga, strings.minhaGdola, strings.plagHaminha); 
}
void PlagState::showStateScreen() 
{ 
    menuScreen.printMenu(strings.menuMinag, strings.minhaGdola, strings.plagHaminha, strings.zeetShabat); 
}
void ZeetShabatState::showStateScreen() 
{ 
    menuScreen.printMenu(strings.menuMinag, strings.plagHaminha, strings.zeetShabat, strings.zeetZomDerabanan); 
}
void ZeetZomState::showStateScreen() 
{ 
    char shanim[50];
    shanimStr(shanim, userPreferences.shanim);
    menuScreen.printMenu(strings.menuMinag, strings.zeetShabat, strings.zeetZomDerabanan, shanim); 
}
void MinhagShanimState::showStateScreen() 
{ 
    char shanim[50];
    shanimStr(shanim, userPreferences.shanim);
    char nerot[50];
    nerotStr(nerot, userPreferences.nerot);
    menuScreen.printMenu(strings.menuMinag, strings.zeetZomDerabanan, shanim, nerot); 
}
void MinhagNerotState::showStateScreen() 
{ 
    char shanim[50];
    shanimStr(shanim, userPreferences.shanim);
    char nerot[50];
    nerotStr(nerot, userPreferences.nerot);
    menuScreen.printMenu(strings.menuMinag, shanim, nerot, strings.hazegColZmaneiHayom); 
}
void ShowTimeState::showStateScreen() 
{ 
    char nerot[50];
    nerotStr(nerot, userPreferences.nerot);
    menuScreen.printMenu(strings.menuMinag, nerot, strings.hazegColZmaneiHayom, strings.shitaTitle); 
}

// Show time
Zman getZman(uint8_t i)
{
    if(i == 1) { return dayInfo.alotHashahar; }
    if(i == 2) { return dayInfo.zmanTalit; }
    if(i == 3) { return dayInfo.sofZmanShmaMga; }
    if(i == 4) { return dayInfo.sofZmanShmaGra; }
    if(i == 5) { return dayInfo.sofZmanTfilaMga; }
    if(i == 6) { return dayInfo.sofZmanTfilaGra; }
    if(i == 7) { return dayInfo.hatsot; }
    if(i == 8) { return dayInfo.minhaGdola; }
    if(i == 9) { return dayInfo.minhaKtana; }
    if(i == 10) { return dayInfo.plagHaminha; }
    if(i == 11) { return dayInfo.tzetHacochavim; }
    if(i == 12) { return dayInfo.rabenuTam; }
    return Zman(0);
}
void ShowTime::showStateScreen() 
{ 
    uint8_t minus = showTemp;
    minus += numOfZmanim-2;
    minus %= numOfZmanim;
    minus++;
    uint8_t plus = showTemp;
    plus %= numOfZmanim;
    plus++;
    menuScreen.printMenu(strings.zmaneHayom, zmanim[minus], zmanim[showTemp], zmanim[plus]); 
    menuScreen.addZman(getZman(showTemp), false);
}
void ShowTime::menuAction() 
{
    this->showTemp = 1;
}
void ShowTime::upAction() 
{
    this->showTemp += numOfZmanim-2;
    this->showTemp %= numOfZmanim;
    this->showTemp++;
}
void ShowTime::downAction() 
{
    this->showTemp %= numOfZmanim;
    this->showTemp++;
}
void ShowTime::okAction() 
{
    this->showTemp = 1;
}

// zriha
Zman getZriha(uint8_t i)
{
    if(i == 1) { return dayInfo.zrihaM; }
    if(i == 2) 
    { 
        if(userPreferences.shita ==1 && userPreferences.ofek > 4) // kiseRahamim
        {
            return dayInfo.zrihaN + 2.5;
        }
        return dayInfo.zrihaN; }
    if(i == 3) { return dayInfo.zrihaA; }
    return dayInfo.zrihaM;
}
void SetZrihaState::showStateScreen() 
{ 
    uint8_t minus = zrihaTemp;
    minus += numOfZS-2;
    minus %= numOfZS;
    minus++;
    uint8_t plus = zrihaTemp;
    plus %= numOfZS;
    plus++;
    menuScreen.printMenu(strings.zriha, zsMethod[minus], zsMethod[zrihaTemp], zsMethod[plus]); 
    menuScreen.addZman(getZriha(zrihaTemp));
}
void SetZrihaState::menuAction() 
{ 
    this->zrihaTemp = userPreferences.zriha;
}
void SetZrihaState::upAction() 
{
    this->zrihaTemp += numOfZS-2;
    this->zrihaTemp %= numOfZS;
    this->zrihaTemp++;
}
void SetZrihaState::downAction() 
{
    this->zrihaTemp %= numOfZS;
    this->zrihaTemp++;
}
void SetZrihaState::okAction() 
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
    userPreferences.zriha= this->zrihaTemp;
    bl.configStates();
    bl.mainState.updateDayValues = true;
}

// shkiaa
Zman getShkiaa(uint8_t i)
{
    if(i == 1) { return dayInfo.shkiaaM; }
    if(i == 2) { return dayInfo.shkiaaN; }
    if(i == 3) { return dayInfo.shkiaaA; }
    return dayInfo.shkiaaM;
}
void SetShkiaaState::showStateScreen() 
{ 
    uint8_t minus = shkiaaTemp;
    minus += numOfZS-2;
    minus %= numOfZS;
    minus++;
    uint8_t plus = shkiaaTemp;
    plus %= numOfZS;
    plus++;
    menuScreen.printMenu(strings.shkiaa, zsMethod[minus], zsMethod[shkiaaTemp], zsMethod[plus]); 
    menuScreen.addZman(getShkiaa(shkiaaTemp));
}
void SetShkiaaState::menuAction() 
{ 
    this->shkiaaTemp = userPreferences.shkiaa;
}
void SetShkiaaState::upAction() 
{
    this->shkiaaTemp += numOfZS-2;
    this->shkiaaTemp %= numOfZS;
    this->shkiaaTemp++;
}
void SetShkiaaState::downAction() 
{
    this->shkiaaTemp %= numOfZS;
    this->shkiaaTemp++;
}
void SetShkiaaState::okAction() 
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
    userPreferences.shkiaa= this->shkiaaTemp;
    bl.configStates();
    bl.mainState.updateDayValues = true;
}

// nerot
void MinhagNerotSetState::showStateScreen() 
{
    char shanim[50];
    shanimStr(shanim, userPreferences.shanim);
    char nerot[50];
    nerotStr(nerot, this->nerotTemp);
    menuScreen.printMenu(strings.menuMinag, shanim, nerot, strings.shitaTitle); 
    menuScreen.nerot->addRoundRect(false, menuScreen.nerot->x, menuScreen.nerot->y+1, menuScreen.nerot->width, menuScreen.nerot->high-1, 3, YELLOW);
    menuScreen.nerot->addArrows();
}
void MinhagNerotSetState::menuAction() 
{ 
    this->nerotTemp = userPreferences.nerot;
}
void MinhagNerotSetState::upAction() 
{
    this->nerotTemp += 1;
    if (this->nerotTemp == 61) { this->nerotTemp = 0; }
}
void MinhagNerotSetState::downAction() 
{
    this->nerotTemp -= 1;
    if (this->nerotTemp == -1) { this->nerotTemp = 60; }
}
void MinhagNerotSetState::okAction() 
{
    userPreferences.nerot= this->nerotTemp;
    bl.configStates();
    bl.mainState.updateDayValues = true;
}

// shanim
void MinhagShanimSetState::showStateScreen() 
{
    char shanim[50];
    shanimStr(shanim, this->shanimTemp);
    char nerot[50];
    nerotStr(nerot, userPreferences.nerot);
    menuScreen.printMenu(strings.menuMinag, strings.zeetHacochavim, shanim, nerot); 
    menuScreen.shanim->addRoundRect(false, menuScreen.shanim->x, menuScreen.shanim->y+1, menuScreen.shanim->width, menuScreen.shanim->high-1, 3, YELLOW);
    menuScreen.shanim->addArrows();
}
void MinhagShanimSetState::menuAction() 
{ 
    this->shanimTemp = userPreferences.shanim;
}
void MinhagShanimSetState::upAction() 
{
    this->shanimTemp += 1;
    this->shanimTemp %= 2;
}
void MinhagShanimSetState::downAction() 
{
    this->shanimTemp += 1;
    this->shanimTemp %= 2;
}
void MinhagShanimSetState::okAction() 
{
    userPreferences.shanim= this->shanimTemp;
    dayInfo.setShanimLahurban();
    bl.mainState.updateDayValues = true;
}

// zeet cochavim
void SetZeetCochavimState::showStateScreen() 
{ 
    uint8_t minus = zeetTemp;
    minus += numOfZeetCochavin-2;
    minus %= numOfZeetCochavin;
    minus++;
    uint8_t plus = zeetTemp;
    plus %= numOfZeetCochavin;
    plus++;
    Zman zman = shitot[userPreferences.shita]->zeetCochavimMethods(zeetTemp, &dayInfo.sun, dayInfo.zrihaM, dayInfo.shkiaaM);
    menuScreen.printMenu(strings.zeetHacochavim, zeetMethod[minus], zeetMethod[zeetTemp], zeetMethod[plus]); 
    menuScreen.addZman(zman);
}
void SetZeetCochavimState::menuAction() 
{ 
    this->zeetTemp = userPreferences.zeetCochavim;
}
void SetZeetCochavimState::upAction() 
{
    this->zeetTemp += numOfZeetCochavin-2;
    this->zeetTemp %= numOfZeetCochavin;
    this->zeetTemp++;
}
void SetZeetCochavimState::downAction() 
{
    this->zeetTemp %= numOfZeetCochavin;
    this->zeetTemp++;
}
void SetZeetCochavimState::okAction() 
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
    userPreferences.zeetCochavim= this->zeetTemp;
    bl.configStates();
    bl.mainState.updateDayValues = true;
}

// rabenu tam
void SetRabenuTamState::showStateScreen() 
{ 
    uint8_t minus = rtTemp;
    minus += numOfZeetCochavin-2;
    minus %= numOfZeetCochavin;
    minus++;
    uint8_t plus = rtTemp;
    plus %= numOfZeetCochavin;
    plus++;
    Zman zman = shitot[userPreferences.shita]->zeetCochavimMethods(rtTemp, &dayInfo.sun, dayInfo.zrihaM, dayInfo.shkiaaM);
    menuScreen.printMenu(strings.rabenuTam, zeetMethod[minus], zeetMethod[rtTemp], zeetMethod[plus]); 
    menuScreen.addZman(zman);
}
void SetRabenuTamState::menuAction() 
{ 
    this->rtTemp = userPreferences.rabenuTam;
}
void SetRabenuTamState::upAction() 
{
    this->rtTemp += numOfZeetCochavin-2;
    this->rtTemp %= numOfZeetCochavin;
    this->rtTemp++;
}
void SetRabenuTamState::downAction() 
{
    this->rtTemp %= numOfZeetCochavin;
    this->rtTemp++;
}
void SetRabenuTamState::okAction() 
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
    userPreferences.rabenuTam= this->rtTemp;
    bl.configStates();
    bl.mainState.updateDayValues = true;
}

// zman talit utfilin
void SetTalitState::showStateScreen() 
{ 
    uint8_t minus = talitTemp;
    minus += numOfTalit-2;
    minus %= numOfTalit;
    minus++;
    uint8_t plus = talitTemp;
    plus %= numOfTalit;
    plus++;
    Zman zman = shitot[userPreferences.shita]->zmanTalitMethods(talitTemp, &dayInfo.sun, dayInfo.alotHashahar, dayInfo.getZriha(), dayInfo.getShkiaa());
    menuScreen.printMenu(strings.zmanTalitUtfilin, talitMethod[minus], talitMethod[talitTemp], talitMethod[plus]); 
    menuScreen.addZman(zman);
}
void SetTalitState::menuAction() 
{ 
    this->talitTemp = userPreferences.zmanTalit;
}
void SetTalitState::upAction() 
{
    this->talitTemp += numOfTalit-2;
    this->talitTemp %= numOfTalit;
    this->talitTemp++;
}
void SetTalitState::downAction() 
{
    this->talitTemp %= numOfTalit;
    this->talitTemp++;
}
void SetTalitState::okAction() 
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
    userPreferences.zmanTalit= this->talitTemp;
    bl.configStates();
    bl.mainState.updateDayValues = true;
}

// alot hashahar
void SetAlotState::showStateScreen() 
{ 
    uint8_t minus = alotTemp;
    minus += numOfAlot-2;
    minus %= numOfAlot;
    minus++;
    uint8_t plus = alotTemp;
    plus %= numOfAlot;
    plus++;
    Zman zman = shitot[userPreferences.shita]->alotMethods(alotTemp, &dayInfo.sun, dayInfo.zrihaM, dayInfo.shkiaaM);
    menuScreen.printMenu(strings.alotHashahar, alotMethod[minus], alotMethod[alotTemp], alotMethod[plus]); 
    menuScreen.addZman(zman);
}
void SetAlotState::menuAction() 
{ 
    this->alotTemp = userPreferences.alotHashahar;
}
void SetAlotState::upAction() 
{
    this->alotTemp += numOfAlot-2;
    this->alotTemp %= numOfAlot;
    this->alotTemp++;
}
void SetAlotState::downAction() 
{
    this->alotTemp %= numOfAlot;
    this->alotTemp++;
}
void SetAlotState::okAction() 
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
    userPreferences.alotHashahar= this->alotTemp;
    bl.configStates();
    bl.mainState.updateDayValues = true;
}

// sof zman kriat shma mga
void SetSozksMgaState::showStateScreen() 
{ 
    uint8_t minus = sozksTemp;
    minus += numOfSozMga-2;
    minus %= numOfSozMga;
    minus++;
    uint8_t plus = sozksTemp;
    plus %= numOfSozMga;
    plus++;
    Zman zman = shitot[userPreferences.shita]->sozksMgaMethods(sozksTemp, &dayInfo.sun, dayInfo.alot90, dayInfo.zeet90, dayInfo.alot72, dayInfo.zeet72, dayInfo.zrihaA, dayInfo.shkiaaA, dayInfo.zrihaN, dayInfo.shkiaaN);
    menuScreen.printMenu(strings.sozksMga, sozMgaMethod[minus], sozMgaMethod[sozksTemp], sozMgaMethod[plus]); 
    menuScreen.addZman(zman);
}
void SetSozksMgaState::menuAction() 
{ 
    this->sozksTemp = userPreferences.sozksMga;
}
void SetSozksMgaState::upAction() 
{
    this->sozksTemp += numOfSozMga-2;
    this->sozksTemp %= numOfSozMga;
    this->sozksTemp++;
}
void SetSozksMgaState::downAction() 
{
    this->sozksTemp %= numOfSozMga;
    this->sozksTemp++;
}
void SetSozksMgaState::okAction() 
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
    userPreferences.sozksMga= this->sozksTemp;
    bl.configStates();
    bl.mainState.updateDayValues = true;
}

// sof zman tfila mga
void SetSoztMgaState::showStateScreen() 
{ 
    uint8_t minus = soztTemp;
    minus += numOfSozMga-2;
    minus %= numOfSozMga;
    minus++;
    uint8_t plus = soztTemp;
    plus %= numOfSozMga;
    plus++;
    Zman zman = shitot[userPreferences.shita]->soztMgaMethods(soztTemp, &dayInfo.sun, dayInfo.alot90, dayInfo.zeet90, dayInfo.alot72, dayInfo.zeet72, dayInfo.zrihaM, dayInfo.shkiaaM, dayInfo.zrihaN, dayInfo.shkiaaN);
    menuScreen.printMenu(strings.soztMga, sozMgaMethod[minus], sozMgaMethod[soztTemp], sozMgaMethod[plus]); 
    menuScreen.addZman(zman);
}
void SetSoztMgaState::menuAction() 
{ 
    this->soztTemp = userPreferences.soztMga;
}
void SetSoztMgaState::upAction() 
{
    this->soztTemp += numOfSozMga-2;
    this->soztTemp %= numOfSozMga;
    this->soztTemp++;
}
void SetSoztMgaState::downAction() 
{
    this->soztTemp %= numOfSozMga;
    this->soztTemp++;
}
void SetSoztMgaState::okAction() 
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
    userPreferences.soztMga= this->soztTemp;
    bl.configStates();
    bl.mainState.updateDayValues = true;
}

// minha gdola
void SetMinhaGState::showStateScreen() 
{ 
    uint8_t minus = minhaTemp;
    minus += numOfMinhaG-2;
    minus %= numOfMinhaG;
    minus++;
    uint8_t plus = minhaTemp;
    plus %= numOfMinhaG;
    plus++;
    Zman zman = shitot[userPreferences.shita]->minhaGMethods(minhaTemp, &dayInfo.sun, dayInfo.hatsot, dayInfo.zrihaM, dayInfo.shkiaaM);
    menuScreen.printMenu(strings.minhaGdola, minhaGMethod[minus], minhaGMethod[minhaTemp], minhaGMethod[plus]); 
    menuScreen.addZman(zman);
}
void SetMinhaGState::menuAction() 
{ 
    this->minhaTemp = userPreferences.minhaG;
}
void SetMinhaGState::upAction() 
{
    this->minhaTemp += numOfMinhaG-2;
    this->minhaTemp %= numOfMinhaG;
    this->minhaTemp++;
}
void SetMinhaGState::downAction() 
{
    this->minhaTemp %= numOfMinhaG;
    this->minhaTemp++;
}
void SetMinhaGState::okAction() 
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
    userPreferences.minhaG= this->minhaTemp;
    bl.configStates();
    bl.mainState.updateDayValues = true;
}

// plag haminha
void SetPlagState::showStateScreen() 
{ 
    uint8_t minus = plagTemp;
    minus += numOfPlag-2;
    minus %= numOfPlag;
    minus++;
    uint8_t plus = plagTemp;
    plus %= numOfPlag;
    plus++;
    Zman zman = shitot[userPreferences.shita]->plagMethods(plagTemp, &dayInfo.sun, dayInfo.zrihaM, dayInfo.shkiaaM, dayInfo.shkiaaM);
    menuScreen.printMenu(strings.plagHaminha, plagMethod[minus], plagMethod[plagTemp], plagMethod[plus]); 
    menuScreen.addZman(zman);
}
void SetPlagState::menuAction() 
{ 
    this->plagTemp = userPreferences.plagMinha;
}
void SetPlagState::upAction() 
{
    this->plagTemp += numOfPlag-2;
    this->plagTemp %= numOfPlag;
    this->plagTemp++;
}
void SetPlagState::downAction() 
{
    this->plagTemp %= numOfPlag;
    this->plagTemp++;
}
void SetPlagState::okAction() 
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
    userPreferences.plagMinha= this->plagTemp;
    bl.configStates();
    bl.mainState.updateDayValues = true;
}

// zeet shabat
void SetZeetShabatState::showStateScreen() 
{ 
    uint8_t minus = zeetTemp;
    minus += numOfZeetCochavin-2;
    minus %= numOfZeetCochavin;
    minus++;
    uint8_t plus = zeetTemp;
    plus %= numOfZeetCochavin;
    plus++;
    Zman zman = shitot[userPreferences.shita]->zeetCochavimMethods(zeetTemp, &dayInfo.sun, dayInfo.zrihaM, dayInfo.shkiaaM);
    menuScreen.printMenu(strings.zeetShabat, zeetMethod[minus], zeetMethod[zeetTemp], zeetMethod[plus]); 
    menuScreen.addZman(zman);
}
void SetZeetShabatState::menuAction() 
{ 
    this->zeetTemp = userPreferences.zeetShabat;
}
void SetZeetShabatState::upAction() 
{
    this->zeetTemp += numOfZeetCochavin-2;
    this->zeetTemp %= numOfZeetCochavin;
    this->zeetTemp++;
}
void SetZeetShabatState::downAction() 
{
    this->zeetTemp %= numOfZeetCochavin;
    this->zeetTemp++;
}
void SetZeetShabatState::okAction() 
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
    userPreferences.zeetShabat= this->zeetTemp;
    bl.configStates();
    bl.mainState.updateDayValues = true;
}

// zeet zom derabanan
void SetZeetZomState::showStateScreen() 
{ 
    uint8_t minus = zeetTemp;
    minus += numOfZeetCochavin-2;
    minus %= numOfZeetCochavin;
    minus++;
    uint8_t plus = zeetTemp;
    plus %= numOfZeetCochavin;
    plus++;
    Zman zman = shitot[userPreferences.shita]->zeetCochavimMethods(zeetTemp, &dayInfo.sun, dayInfo.zrihaM, dayInfo.shkiaaM);
    menuScreen.printMenu(strings.zeetZomDerabanan, zeetMethod[minus], zeetMethod[zeetTemp], zeetMethod[plus]); 
    menuScreen.addZman(zman);
}
void SetZeetZomState::menuAction() 
{ 
    this->zeetTemp = userPreferences.zeetZom;
}
void SetZeetZomState::upAction() 
{
    this->zeetTemp += numOfZeetCochavin-2;
    this->zeetTemp %= numOfZeetCochavin;
    this->zeetTemp++;
}
void SetZeetZomState::downAction() 
{
    this->zeetTemp %= numOfZeetCochavin;
    this->zeetTemp++;
}
void SetZeetZomState::okAction() 
{
    messageScreen.printMessages(messageScreen.SUCCESS, "! gmuc", "...ohbnz  ifsgn");
    userPreferences.zeetZom= this->zeetTemp;
    bl.configStates();
    bl.mainState.updateDayValues = true;
}