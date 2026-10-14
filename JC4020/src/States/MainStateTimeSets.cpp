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
    timeSetFuncList[7] = std::make_pair(&MainState::timeSetZrihaYomShkiaa, &MainState::flagTimeSetZrihaYomShkiaa);
    timeSetFuncList[8] = std::make_pair(&MainState::timeSetZrihaParashaShkiaa, &MainState::flagTimeSetZrihaParashaShkiaa);
    timeSetFuncList[9] = std::make_pair(&MainState::timeSetShabatInParashaShabatOut, &MainState::flagTimeSetShabatInParashaShabatOut);
    timeSetFuncList[10] = std::make_pair(&MainState::infoSetShabatInShabatOutRabenuTam, &MainState::flagInfoSetShabatInShabatOutRabenuTam);
    
    timeSetFuncList[11] = std::make_pair(&MainState::infoSetHagInHagOutRabenuTam, &MainState::flagInfoSetHagInHagOutRabenuTam);
    timeSetFuncList[12] = std::make_pair(&MainState::infoSetRegularDay, &MainState::flagInfoSetRegularDay);
    timeSetFuncList[13] = std::make_pair(&MainState::infoSetRoshHodesh, &MainState::flagInfoSetRoshHodesh);
    timeSetFuncList[14] = std::make_pair(&MainState::infoSetShabatPlus, &MainState::flagInfoSetShabatPlus);
    timeSetFuncList[15] = std::make_pair(&MainState::infoSetNextRoshHodesh, &MainState::flagInfoSetNextRoshHodesh);
    timeSetFuncList[16] = std::make_pair(&MainState::infoSetMolad, &MainState::flagInfoSetMolad);
    timeSetFuncList[17] = std::make_pair(&MainState::infoSetHagYaaleVeyavoHalel, &MainState::flagInfoSetHagYaaleVeyavoHalel);
    timeSetFuncList[18] = std::make_pair(&MainState::infoSetErevHagEruv, &MainState::flagInfoSetErevHagEruv);
    timeSetFuncList[19] = std::make_pair(&MainState::infoSetHametz, &MainState::flagInfoSetHametz);
    timeSetFuncList[20] = std::make_pair(&MainState::infoSetOmer, &MainState::flagInfoSetOmer);
    timeSetFuncList[21] = std::make_pair(&MainState::infoSetHanuka, &MainState::flagInfoSetHanuka);
    timeSetFuncList[22] = std::make_pair(&MainState::infoSetZom, &MainState::flagInfoSetZom);
    timeSetFuncList[23] = std::make_pair(&MainState::infoSetTishaBeav, &MainState::flagInfoSetTishaBeav);
    timeSetFuncList[24] = std::make_pair(&MainState::infoSetIsraeli, &MainState::flagInfoSetIsraeli);
    timeSetFuncList[25] = std::make_pair(&MainState::infoSetAseretYemeTshuva, &MainState::flagInfoSetAseretYemeTshuva);
    timeSetFuncList[26] = std::make_pair(&MainState::infoSetPurim, &MainState::flagInfoSetPurim);
    timeSetFuncList[27] = std::make_pair(&MainState::infoSetYom, &MainState::flagInfoSetYom);
    timeSetFuncList[28] = std::make_pair(&MainState::infoSetParasha, &MainState::flagInfoSetParasha);
    timeSetFuncList[29] = std::make_pair(&MainState::infoSetHaftara, &MainState::flagInfoSetHaftara);
    timeSetFuncList[30] = std::make_pair(&MainState::infoSetIvri, &MainState::flagInfoSetIvri);
    timeSetFuncList[31] = std::make_pair(&MainState::infoSetYerushalmi, &MainState::flagInfoSetYerushalmi);
    timeSetFuncList[32] = std::make_pair(&MainState::infoSetChabad, &MainState::flagInfoSetChabad);
}

