#include "States.h"
extern DayInfo dayInfo;
extern DayInfo dayInfoTomorrow;

bool showAll = false;
void MainState::initInfoSetFuncList()
{
    infoSetFuncList[0] = std::make_pair(&MainState::infoSetRegularDay, &MainState::flagInfoSetRegularDay);
    infoSetFuncList[1] = std::make_pair(&MainState::infoSetShabatInShabatOutRabenuTam, &MainState::flagInfoSetShabatInShabatOutRabenuTam);
    infoSetFuncList[2] = std::make_pair(&MainState::infoSetHagInHagOutRabenuTam, &MainState::flagInfoSetHagInHagOutRabenuTam);
    infoSetFuncList[3] = std::make_pair(&MainState::infoSetRoshHodesh, &MainState::flagInfoSetRoshHodesh);
    infoSetFuncList[4] = std::make_pair(&MainState::infoSetShabatPlus, &MainState::flagInfoSetShabatPlus);
    infoSetFuncList[5] = std::make_pair(&MainState::infoSetNextRoshHodesh, &MainState::flagInfoSetNextRoshHodesh);
    infoSetFuncList[6] = std::make_pair(&MainState::infoSetMolad, &MainState::flagInfoSetMolad);
    infoSetFuncList[7] = std::make_pair(&MainState::infoSetHagYaaleVeyavoHalel, &MainState::flagInfoSetHagYaaleVeyavoHalel);
    infoSetFuncList[8] = std::make_pair(&MainState::infoSetErevHagEruv, &MainState::flagInfoSetErevHagEruv);
    infoSetFuncList[9] = std::make_pair(&MainState::infoSetHametz, &MainState::flagInfoSetHametz);
    infoSetFuncList[10] = std::make_pair(&MainState::infoSetOmer, &MainState::flagInfoSetOmer);
    infoSetFuncList[11] = std::make_pair(&MainState::infoSetHanuka, &MainState::flagInfoSetHanuka);
    infoSetFuncList[12] = std::make_pair(&MainState::infoSetZom, &MainState::flagInfoSetZom);
    infoSetFuncList[13] = std::make_pair(&MainState::infoSetTishaBeav, &MainState::flagInfoSetTishaBeav);
    infoSetFuncList[14] = std::make_pair(&MainState::infoSetIsraeli, &MainState::flagInfoSetIsraeli);
    infoSetFuncList[15] = std::make_pair(&MainState::infoSetAseretYemeTshuva, &MainState::flagInfoSetAseretYemeTshuva);
    infoSetFuncList[16] = std::make_pair(&MainState::infoSetPurim, &MainState::flagInfoSetPurim);
    infoSetFuncList[17] = std::make_pair(&MainState::infoSetYerushalmi, &MainState::flagInfoSetYerushalmi);
    infoSetFuncList[18] = std::make_pair(&MainState::infoSetHaftara, &MainState::flagInfoSetHaftara);
    infoSetFuncList[19] = std::make_pair(&MainState::infoSetIvri, &MainState::flagInfoSetIvri);
    infoSetFuncList[20] = std::make_pair(&MainState::infoSetChabad, &MainState::flagInfoSetChabad);

}
// יום רגיל
void MainState::infoSetRegularDay()
{
    // משיב/מוריד - ברכנו/ברך עלינו
    // דף היומי
    // א"א תחנון/צדקתך או טמפרטורה
    char dafStr[50];
    dayInfo.dafYomiBavli.toString(dafStr, true);
    if(dayInfo.enZidkatcha || dayInfo.enTahanun) // small screen
    {
        if(userPreferences.tfila==1)
        {
            if(dayInfo.getBircatHashanim() != "")
            {
                mainScreen.update(mainScreen.infoUpHalfRightS, dayInfo.getGeshemTal(), txtTitle6);
                mainScreen.update(mainScreen.infoUpHalfLeftS, dayInfo.getBircatHashanim(), txtTitle6);
            }
            else{
                if(dayInfo.isRoshHodesh && dayInfo.isShabat)
                {
                    mainScreen.update(mainScreen.infoUpHalfRightS, dayInfo.getGeshemTal(), txtTitle6);
                    mainScreen.update(mainScreen.infoUpHalfLeftS, "wrmh vwt", txtTitle6);
                }
                else{
                    mainScreen.update(mainScreen.infoUpS, dayInfo.getGeshemTal(), txtTitle6);
                }
            }
        }
        if(userPreferences.dafHayomi==1)
        {
            if(userPreferences.tfila==1) { mainScreen.update(mainScreen.infoDownS, dafStr, txtValue6); }
            else { 
                Text txt = Text(mainScreen.colorBottomV, &font6, 1, 12, CENTER);
                mainScreen.update(mainScreen.infoAllS, dafStr, txt);
            }
        } 

        if(dayInfo.enTahanun && userPreferences.tfila==1)
        {
            mainScreen.update(mainScreen.infoShortUpLeft, strings.enOmrim, txtTitle6);
            Text txt = Text(mainScreen.colorBottomT, &font6, 0, 8, CENTER);
            mainScreen.update(mainScreen.infoShortDownLeft, strings.tahanun, txt);
        }
        else if(dayInfo.enZidkatcha && userPreferences.tfila==1)
        {
            mainScreen.update(mainScreen.infoShortUpLeft, strings.enOmrim, txtTitle6);
            Text txt = Text(mainScreen.colorBottomT, &font6, 0, 8, CENTER);
            mainScreen.update(mainScreen.infoShortDownLeft, strings.zidkatcha, txt);        
        }
    }
    else
    {
        if(userPreferences.tfila==1)
        {
            if(dayInfo.getBircatHashanim() != "")
            {
                mainScreen.update(mainScreen.infoUpHalfRight, dayInfo.getGeshemTal(), txtTitle6);
                mainScreen.update(mainScreen.infoUpHalfLeft, dayInfo.getBircatHashanim(), txtTitle6);
            }
            else{
                const char* str;
                if(dayInfo.isPesach)
                {
                    if(isTimeInRange(Zman(5,0),Zman(13,0))) {str = strings.moridHatalMimusaf; }
                    else if(isTimeInRange(Zman(13,1),Zman(23,59))) { str =  strings.moridHatal; }
                    else { str = strings.mashivHaruah; }
                }
                else if((dayInfo.isSimchatTora && userPreferences.hul==0) || (dayInfo.isShminiAzeret))
                {
                    if(isTimeInRange(Zman(5,0),Zman(13,0))) { str =  strings.mashivHaruahMimusaf; }
                    else if(isTimeInRange(Zman(13,1),Zman(23,59))) { str = strings.mashivHaruah; }
                    else { str = strings.moridHatal; }
                }
                else { str = dayInfo.getGeshemTal(); }
                mainScreen.update(mainScreen.infoUp, str, txtTitle6);
            }
        }
        if(userPreferences.dafHayomi==1)
        {
            if(userPreferences.tfila==1) { mainScreen.update(mainScreen.infoDown, dafStr, txtValue6); }
            else 
            { 
                Text txt = Text(mainScreen.colorBottomV, &font6, 1, 12, CENTER);
                mainScreen.update(mainScreen.infoAll, dafStr, txt);
            }
        }     
    }
}
bool MainState::flagInfoSetRegularDay()
{
    bool f = (userPreferences.tfila==1 || userPreferences.dafHayomi==1);
    return f || showAll;
}
// שישי ושבת
void MainState::infoSetShabatInShabatOutRabenuTam()
{
    // כניסת שבת - צאת שבת - רבינו תם
    mainScreen.update(mainScreen.zmanInfoRightUp, strings.shabatIn, txtTitle6);
    mainScreen.update(mainScreen.zmanInfoRightDown, dayInfo.nextShishi->shabatIn, txtZman);
    mainScreen.update(mainScreen.zmanInfoMiddleUp, strings.shabatOut, txtTitle6);
    mainScreen.update(mainScreen.zmanInfoMiddleDown, dayInfo.nextShabat->shabatOut, txtZman);
    mainScreen.update(mainScreen.zmanInfoLeftUp, strings.rabenuTamRT, txtTitle6);
    mainScreen.update(mainScreen.zmanInfoLeftDown, dayInfo.nextShabat->rabenuTam, txtZman);
}
bool MainState::flagInfoSetShabatInShabatOutRabenuTam()
{
    if(userPreferences.IORT==0) { return false; }
    if(userPreferences.IORT==1) { return dayInfo.isYomHol(); }
    if(userPreferences.IORT==2)
    { 
        return (dayInfo.isShishi && !dayInfo.isErevHag && !dayInfo.isErevZom)||(dayInfo.isShabat && !dayInfo.isHag && !dayInfo.isZom)||(dayInfo.isMozaeiShabat && !dayInfo.isMozaeiHag && !dayInfo.isMozaeiZom);
    }
    if(userPreferences.IORT==3) { return true; }
    return false;
}
void MainState::infoSetShabatPlus()
{
    // שבת מיוחדת
    char shabatPlus[50];
    dayInfo.getShabatPlus(shabatPlus);
    Text txt = Text(mainScreen.colorBottomT, &font7, 1, 18, CENTER);
    mainScreen.update(mainScreen.infoAll, shabatPlus, txt);
}
bool MainState::flagInfoSetShabatPlus()
{
    bool f = userPreferences.tfila==1;
    f = f && (dayInfo.isShabatPlus);
    f = f && dayInfo.isShabat;
    return f || showAll;
}
// יום טוב
void MainState::infoSetHagInHagOutRabenuTam()
{
    // כניסת החג - צאת החג - רבינו תם
    mainScreen.update(mainScreen.zmanInfoRightUp, strings.hagIn, txtTitle6);
    mainScreen.update(mainScreen.zmanInfoRightDown, dayInfo.getHagIn(), txtZman);
    mainScreen.update(mainScreen.zmanInfoMiddleUp, strings.HagOut, txtTitle6);
    mainScreen.update(mainScreen.zmanInfoMiddleDown, dayInfo.getHagOut(), txtZman);
    mainScreen.update(mainScreen.zmanInfoLeftUp, strings.rabenuTamRT, txtTitle6);
    mainScreen.update(mainScreen.zmanInfoLeftDown, dayInfo.rabenuTam, txtZman);
}
bool MainState::flagInfoSetHagInHagOutRabenuTam()
{
    bool f = userPreferences.moadim==1;
    f = f && (dayInfo.isHag || dayInfo.isErevHag);
    return f || showAll;
}
// ימים מיוחדים - שינויים בתפילה
void MainState::infoSetHagYaaleVeyavoHalel()
{
    char holiday[50];
    dayInfo.getHoliday(holiday);
    if((dayInfo.halelDilug||dayInfo.halelShalem) && userPreferences.tfila==1) // הלל וגם יעלה ויבוא
    {
        mainScreen.update(mainScreen.infoUp, holiday, txtTitle6);
        if(dayInfo.halelDilug) {mainScreen.update(mainScreen.infoDownHalfRight, strings.halelBedilug, txtValue6);}
        if(dayInfo.halelShalem) {mainScreen.update(mainScreen.infoDownHalfRight, strings.halelShalem, txtValue6);}
        mainScreen.update(mainScreen.infoDownHalfLeft, strings.yaaleveyavo, txtValue6);
    }
    else if(dayInfo.yaaleVeyavo && userPreferences.tfila==1) // רק יעלה ויבוא
    {
        mainScreen.update(mainScreen.infoUp, holiday, txtTitle6);
        mainScreen.update(mainScreen.infoDown, strings.yaaleveyavo, txtValue6);
    }
    else // אין שינויים בתפילה (מועדים)
    {
        Text txt = Text(mainScreen.colorBottomT, &font7, 1, 18, CENTER);
        mainScreen.update(mainScreen.infoAll, holiday, txt);
    }
}
bool MainState::flagInfoSetHagYaaleVeyavoHalel()
{
    bool f = (dayInfo.isHag || dayInfo.isMoed);
    return f || showAll;
}
// ערב חג - ערוב תבשילין
void MainState::infoSetErevHagEruv()
{
    char holiday[50];
    dayInfo.getHoliday(holiday);
    if(dayInfo.eruvTavshilin)
    {
        mainScreen.update(mainScreen.infoUp, holiday, txtTitle6);
        mainScreen.update(mainScreen.infoDown, strings.eruvTavshilin, txtValue6);
    }
    else 
    {
        Text txt = Text(mainScreen.colorBottomT, &ariel10, 1, 15, CENTER);
        mainScreen.update(mainScreen.infoAll, holiday, txt);
    }
}
bool MainState::flagInfoSetErevHagEruv()
{
    bool f = (dayInfo.isErevHag);
    return f || showAll;
}
// צום
void MainState::infoSetZom()
{
    // כניסת הצום - שם הצום - צאת הצום
    mainScreen.update(mainScreen.zmanRightUpS, strings.ZomIn, txtTitle6);
    mainScreen.update(mainScreen.zmanRightDownS, dayInfo.tzomIn, txtZman);

    mainScreen.update(mainScreen.zmanLeftUpS, strings.ZomOut, txtTitle6);
    mainScreen.update(mainScreen.zmanLeftDownS, dayInfo.tzomOut, txtZman);

    // צום רגיל
    // עננו / יעלה ויבוא
    char holiday[50];
    Text txt1 = Text(mainScreen.colorBottomV, &font6, 1, 10, CENTER);
    if(dayInfo.anenu && userPreferences.tfila==1) { mainScreen.update(mainScreen.zmanMiddleDownS, strings.anenu, txt1); }
    if(dayInfo.yaaleVeyavo && userPreferences.tfila==1) { mainScreen.update(mainScreen.zmanMiddleDownS, strings.yaaleveyavo, txt1); }
    Text txt = Text(mainScreen.colorBottomT, &font7, 1, 11, CENTER);
    if(dayInfo.isZom||dayInfo.isMozaeiZom) 
    { 
        dayInfo.getHoliday(holiday);
        if(!((dayInfo.anenu||dayInfo.yaaleVeyavo) && userPreferences.tfila==1)) { txt.marginY = 16; }
        mainScreen.update(mainScreen.zmanMiddleUpS, holiday, txt); 
    }
    // ערב ט"ב ויו"כ
    if(dayInfo.isErevZom) 
    { 
        dayInfo.tomorrow->getHoliday(holiday);
        mainScreen.update(mainScreen.zmanMiddleUpS, strings.erev, txtTitle6);
        mainScreen.update(mainScreen.zmanMiddleDownS, holiday, txtValue6);
    }
}
bool MainState::flagInfoSetZom()
{
    bool f = dayInfo.isZom || dayInfo.isErevZom;
    f = f || (dayInfo.isMozaeiZom && isTimeInRange(Zman(14,0), zmanPlusDakot(dayInfo.rabenuTam, 5)));
    f = f && userPreferences.moadim==1;
    return f || showAll;
}
void MainState::infoSetTishaBeav()
{
    // צום תשעה באב
    // שנים לחורבן
    char shanim[50];
    dayInfo.getShanimLahurban(shanim);
    mainScreen.update(mainScreen.infoUp, strings.zomTishaBeav, txtTitle6);
    mainScreen.update(mainScreen.infoDown, shanim, txtValue6);
}
bool MainState::flagInfoSetTishaBeav()
{
    bool f = userPreferences.moadim==1;
    f = f && dayInfo.isTishaBeav;
    return f || showAll;
}

