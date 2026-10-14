#include "States.h"
extern DayInfo dayInfo;


void MainState::initTimeSetFuncList()
{
    timeSetFuncList[0] = std::make_pair(&MainState::timeSetTalitAlotZriha, &MainState::flagTimeSetTalitAlotZriha);
    timeSetFuncList[1] = std::make_pair(&MainState::timeSetSozShma, &MainState::flagTimeSetSozShma);
    timeSetFuncList[2] = std::make_pair(&MainState::timeSetSozTfila, &MainState::flagTimeSetSozTfila);
    timeSetFuncList[3] = std::make_pair(&MainState::timeSetHazotMinhaGMinhaK, &MainState::flagTimeSetHazotMinhaGMinhaK);
    timeSetFuncList[4] = std::make_pair(&MainState::timeSetPlagShkiaaCochavim, &MainState::flagTimeSetPlagShkiaaCochavim);
    timeSetFuncList[5] = std::make_pair(&MainState::timeSetHazotAlotZriha, &MainState::flagTimeSetHazotAlotZriha);
    timeSetFuncList[6] = std::make_pair(&MainState::timeSetZrihaShkiaaCochavim, &MainState::flagTimeSetZrihaShkiaaCochavim);
}
void MainState::timeSetZrihaShkiaaCochavim()
{
    // זריחה - שקיעה - צאת הכוכבים
    mainScreen.update(mainScreen.zmanRightUp, strings.zriha, txtZmanT);
    mainScreen.update(mainScreen.zmanRightDown, dayInfo.getZriha(), txtZmanV);
    mainScreen.update(mainScreen.zmanMiddleUp, strings.shkiaa, txtZmanT);
    mainScreen.update(mainScreen.zmanMiddleDown, dayInfo.getShkiaa(), txtZmanV);
    mainScreen.update(mainScreen.zmanLeftUp, strings.tzetCoc, txtZmanT);
    mainScreen.update(mainScreen.zmanLeftDown, dayInfo.getZeetCochavim(), txtZmanV);
}
bool MainState::flagTimeSetZrihaShkiaaCochavim()
{
    if(userPreferences.ZSZ1==1) 
    {
        if(isTimeInRange(zmanPlusDakot(dayInfo.getZeetCochavim(), 31), Zman(23,59))||isTimeInRange(Zman(0,0), Zman(3,0)))            
        return true; 
    }
    if(userPreferences.ZSZ1==2) { return true; }
    if(userPreferences.ZSZ1==0) { return false; }
    return false;
}
void MainState::timeSetTalitAlotZriha()
{
    // עלות השחר - זמן טלית ותפילין - זריחה
    mainScreen.update(mainScreen.zmanRightUp, strings.aHashahar, txtZmanT);
    mainScreen.update(mainScreen.zmanRightDown, dayInfo.alotHashahar, txtZmanV);
    mainScreen.update(mainScreen.zmanMiddleUp, strings.talit, txtZmanT);
    mainScreen.update(mainScreen.zmanMiddleDown, dayInfo.zmanTalit, txtZmanV);
    mainScreen.update(mainScreen.zmanLeftUp, strings.zriha, txtZmanT);
    mainScreen.update(mainScreen.zmanLeftDown, dayInfo.getZriha(), txtZmanV);
}
bool MainState::flagTimeSetTalitAlotZriha()
{
    // שלוש לפנות בוקר עד חצי שעה אחרי זריחה
    if(userPreferences.TAZ==1) 
    {
        if(isTimeInRange(Zman(3,1), zmanPlusDakot(dayInfo.getZriha(), 30)))
        return true; 
    }
    if(userPreferences.TAZ==2) { return true; }
    if(userPreferences.TAZ==0) { return false; }
    return false;
}
void MainState::timeSetSozShma()
{
    // מג"א - סוף זמן קריאת שמע - גר"א
    mainScreen.update(mainScreen.zmanRightUp, strings.mga, txtZmanT);
    mainScreen.update(mainScreen.zmanRightDown, dayInfo.sofZmanShmaMga, txtZmanV);
    mainScreen.update(mainScreen.zmanMiddleUp, strings.sofZman, txtZmanT);
    mainScreen.update(mainScreen.zmanMiddleDown, strings.ks, txtZmanT);
    if(userPreferences.eda==CHABAD) { mainScreen.update(mainScreen.zmanLeftUp, strings.adhaz, txtZmanT); }
    else { mainScreen.update(mainScreen.zmanLeftUp, strings.gra, txtZmanT); }
    mainScreen.update(mainScreen.zmanLeftDown, dayInfo.sofZmanShmaGra, txtZmanV);
}
bool MainState::flagTimeSetSozShma()
{
    // חצי שעה אחרי זריחה עד 5 דקות אחרי סו"ז שמע גר"א
    if(userPreferences.SOZKS==1) 
    {
        if(isTimeInRange(zmanPlusDakot(dayInfo.getZriha(), 31), zmanPlusDakot(dayInfo.sofZmanShmaGra, 5)))
        return true; 
    }
    if(userPreferences.SOZKS==2) { return true; }
    if(userPreferences.SOZKS==0) { return false; }
    return false;
}
void MainState::timeSetSozTfila()
{
    // מג"א - סוף זמן תפילה - גר"א
    mainScreen.update(mainScreen.zmanRightUp, strings.mga, txtZmanT);
    mainScreen.update(mainScreen.zmanRightDown, dayInfo.sofZmanTfilaMga, txtZmanV);
    mainScreen.update(mainScreen.zmanMiddleUp, strings.sofZman, txtZmanT);
    mainScreen.update(mainScreen.zmanMiddleDown, strings.tfila, txtZmanT);
    if(userPreferences.eda==CHABAD) { mainScreen.update(mainScreen.zmanLeftUp, strings.adhaz, txtZmanT); }
    else{ mainScreen.update(mainScreen.zmanLeftUp, strings.gra, txtZmanT); }
    mainScreen.update(mainScreen.zmanLeftDown, dayInfo.sofZmanTfilaGra, txtZmanV);
}
bool MainState::flagTimeSetSozTfila()
{
    // חמש דקות אחרי סו"ז שמע מג"א עד רבע שעה אחרי סו"ז תפילה גר"א
    if(userPreferences.SOZT==1) 
    {
        if(isTimeInRange(zmanPlusDakot(dayInfo.sofZmanShmaGra, 6), zmanPlusDakot(dayInfo.sofZmanTfilaGra, 15)))
        return true; 
    }
    if(userPreferences.SOZT==2) { return true; }
    if(userPreferences.SOZT==0) { return false; }
    return false;
}
void MainState::timeSetHazotMinhaGMinhaK()
{
    // חצות היום - מנחה גדולה - מנחה קטנה
    Text hazot_T = Text(mainScreen.colorBottomT, &ariel10, 20, 13, LEFT);
    Text hazot_V = Text(mainScreen.colorBottomV, &ariel10, 20, 13, LEFT);

    Text gdola_T = Text(mainScreen.colorBottomT, &ariel10, 10, 13, LEFT);
    Text gdola_V = Text(mainScreen.colorBottomV, &ariel10, 22, 13, LEFT);

    Text ktana_T = Text(mainScreen.colorBottomT, &ariel10, 0, 13, LEFT);
    Text ktana_V = Text(mainScreen.colorBottomV, &ariel10, 0, 13, CENTER);

    mainScreen.update(mainScreen.zmanRightUp, strings.hazot, hazot_T);
    mainScreen.update(mainScreen.zmanRightDown, dayInfo.hatsot, hazot_V);
    mainScreen.update(mainScreen.zmanMiddleUp, strings.minhaG, gdola_T);
    mainScreen.update(mainScreen.zmanMiddleDown, dayInfo.minhaGdola, gdola_V);
    mainScreen.update(mainScreen.zmanLeftUp, strings.minhaKtana, ktana_T);
    mainScreen.update(mainScreen.zmanLeftDown, dayInfo.minhaKtana, ktana_V);
}
bool MainState::flagTimeSetHazotMinhaGMinhaK()
{
    // רבע שעה אחרי סו"ז תפילה גר"א עד רבע שעה אחרי מנחה קטנה
    if(userPreferences.HMM==1) 
    {
        if(isTimeInRange(zmanPlusDakot(dayInfo.sofZmanTfilaGra, 16), zmanPlusDakot(dayInfo.minhaKtana, 15)))
        return true; 
    }
    if(userPreferences.HMM==2) { return true; }
    if(userPreferences.HMM==0) { return false; }
    return false;
}
void MainState::timeSetPlagShkiaaCochavim()
{
    // פלג המנחה - שקיעה - צאת הכוכבים
    mainScreen.update(mainScreen.zmanRightUp, strings.plagMinha, txtZmanT);
    mainScreen.update(mainScreen.zmanRightDown, dayInfo.plagHaminha, txtZmanV);
    mainScreen.update(mainScreen.zmanMiddleUp, strings.shkiaa, txtZmanT);
    mainScreen.update(mainScreen.zmanMiddleDown, dayInfo.getShkiaa(), txtZmanV);
    mainScreen.update(mainScreen.zmanLeftUp, strings.tzetCoc, txtZmanT);
    mainScreen.update(mainScreen.zmanLeftDown, dayInfo.getZeetCochavim(), txtZmanV);
}
bool MainState::flagTimeSetPlagShkiaaCochavim()
{
    // רבע שעה אחרי מנחה קטנה עד חצי שעה אחרי צאת הכוכבים
    if(userPreferences.PSZ==1) 
    {
        if(isTimeInRange(zmanPlusDakot(dayInfo.minhaKtana, 16), zmanPlusDakot(dayInfo.getZeetCochavim(), 30)))
        return true; 
    }
    if(userPreferences.PSZ==2) { return true; }
    if(userPreferences.PSZ==0) { return false; }
    return false;
}
void MainState::timeSetHazotAlotZriha()
{
    // חצות - עלות השחר - זריחה
    mainScreen.update(mainScreen.zmanRightUp, strings.hazotLayla , txtZmanT);
    mainScreen.update(mainScreen.zmanRightDown, dayInfo.hazotLayla, txtZmanV);
    mainScreen.update(mainScreen.zmanMiddleUp, strings.aHashahar, txtZmanT);
    mainScreen.update(mainScreen.zmanMiddleDown, dayInfo.tomorrow->alotHashahar, txtZmanV);
    mainScreen.update(mainScreen.zmanLeftUp, strings.zriha, txtZmanT);
    mainScreen.update(mainScreen.zmanLeftDown, dayInfo.tomorrow->getZriha(), txtZmanV);
}
bool MainState::flagTimeSetHazotAlotZriha()
{
    // חצי שעה אחרי צאת הכוכבים עד שלוש בבוקר
    if(userPreferences.HAZ==1) 
    {
        if(isTimeInRange(zmanPlusDakot(dayInfo.getZeetCochavim(), 30), Zman(23,59)) || isTimeInRange(Zman(0,0), Zman(3,0)))
        return true; 
    }
    if(userPreferences.HAZ==2) { return true; }
    if(userPreferences.HAZ==0) { return false; }
    return false;
}