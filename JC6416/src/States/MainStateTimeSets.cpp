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
    timeSetFuncList[7] = std::make_pair(&MainState::infoSetLoazi, &MainState::flagInfoSetLoazi);
}
void MainState::timeSetZrihaShkiaaCochavim()
{
    // זריחה - שקיעה - צאת הכוכבים
    mainScreen.update(mainScreen.zmanRightUp, strings.zriha, txtTitle6);
    mainScreen.update(mainScreen.zmanRightDown, dayInfo.getZriha(), txtZman);
    mainScreen.update(mainScreen.zmanMiddleUp, strings.shkiaa, txtTitle6);
    mainScreen.update(mainScreen.zmanMiddleDown, dayInfo.getShkiaa(), txtZman);
    mainScreen.update(mainScreen.zmanLeftUp, strings.tzetCoc, txtTitle6);
    mainScreen.update(mainScreen.zmanLeftDown, dayInfo.getZeetCochavim(), txtZman);
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
    mainScreen.update(mainScreen.zmanRightUp, strings.aHashahar, txtTitle6);
    mainScreen.update(mainScreen.zmanRightDown, dayInfo.alotHashahar, txtZman);
    mainScreen.update(mainScreen.zmanMiddleUp, strings.talit, txtTitle6);
    mainScreen.update(mainScreen.zmanMiddleDown, dayInfo.zmanTalit, txtZman);
    mainScreen.update(mainScreen.zmanLeftUp, strings.zriha, txtTitle6);
    mainScreen.update(mainScreen.zmanLeftDown, dayInfo.getZriha(), txtZman);
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
    mainScreen.update(mainScreen.zmanRightUp, strings.mga, txtTitle6);
    mainScreen.update(mainScreen.zmanRightDown, dayInfo.sofZmanShmaMga, txtZman);
    mainScreen.update(mainScreen.zmanMiddleUp, strings.sofZman, txtTitle5);
    mainScreen.update(mainScreen.zmanMiddleDown, strings.kriatSma, txtValue5);
    if(userPreferences.eda==CHABAD) { mainScreen.update(mainScreen.zmanLeftUp, strings.adhaz, txtTitle6); }
    else { mainScreen.update(mainScreen.zmanLeftUp, strings.gra, txtTitle6); }
    mainScreen.update(mainScreen.zmanLeftDown, dayInfo.sofZmanShmaGra, txtZman);
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
    mainScreen.update(mainScreen.zmanRightUp, strings.mga, txtTitle6);
    mainScreen.update(mainScreen.zmanRightDown, dayInfo.sofZmanTfilaMga, txtZman);
    mainScreen.update(mainScreen.zmanMiddleUp, strings.sofZman, txtTitle5);
    mainScreen.update(mainScreen.zmanMiddleDown, strings.tfila, txtValue5);
    if(userPreferences.eda==CHABAD) { mainScreen.update(mainScreen.zmanLeftUp, strings.adhaz, txtTitle6); }
    else{ mainScreen.update(mainScreen.zmanLeftUp, strings.gra, txtTitle6); }
    mainScreen.update(mainScreen.zmanLeftDown, dayInfo.sofZmanTfilaGra, txtZman);
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
    mainScreen.update(mainScreen.zmanRightUp, strings.hazot, txtTitle6);
    mainScreen.update(mainScreen.zmanRightDown, dayInfo.hatsot, txtZman);
    mainScreen.update(mainScreen.zmanMiddleUp, strings.minhaG, txtTitle6);
    mainScreen.update(mainScreen.zmanMiddleDown, dayInfo.minhaGdola, txtZman);
    mainScreen.update(mainScreen.zmanLeftUp, strings.minhaKtana, txtTitle6);
    mainScreen.update(mainScreen.zmanLeftDown, dayInfo.minhaKtana, txtZman);
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
    mainScreen.update(mainScreen.zmanRightUp, strings.plagMinha, txtTitle6);
    mainScreen.update(mainScreen.zmanRightDown, dayInfo.plagHaminha, txtZman);
    mainScreen.update(mainScreen.zmanMiddleUp, strings.shkiaa, txtTitle6);
    mainScreen.update(mainScreen.zmanMiddleDown, dayInfo.getShkiaa(), txtZman);
    mainScreen.update(mainScreen.zmanLeftUp, strings.tzetCoc, txtTitle6);
    mainScreen.update(mainScreen.zmanLeftDown, dayInfo.getZeetCochavim(), txtZman);
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
    mainScreen.update(mainScreen.zmanRightUp, strings.hazotLayla , txtTitle6);
    mainScreen.update(mainScreen.zmanRightDown, dayInfo.hazotLayla, txtZman);
    mainScreen.update(mainScreen.zmanMiddleUp, strings.aHashahar, txtTitle6);
    mainScreen.update(mainScreen.zmanMiddleDown, dayInfo.tomorrow->alotHashahar, txtZman);
    mainScreen.update(mainScreen.zmanLeftUp, strings.zriha, txtTitle6);
    mainScreen.update(mainScreen.zmanLeftDown, dayInfo.tomorrow->getZriha(), txtZman);
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
// תאריך לועזי
void MainState::infoSetLoazi()
{
    char date[50];
    dayInfo.loaziDate.toString(date, userPreferences.dateFormat);
    Text txt = Text(mainScreen.colorBottomT, &FreeSansBold9pt7b, 1, 19, CENTER);
    mainScreen.update(mainScreen.zmanAll, date, txt);
}
bool MainState::flagInfoSetLoazi()
{
    if(userPreferences.loazi==1) { return true; }
    return false;
}