// ראש חודש
void MainState::infoSetRoshHodesh()
{
    // 
    char roshHodesh[50];
    dayInfo.tomorrow->getRoshHodesh(roshHodesh);
    mainScreen.update(mainScreen.infoUp, roshHodesh, txtTitle6);
    if(userPreferences.tfila==1)
    {
        if(dayInfo.halelDilug||dayInfo.halelShalem) { mainScreen.update(mainScreen.infoDownHalfRight, strings.yaaleveyavo, txtValue6);}
        else {mainScreen.update(mainScreen.infoDown, strings.yaaleveyavo, txtValue6);}
        if(dayInfo.halelDilug) {mainScreen.update(mainScreen.infoDownHalfLeft, strings.halelBedilug, txtValue6);}
        if(dayInfo.halelShalem) {mainScreen.update(mainScreen.infoDownHalfLeft, strings.halelShalem, txtValue6);}
    }
}
bool MainState::flagInfoSetRoshHodesh()
{
    bool f = userPreferences.moadim==1;
    f = f && dayInfo.isRoshHodesh;
    return f || showAll;
}
// ראש חודש קרוב
void MainState::infoSetNextRoshHodesh()
{
    // 
    char nextRoshHodeshUp[50];
    char nextRoshHodeshDown[50];
    dayInfo.getNextRoshHodesh(nextRoshHodeshUp, nextRoshHodeshDown);
    mainScreen.update(mainScreen.infoUp, nextRoshHodeshUp, txtTitle6);
    mainScreen.update(mainScreen.infoDown, nextRoshHodeshDown, txtValue6);
}
bool MainState::flagInfoSetNextRoshHodesh()
{
    bool f = userPreferences.moadim==1;
    f = f && userPreferences.tfila==1;
    f = f && dayInfo.isShabatMevarchin;
    f = f && dayInfo.isShabat;
    f = f && isTimeInRange(Zman(5,0),Zman(14,0));
    return f || showAll;
}
// מולד
void MainState::infoSetMolad()
{
    char moladUp[50];
    char moladDown[50];
    dayInfo.getMolad(moladUp, moladDown);
    mainScreen.update(mainScreen.infoUp, moladUp, txtTitle6);
    mainScreen.update(mainScreen.infoDown, moladDown, txtTitle6);
}
bool MainState::flagInfoSetMolad()
{
    bool f = userPreferences.moadim==1;
    f = f && userPreferences.tfila==1;
    f = f && dayInfo.isShabatMevarchin;
    f = f && dayInfo.isShabat;
    f = f && isTimeInRange(Zman(5,0),Zman(14,0));
    return f || showAll;
}
// מועד ישראלי
void MainState::infoSetIsraeli()
{
    // יום העצמאות/השואה/הזיכרון/ירושלים
    char buffer[50];
    dayInfo.getHoliday(buffer);
    mainScreen.update(mainScreen.infoUp, buffer, txtTitle6);
    if(dayInfo.isAzmaut) 
    {
        dayInfo.getShanimLamdina(buffer);
        mainScreen.update(mainScreen.infoDown, buffer, txtValue6); 
    }
    if(dayInfo.isYomYerushalaim) 
    {
        dayInfo.getShanimLyerushalaim(buffer);
        mainScreen.update(mainScreen.infoDown, buffer, txtValue6); 
    }
    if(dayInfo.isYomHashoa) { mainScreen.update(mainScreen.infoDown, strings.LashoaVelagvura, txtValue6); }
    if(dayInfo.isYomHazicaron) { mainScreen.update(mainScreen.infoDown, strings.LehalleMaarchotIsrael, txtValue6); }
}
bool MainState::flagInfoSetIsraeli()
{
    bool f = userPreferences.showIsraeli==1;
    f = f && dayInfo.isHagIsraeli;
    return f || showAll;
}
// ספירת העומר
void MainState::infoSetOmer()
{
    // היום ... ימים לעומר
    // ...שהם
    char line1[50];
    char line2[50];
    dayInfo.getOmer(line1, line2);
    if(dayInfo.omer < 7)
    {
        Text txt = Text(mainScreen.colorBottomT, &font6, 1, 12, CENTER);
        mainScreen.update(mainScreen.infoAll, line1, txt);
    }
    else{
        Text txt = txtTitle6;
        if (!mainScreen.infoUp->textFits(line1, txt)) { txt.font = &font5; }
        mainScreen.update(mainScreen.infoUp, line1, txt);
        if (!mainScreen.infoDown->textFits(line2, txt)) { txt.font = &font5; }
        mainScreen.update(mainScreen.infoDown, line2, txt);
    }
}
bool MainState::flagInfoSetOmer()
{
    bool f = userPreferences.tfila==1;
    f = f && dayInfo.isOmer;
    return f || showAll;
}
// סוף זמן חמץ
void MainState::infoSetHametz()
{
    // סוף זמן אכילת חמץ
    mainScreen.update(mainScreen.infoUpHalfRight, strings.sofZmanAchilatHametz, txtTitle6);
    mainScreen.update(mainScreen.infoDownHalfRight, dayInfo.sofZmanTfilaMga, txtZman);
    // סוף זמן ביעור חמץ
    mainScreen.update(mainScreen.infoUpHalfLeft, strings.sofZmanBeiurHametz, txtTitle6);
    mainScreen.update(mainScreen.infoDownHalfLeft, dayInfo.sofZmanBeiurHamez, txtZman);
}
bool MainState::flagInfoSetHametz()
{
    bool f = userPreferences.moadim==1;
    f = f && dayInfo.tomorrow->isPesach;
    f = f && isTimeInRange(Zman(1,0), Zman(13,0));
    return f || showAll;
}
// חנוכה
void MainState::infoSetHanuka()
{
    // נר [יום] של חנוכה
    // על הניסים
    char hanuka[50];
    dayInfo.getHanuka(hanuka);
    mainScreen.update(mainScreen.infoUp, hanuka, txtTitle6);
    if(userPreferences.tfila==1) { 
        mainScreen.update(mainScreen.infoDownHalfRight, strings.alHanisim, txtValue6); 
        mainScreen.update(mainScreen.infoDownHalfLeft, strings.halelShalem, txtValue6); 
    }
}
bool MainState::flagInfoSetHanuka()
{
    bool f = userPreferences.moadim==1;
    f = f && dayInfo.isHanuka;
    return f || showAll;
}
// עשרת ימי תשובה
void MainState::infoSetAseretYemeTshuva()
{
    // עשרת ימי תשובה
    // המלך הקדוש - המלך המשפט
    mainScreen.update(mainScreen.infoUp, strings.aseretYemeTshuva, txtTitle6);
    if(dayInfo.hamelechHamishpat)
    {
        mainScreen.update(mainScreen.infoDownHalfRight, strings.hamelechHakadosh, txtValue6);
        mainScreen.update(mainScreen.infoDownHalfLeft, strings.hamelechHamishpat, txtValue6);
    }
    else{
        mainScreen.update(mainScreen.infoDown, strings.hamelechHakadosh, txtValue6);
    }
}
bool MainState::flagInfoSetAseretYemeTshuva()
{
    bool f = userPreferences.tfila==1;
    f = f && dayInfo.hamelechHakadosh;
    return f || showAll;
}
// פורים
void MainState::infoSetPurim()
{
    // פורים שמח
    // על הניסים
    mainScreen.update(mainScreen.infoUp, strings.purimSameah, txtTitle6);
    if(userPreferences.tfila==1) { mainScreen.update(mainScreen.infoDown, strings.alHanisim, txtValue6); }
}
bool MainState::flagInfoSetPurim()
{
    bool f = userPreferences.moadim==1;
    f = f && ((userPreferences.homa==1 && dayInfo.isShushanPurim)||(userPreferences.homa==0 && dayInfo.isPurim));
    return f || showAll;
}
// דף היומי ירושלמי
void MainState::infoSetYerushalmi()
{
    char dafStr[50];
    dayInfo.dafYomiYerushalmi.toString(dafStr, false);
    mainScreen.update(mainScreen.infoUp, strings.dafHayomiYerushalmi, txtTitle6);
    Text txt = Text(mainScreen.colorBottomV, &font6, 1, 7, CENTER);
    mainScreen.update(mainScreen.infoDown, dafStr, txt);
}
bool MainState::flagInfoSetYerushalmi()
{
    bool f = userPreferences.yerushalmi==1;
    return f || showAll;
}
// הפטרה
void MainState::infoSetHaftara()
{
    char shem[50];
    char makor[50];
    dayInfo.getHaftara(shem, makor);
    Text txt = Text(mainScreen.colorBottomT, &font7, 0, 9, CENTER);
    mainScreen.update(mainScreen.infoUp, shem, txt);
    txt = Text(mainScreen.colorBottomV, &font6, 1, 9, CENTER);
    mainScreen.update(mainScreen.infoDown, makor, txt);
}
bool MainState::flagInfoSetHaftara()
{
    bool f = dayInfo.isShabat || dayInfo.isHag || dayInfo.isYomKipur || dayInfo.isTishaBeav;
    f = f || (dayInfo.isZom && (userPreferences.eda == ASHKENAZ || userPreferences.eda == CHABAD || userPreferences.eda == SFARAD));
    f = f || (userPreferences.showIsraeli==1 && dayInfo.isAzmaut);
    f = f && userPreferences.haftara==1;
    return f || showAll;
}
// תאריך עברי
void MainState::infoSetIvri()
{
    char date[50];
    dayInfo.hebrewDate.toString(date, 2);
    Text txt = Text(mainScreen.colorBottomT, &ariel10, 1, 19, CENTER);
    if (!mainScreen.infoAll->textFits(date, txt)) 
    { 
        dayInfo.hebrewDate.toString(date, 1);
    }
    mainScreen.update(mainScreen.infoAll, date, txt);
}
bool MainState::flagInfoSetIvri()
{
    bool f = userPreferences.ivri==1;
    return f || showAll;
}
// ימי חב"ד
void MainState::infoSetChabad()
{
    char chabadStr[40];
    if(dayInfo.yom_chabad >= 20)
    {
        char chabadStr1[40];
        uint8_t idx = dayInfo.yom_chabad;
        idx = (idx-20)*2+20;
        dayInfo.getYomHabad(chabadStr, idx);
        dayInfo.getYomHabad(chabadStr1, idx+1);
        Text txt = Text(mainScreen.colorBottomT, &font6, 1, 8, CENTER);
        mainScreen.update(mainScreen.infoUp, chabadStr, txt);
        mainScreen.update(mainScreen.infoDown, chabadStr1, txt);
    }
    else
    {
        dayInfo.getYomHabad(chabadStr, dayInfo.yom_chabad);
        Text txt = Text(mainScreen.colorBottomT, &font7, 1, 18, CENTER);
        if (!mainScreen.infoAll->textFits(chabadStr, txt)) { txt.font = &font6; }
        if (!mainScreen.infoAll->textFits(chabadStr, txt)) { txt.font = &font5; }
        mainScreen.update(mainScreen.infoAll, chabadStr, txt);
    }
}
bool MainState::flagInfoSetChabad()
{
    bool f = userPreferences.eda==6;
    f = f && dayInfo.isYomChabad;
    return f || showAll;
}