void MainState::timeSetZrihaYomShkiaa()
{
    // זריחה - יום - שקיעה
    char dayStr[20];
    dayInfo.getDayInWeek(dayStr, false);
    Text txt = Text(mainScreen.colorBottomT, &font5, 15, 7, CENTER);
    if(userPreferences.leil==1 && dayInfo.afterShkiaa) 
    {
        Serial.println("if(userPreferences.leil==1 && dayInfo.afterShkiaa)");
        Serial.println(dayStr);
        Serial.println(daysOfTheWeek[3]);
        if(strcmp(dayStr, daysOfTheWeek[3]) == 0)
        {
            Serial.println("if(dayStr==daysOfTheWeek[3])");
            txt.align=RIGHT;
        }
        txt.marginY=9;
    }
    mainScreen.update(mainScreen.zmanRightUpS, strings.zriha, txtTitle6);
    mainScreen.update(mainScreen.zmanRightDownS, dayInfo.getZriha(), txtZman);
    char yomOrLeil[5];
    dayInfo.getYomOrLeil(yomOrLeil);
    mainScreen.update(mainScreen.zmanMiddleUpS, yomOrLeil, txt);
    txt = Text(mainScreen.colorBottomV, &ariel10, 0, 9, CENTER);
    mainScreen.update(mainScreen.zmanMiddleDownS, dayStr, txt);
    mainScreen.update(mainScreen.zmanLeftUpS, strings.shkiaa, txtTitle6);
    mainScreen.update(mainScreen.zmanLeftDownS, dayInfo.getShkiaa(), txtZman);
}
bool MainState::flagTimeSetZrihaYomShkiaa()
{
    if(userPreferences.ZYS==0) { return false; }
    if(userPreferences.ZYS==1) { return dayInfo.isYomHol(); }
    if(userPreferences.ZYS==2) { return !dayInfo.isYomHol(); }
    if(userPreferences.ZYS==3) { return true; }
    return false;
}
void MainState::timeSetZrihaParashaShkiaa()
{
    // זריחה - פרשת השבוע - שקיעה
    char parasha[50];
    dayInfo.getParasha(parasha);
    Text txt = Text(mainScreen.colorBottomV, &ariel10, 0, 9, CENTER);
    bool fit10 = mainScreen.zmanMiddleDownS->textFits(parasha, txt);
    if(!fit10)
    {
        txt = Text(mainScreen.colorBottomV, &font7, 0, 9, CENTER);
        bool fit7 = mainScreen.zmanMiddleDownS->textFits(parasha, txt);
        if(!fit7)
        {
            txt = Text(mainScreen.colorBottomV, &font6, 0, 9, CENTER);
        }
    }
    Text txtTitle = Text(mainScreen.colorBottomT, &font5, 0, 8, CENTER);
    if(fit10){txtTitle.marginY=7;}

    mainScreen.update(mainScreen.zmanRightUpS, strings.zriha, txtTitle6);
    mainScreen.update(mainScreen.zmanRightDownS, dayInfo.getZriha(), txtZman);
    mainScreen.update(mainScreen.zmanMiddleUpS, strings.parashatShavua, txtTitle);
    mainScreen.update(mainScreen.zmanMiddleDownS, parasha, txt);
    mainScreen.update(mainScreen.zmanLeftUpS, strings.shkiaa, txtTitle6);
    mainScreen.update(mainScreen.zmanLeftDownS, dayInfo.getShkiaa(), txtZman);
}
bool MainState::flagTimeSetZrihaParashaShkiaa()
{
    if(userPreferences.ZPS==0) { return false; }
    if(userPreferences.ZPS==1) { return dayInfo.isYomHol(); }
    if(userPreferences.ZPS==2) { return (dayInfo.isShishi && !dayInfo.isErevHag && !dayInfo.isErevZom)||(dayInfo.isShabat && !dayInfo.isHag && !dayInfo.isZom)||(dayInfo.isMozaeiShabat && !dayInfo.isMozaeiHag && !dayInfo.isMozaeiZom);}
    if(userPreferences.ZPS==3) { return true; }
    return false;
}
void MainState::timeSetShabatInParashaShabatOut()
{
    // כניסת שבת - פרשת השבוע - צאת שבת
    char parasha[50];
    dayInfo.getParasha(parasha);
    Text txt = Text(mainScreen.colorBottomV, &ariel10, 0, 9, CENTER);
    bool fit10 = mainScreen.zmanMiddleDownS->textFits(parasha, txt);
    if(!fit10)
    {
        txt = Text(mainScreen.colorBottomV, &font7, 0, 9, CENTER);
        bool fit7 = mainScreen.zmanMiddleDownS->textFits(parasha, txt);
        if(!fit7)
        {
            txt = Text(mainScreen.colorBottomV, &font6, 0, 9, CENTER);
        }
    }
    Text txtTitle = Text(mainScreen.colorBottomT, &font5, 0, 8, CENTER);
    if(fit10){txtTitle.marginY=7;}

    mainScreen.update(mainScreen.zmanRightUpS, strings.shabatIn, txtTitle6);
    mainScreen.update(mainScreen.zmanRightDownS, dayInfo.nextShishi->shabatIn, txtZman);
    mainScreen.update(mainScreen.zmanMiddleUpS, strings.parashatShavua, txtTitle);
    mainScreen.update(mainScreen.zmanMiddleDownS, parasha, txt);
    mainScreen.update(mainScreen.zmanLeftUpS, strings.shabatOut, txtTitle6);
    mainScreen.update(mainScreen.zmanLeftDownS, dayInfo.nextShabat->shabatOut, txtZman);
}
bool MainState::flagTimeSetShabatInParashaShabatOut()
{
    if(userPreferences.IPO==0) { return false; }
    if(userPreferences.IPO==1) { return dayInfo.isYomHol(); }
    if(userPreferences.IPO==2) { return (dayInfo.isShishi && !dayInfo.isErevHag && !dayInfo.isErevZom)||(dayInfo.isShabat && !dayInfo.isHag && !dayInfo.isZom)||(dayInfo.isMozaeiShabat && !dayInfo.isMozaeiHag && !dayInfo.isMozaeiZom);}
    if(userPreferences.IPO==3) { return true; }
    return false;
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
    if(dayInfo.isYomHol())
    {
        if(userPreferences.ZSZ==1) 
        {
            if(isTimeInRange(zmanPlusDakot(dayInfo.getZeetCochavim(), 31), Zman(23,59))||isTimeInRange(Zman(0,0), Zman(3,0))) 
            return true; 
        }
        if(userPreferences.ZSZ==2) { return true; }
        if(userPreferences.ZSZ==0) { return false; }
    }
    else
    {
        if(userPreferences.ZSZ1==1) 
        {
            if(isTimeInRange(zmanPlusDakot(dayInfo.getZeetCochavim(), 31), Zman(23,59))||isTimeInRange(Zman(0,0), Zman(3,0)))            
            return true; 
        }
        if(userPreferences.ZSZ1==2) { return true; }
        if(userPreferences.ZSZ1==0) { return false; }
    }
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



