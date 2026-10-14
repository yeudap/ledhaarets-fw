#include "States.h"
extern DayInfo dayInfo;
extern DayInfo dayInfoTomorrow;

bool showAll2 = false;
void MainState::initInfoSetFuncList2()
{
    numOfInfoSets = 21;
    infoSetFuncList[0] = std::make_pair(&MainState::infoSet_2_RegularDay, &MainState::flaginfoSet_2_RegularDay);
    infoSetFuncList[1] = std::make_pair(&MainState::infoSet_2_ShabatInShabatOutRabenuTam, &MainState::flaginfoSet_2_ShabatInShabatOutRabenuTam);
    infoSetFuncList[2] = std::make_pair(&MainState::infoSet_2_HagInHagOutRabenuTam, &MainState::flaginfoSet_2_HagInHagOutRabenuTam);
    infoSetFuncList[3] = std::make_pair(&MainState::infoSet_2_RoshHodesh, &MainState::flaginfoSet_2_RoshHodesh);
    infoSetFuncList[4] = std::make_pair(&MainState::infoSet_2_ShabatPlus, &MainState::flaginfoSet_2_ShabatPlus);
    infoSetFuncList[5] = std::make_pair(&MainState::infoSet_2_NextRoshHodesh, &MainState::flaginfoSet_2_NextRoshHodesh);
    infoSetFuncList[6] = std::make_pair(&MainState::infoSet_2_Molad, &MainState::flaginfoSet_2_Molad);
    infoSetFuncList[7] = std::make_pair(&MainState::infoSet_2_HagYaaleVeyavoHalel, &MainState::flaginfoSet_2_HagYaaleVeyavoHalel);
    infoSetFuncList[8] = std::make_pair(&MainState::infoSet_2_ErevHagEruv, &MainState::flaginfoSet_2_ErevHagEruv);
    infoSetFuncList[9] = std::make_pair(&MainState::infoSet_2_Hametz, &MainState::flaginfoSet_2_Hametz);
    infoSetFuncList[10] = std::make_pair(&MainState::infoSet_2_Omer, &MainState::flaginfoSet_2_Omer);
    infoSetFuncList[11] = std::make_pair(&MainState::infoSet_2_Hanuka, &MainState::flaginfoSet_2_Hanuka);
    infoSetFuncList[12] = std::make_pair(&MainState::infoSet_2_Zom, &MainState::flaginfoSet_2_Zom);
    infoSetFuncList[13] = std::make_pair(&MainState::infoSet_2_TishaBeav, &MainState::flaginfoSet_2_TishaBeav);
    infoSetFuncList[14] = std::make_pair(&MainState::infoSet_2_Israeli, &MainState::flaginfoSet_2_Israeli);
    infoSetFuncList[15] = std::make_pair(&MainState::infoSet_2_AseretYemeTshuva, &MainState::flaginfoSet_2_AseretYemeTshuva);
    infoSetFuncList[16] = std::make_pair(&MainState::infoSet_2_Purim, &MainState::flaginfoSet_2_Purim);
    infoSetFuncList[17] = std::make_pair(&MainState::infoSet_2_Yerushalmi, &MainState::flaginfoSet_2_Yerushalmi);
    infoSetFuncList[18] = std::make_pair(&MainState::infoSet_2_Haftara, &MainState::flaginfoSet_2_Haftara);
    infoSetFuncList[19] = std::make_pair(&MainState::infoSet_2_Ivri, &MainState::flaginfoSet_2_Ivri);
    infoSetFuncList[20] = std::make_pair(&MainState::infoSet_2_Chabad, &MainState::flaginfoSet_2_Chabad);

}
// יום רגיל
void MainState::infoSet_2_RegularDay()
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
                mainScreen.update(mainScreen.infoUpHalfRightS, dayInfo.getGeshemTal(), txtInfoUp);
                mainScreen.update(mainScreen.infoUpHalfLeftS, dayInfo.getBircatHashanim(), txtInfoUp);
            }
            else{
                if(dayInfo.isRoshHodesh && dayInfo.isShabat)
                {
                    mainScreen.update(mainScreen.infoUpHalfRightS, dayInfo.getGeshemTal(), txtInfoUp);
                    mainScreen.update(mainScreen.infoUpHalfLeftS, "wrmh vwt", txtInfoUp);
                }
                else{
                    mainScreen.update(mainScreen.infoUpS, dayInfo.getGeshemTal(), txtInfoUp);
                }
            }
        }
        if(userPreferences.dafHayomi==1)
        {
            if(userPreferences.tfila==1) { mainScreen.update(mainScreen.infoDownS, dafStr, txtInfoDown); }
            else { 
                mainScreen.update(mainScreen.infoAllS, dafStr, txtInfoDown);
            }
        } 

        if(dayInfo.enTahanun && userPreferences.tfila==1)
        {
            mainScreen.update(mainScreen.infoShortUpLeft, strings.enOmrimRT, txtInfoUp);
            mainScreen.update(mainScreen.infoShortDownLeft, strings.tahanun, txtInfoUp);
        }
        else if(dayInfo.enZidkatcha && userPreferences.tfila==1)
        {
            mainScreen.update(mainScreen.infoShortUpLeft, strings.enOmrimRT, txtInfoUp);
            mainScreen.update(mainScreen.infoShortDownLeft, strings.zidkatcha, txtInfoUp);        
        }
    }
    else
    {
        if(userPreferences.tfila==1)
        {
            if(dayInfo.getBircatHashanim() != "")
            {
                mainScreen.update(mainScreen.infoUpHalfRight, dayInfo.getGeshemTal(), txtInfoUp);
                mainScreen.update(mainScreen.infoUpHalfLeft, dayInfo.getBircatHashanim(), txtInfoUp);
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
                mainScreen.update(mainScreen.infoUp, str, txtInfoUp);
            }
        }
        if(userPreferences.dafHayomi==1)
        {
            if(userPreferences.tfila==1) { mainScreen.update(mainScreen.infoDown, dafStr, txtInfoDown); }
            else 
            { 
                mainScreen.update(mainScreen.infoAll, dafStr, txtInfoDown);
            }
        }     
    }
}
bool MainState::flaginfoSet_2_RegularDay()
{
    bool f = (userPreferences.tfila==1 || userPreferences.dafHayomi==1);
    return f || showAll2;
}
// שישי ושבת
void MainState::infoSet_2_ShabatInShabatOutRabenuTam()
{
    // כניסת שבת - צאת שבת - רבינו תם
    mainScreen.update(mainScreen.zmanInfoRightUp, strings.cnisatShabat, txtInfoUp);
    mainScreen.update(mainScreen.zmanInfoRightDown, dayInfo.nextShishi->shabatIn, txtInfoDown);
    mainScreen.update(mainScreen.zmanInfoMiddleUp, strings.zeetShabat, txtInfoUp);
    mainScreen.update(mainScreen.zmanInfoMiddleDown, dayInfo.nextShabat->shabatOut, txtInfoDown);
    mainScreen.update(mainScreen.zmanInfoLeftUp, strings.rabenuTam, txtInfoUp);
    mainScreen.update(mainScreen.zmanInfoLeftDown, dayInfo.nextShabat->rabenuTam, txtInfoDown);
}
bool MainState::flaginfoSet_2_ShabatInShabatOutRabenuTam()
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
void MainState::infoSet_2_ShabatPlus()
{
    // שבת מיוחדת
    char shabatPlus[50];
    dayInfo.getShabatPlus(shabatPlus);
    mainScreen.update(mainScreen.infoAll, shabatPlus, txtInfoUp);
}
bool MainState::flaginfoSet_2_ShabatPlus()
{
    bool f = userPreferences.tfila==1;
    f = f && (dayInfo.isShabatPlus);
    f = f && dayInfo.isShabat;
    return f || showAll2;
}
// יום טוב
void MainState::infoSet_2_HagInHagOutRabenuTam()
{
    // כניסת החג - צאת החג - רבינו תם
    mainScreen.update(mainScreen.zmanInfoRightUp, strings.cnisatHag, txtInfoUp);
    mainScreen.update(mainScreen.zmanInfoRightDown, dayInfo.getHagIn(), txtInfoDown);
    mainScreen.update(mainScreen.zmanInfoMiddleUp, strings.zeetHag, txtInfoUp);
    mainScreen.update(mainScreen.zmanInfoMiddleDown, dayInfo.getHagOut(), txtInfoDown);
    mainScreen.update(mainScreen.zmanInfoLeftUp, strings.rabenuTamRT, txtInfoUp);
    mainScreen.update(mainScreen.zmanInfoLeftDown, dayInfo.rabenuTam, txtInfoDown);
}
bool MainState::flaginfoSet_2_HagInHagOutRabenuTam()
{
    bool f = userPreferences.moadim==1;
    f = f && (dayInfo.isHag || dayInfo.isErevHag);
    return f || showAll2;
}
// ימים מיוחדים - שינויים בתפילה
void MainState::infoSet_2_HagYaaleVeyavoHalel()
{
    char holiday[50];
    dayInfo.getHoliday(holiday);
    if((dayInfo.halelDilug||dayInfo.halelShalem) && userPreferences.tfila==1) // הלל וגם יעלה ויבוא
    {
        mainScreen.update(mainScreen.infoUp, holiday, txtInfoUp);
        if(dayInfo.halelDilug) {mainScreen.update(mainScreen.infoDownHalfRight, strings.halelBedilug, txtInfoDown);}
        if(dayInfo.halelShalem) {mainScreen.update(mainScreen.infoDownHalfRight, strings.halelShalem, txtInfoDown);}
        mainScreen.update(mainScreen.infoDownHalfLeft, strings.yaaleveyavo, txtInfoDown);
    }
    else if(dayInfo.yaaleVeyavo && userPreferences.tfila==1) // רק יעלה ויבוא
    {
        mainScreen.update(mainScreen.infoUp, holiday, txtInfoUp);
        mainScreen.update(mainScreen.infoDown, strings.yaaleveyavo, txtInfoDown);
    }
    else // אין שינויים בתפילה (מועדים)
    {
        mainScreen.update(mainScreen.infoAll, holiday, txtInfoUp);
    }
}
bool MainState::flaginfoSet_2_HagYaaleVeyavoHalel()
{
    bool f = (dayInfo.isHag || dayInfo.isMoed);
    return f || showAll2;
}
// ערב חג - ערוב תבשילין
void MainState::infoSet_2_ErevHagEruv()
{
    char holiday[50];
    dayInfo.getHoliday(holiday);
    if(dayInfo.eruvTavshilin)
    {
        mainScreen.update(mainScreen.infoUp, holiday, txtInfoUp);
        mainScreen.update(mainScreen.infoDown, strings.eruvTavshilin, txtInfoDown);
    }
    else 
    {
        mainScreen.update(mainScreen.infoAll, holiday, txtInfoUp);
    }
}
bool MainState::flaginfoSet_2_ErevHagEruv()
{
    bool f = (dayInfo.isErevHag);
    return f || showAll2;
}
// צום
void MainState::infoSet_2_Zom()
{
    // כניסת הצום - שם הצום - צאת הצום
    mainScreen.update(mainScreen.zmanRightUpS, strings.cnisatZom, txtInfoUp);
    mainScreen.update(mainScreen.zmanRightDownS, dayInfo.tzomIn, txtInfoDown);

    mainScreen.update(mainScreen.zmanLeftUpS, strings.zeetZom, txtInfoUp);
    mainScreen.update(mainScreen.zmanLeftDownS, dayInfo.tzomOut, txtInfoDown);

    // צום רגיל
    // עננו / יעלה ויבוא
    char holiday[50];
    if(dayInfo.anenu && userPreferences.tfila==1) { mainScreen.update(mainScreen.zmanMiddleDownS, strings.anenu, txtInfoDown); }
    if(dayInfo.yaaleVeyavo && userPreferences.tfila==1) { mainScreen.update(mainScreen.zmanMiddleDownS, strings.yaaleveyavo, txtInfoDown); }
    if(dayInfo.isZom||dayInfo.isMozaeiZom) 
    { 
        dayInfo.getHoliday(holiday);
        // if(!((dayInfo.anenu||dayInfo.yaaleVeyavo) && userPreferences.tfila==1)) { txt.marginY = 16; }
        mainScreen.update(mainScreen.zmanMiddleUpS, holiday, txtInfoDown); 
    }
    // ערב ט"ב ויו"כ
    if(dayInfo.isErevZom) 
    { 
        dayInfo.tomorrow->getHoliday(holiday);
        mainScreen.update(mainScreen.zmanMiddleUpS, strings.erev, txtInfoDown);
        mainScreen.update(mainScreen.zmanMiddleDownS, holiday, txtInfoDown);
    }
}
bool MainState::flaginfoSet_2_Zom()
{
    bool f = dayInfo.isZom || dayInfo.isErevZom;
    f = f || (dayInfo.isMozaeiZom && isTimeInRange(Zman(14,0), zmanPlusDakot(dayInfo.rabenuTam, 5)));
    f = f && userPreferences.moadim==1;
    return f || showAll2;
}
void MainState::infoSet_2_TishaBeav()
{
    // צום תשעה באב
    // שנים לחורבן
    char shanim[50];
    dayInfo.getShanimLahurban(shanim);
    mainScreen.update(mainScreen.infoUp, strings.zomTishaBeav, txtInfoUp);
    mainScreen.update(mainScreen.infoDown, shanim, txtInfoDown);
}
bool MainState::flaginfoSet_2_TishaBeav()
{
    bool f = userPreferences.moadim==1;
    f = f && dayInfo.isTishaBeav;
    return f || showAll2;
}

// ראש חודש
void MainState::infoSet_2_RoshHodesh()
{
    // 
    char roshHodesh[50];
    dayInfo.tomorrow->getRoshHodesh(roshHodesh);
    mainScreen.update(mainScreen.infoUp, roshHodesh, txtInfoUp);
    if(userPreferences.tfila==1)
    {
        if(dayInfo.halelDilug||dayInfo.halelShalem) { mainScreen.update(mainScreen.infoDownHalfRight, strings.yaaleveyavo, txtInfoDown);}
        else {mainScreen.update(mainScreen.infoDown, strings.yaaleveyavo, txtInfoDown);}
        if(dayInfo.halelDilug) {mainScreen.update(mainScreen.infoDownHalfLeft, strings.halelBedilug, txtInfoDown);}
        if(dayInfo.halelShalem) {mainScreen.update(mainScreen.infoDownHalfLeft, strings.halelShalem, txtInfoDown);}
    }
}
bool MainState::flaginfoSet_2_RoshHodesh()
{
    bool f = userPreferences.moadim==1;
    f = f && dayInfo.isRoshHodesh;
    return f || showAll2;
}
// ראש חודש קרוב
void MainState::infoSet_2_NextRoshHodesh()
{
    // 
    char nextRoshHodeshUp[50];
    char nextRoshHodeshDown[50];
    dayInfo.getNextRoshHodesh(nextRoshHodeshUp, nextRoshHodeshDown);
    mainScreen.update(mainScreen.infoUp, nextRoshHodeshUp, txtInfoUp);
    mainScreen.update(mainScreen.infoDown, nextRoshHodeshDown, txtInfoUp);
}
bool MainState::flaginfoSet_2_NextRoshHodesh()
{
    bool f = userPreferences.moadim==1;
    f = f && userPreferences.tfila==1;
    f = f && dayInfo.isShabatMevarchin;
    f = f && dayInfo.isShabat;
    f = f && isTimeInRange(Zman(5,0),Zman(14,0));
    return f || showAll2;
}
// מולד
void MainState::infoSet_2_Molad()
{
    char moladUp[50];
    char moladDown[50];
    dayInfo.getMolad(moladUp, moladDown);
    mainScreen.update(mainScreen.infoUp, moladUp, txtInfoUp);
    mainScreen.update(mainScreen.infoDown, moladDown, txtInfoUp);
}
bool MainState::flaginfoSet_2_Molad()
{
    bool f = userPreferences.moadim==1;
    f = f && userPreferences.tfila==1;
    f = f && dayInfo.isShabatMevarchin;
    f = f && dayInfo.isShabat;
    f = f && isTimeInRange(Zman(5,0),Zman(14,0));
    return f || showAll2;
}
// מועד ישראלי
void MainState::infoSet_2_Israeli()
{
    // יום העצמאות/השואה/הזיכרון/ירושלים
    char buffer[50];
    dayInfo.getHoliday(buffer);
    mainScreen.update(mainScreen.infoUp, buffer, txtInfoUp);
    if(dayInfo.isAzmaut) 
    {
        dayInfo.getShanimLamdina(buffer);
        mainScreen.update(mainScreen.infoDown, buffer, txtInfoDown); 
    }
    if(dayInfo.isYomYerushalaim) 
    {
        dayInfo.getShanimLyerushalaim(buffer);
        mainScreen.update(mainScreen.infoDown, buffer, txtInfoDown); 
    }
    if(dayInfo.isYomHashoa) { mainScreen.update(mainScreen.infoDown, strings.LashoaVelagvura, txtInfoDown); }
    if(dayInfo.isYomHazicaron) { mainScreen.update(mainScreen.infoDown, strings.LehalleMaarchotIsrael, txtInfoDown); }
}
bool MainState::flaginfoSet_2_Israeli()
{
    bool f = userPreferences.showIsraeli==1;
    f = f && dayInfo.isHagIsraeli;
    return f || showAll2;
}
// ספירת העומר
void MainState::infoSet_2_Omer()
{
    // היום ... ימים לעומר
    // ...שהם
    char line1[50];
    char line2[50];
    dayInfo.getOmer(line1, line2);
    if(dayInfo.omer < 7)
    {
        mainScreen.update(mainScreen.infoAll, line1, txtInfoUp);
    }
    else{
        mainScreen.update(mainScreen.infoUp, line1, txtInfoUp);
        mainScreen.update(mainScreen.infoDown, line2, txtInfoUp);
    }
}
bool MainState::flaginfoSet_2_Omer()
{
    bool f = userPreferences.tfila==1;
    f = f && dayInfo.isOmer;
    return f || showAll2;
}
// סוף זמן חמץ
void MainState::infoSet_2_Hametz()
{
    // סוף זמן אכילת חמץ
    mainScreen.update(mainScreen.infoUpHalfRight, strings.sofZmanAchilatHametz, txtInfoUp);
    mainScreen.update(mainScreen.infoDownHalfRight, dayInfo.sofZmanTfilaMga, txtInfoDown);
    // סוף זמן ביעור חמץ
    mainScreen.update(mainScreen.infoUpHalfLeft, strings.sofZmanBeiurHametz, txtInfoUp);
    mainScreen.update(mainScreen.infoDownHalfLeft, dayInfo.sofZmanBeiurHamez, txtInfoDown);
}
bool MainState::flaginfoSet_2_Hametz()
{
    bool f = userPreferences.moadim==1;
    f = f && dayInfo.tomorrow->isPesach;
    f = f && isTimeInRange(Zman(1,0), Zman(13,0));
    return f || showAll2;
}
// חנוכה
void MainState::infoSet_2_Hanuka()
{
    // נר [יום] של חנוכה
    // על הניסים
    char hanuka[50];
    dayInfo.getHanuka(hanuka);
    mainScreen.update(mainScreen.infoUp, hanuka, txtInfoUp);
    if(userPreferences.tfila==1) { 
        mainScreen.update(mainScreen.infoDownHalfRight, strings.alHanisim, txtInfoDown); 
        mainScreen.update(mainScreen.infoDownHalfLeft, strings.halelShalem, txtInfoDown); 
    }
}
bool MainState::flaginfoSet_2_Hanuka()
{
    bool f = userPreferences.moadim==1;
    f = f && dayInfo.isHanuka;
    return f || showAll2;
}
// עשרת ימי תשובה
void MainState::infoSet_2_AseretYemeTshuva()
{
    // עשרת ימי תשובה
    // המלך הקדוש - המלך המשפט
    mainScreen.update(mainScreen.infoUp, strings.aseretYemeTshuva, txtInfoUp);
    if(dayInfo.hamelechHamishpat)
    {
        mainScreen.update(mainScreen.infoDownHalfRight, strings.hamelechHakadosh, txtInfoDown);
        mainScreen.update(mainScreen.infoDownHalfLeft, strings.hamelechHamishpat, txtInfoDown);
    }
    else{
        mainScreen.update(mainScreen.infoDown, strings.hamelechHakadosh, txtInfoDown);
    }
}
bool MainState::flaginfoSet_2_AseretYemeTshuva()
{
    bool f = userPreferences.tfila==1;
    f = f && dayInfo.hamelechHakadosh;
    return f || showAll2;
}
// פורים
void MainState::infoSet_2_Purim()
{
    // פורים שמח
    // על הניסים
    mainScreen.update(mainScreen.infoUp, strings.purimSameah, txtInfoUp);
    if(userPreferences.tfila==1) { mainScreen.update(mainScreen.infoDown, strings.alHanisim, txtInfoDown); }
}
bool MainState::flaginfoSet_2_Purim()
{
    bool f = userPreferences.moadim==1;
    f = f && ((userPreferences.homa==1 && dayInfo.isShushanPurim)||(userPreferences.homa==0 && dayInfo.isPurim));
    return f || showAll2;
}
// דף היומי ירושלמי
void MainState::infoSet_2_Yerushalmi()
{
    char dafStr[50];
    dayInfo.dafYomiYerushalmi.toString(dafStr, false);
    mainScreen.update(mainScreen.infoUp, strings.dafHayomiYerushalmi, txtInfoUp);
    mainScreen.update(mainScreen.infoDown, dafStr, txtInfoDown);
}
bool MainState::flaginfoSet_2_Yerushalmi()
{
    bool f = userPreferences.yerushalmi==1;
    return f || showAll2;
}
// הפטרה
void MainState::infoSet_2_Haftara()
{
    char shem[50];
    char makor[50];
    dayInfo.getHaftara(shem, makor);
    mainScreen.update(mainScreen.infoUp, shem, txtInfoUp);
    mainScreen.update(mainScreen.infoDown, makor, txtInfoDown);
}
bool MainState::flaginfoSet_2_Haftara()
{
    bool f = dayInfo.isShabat || dayInfo.isHag || dayInfo.isYomKipur || dayInfo.isTishaBeav;
    f = f || (dayInfo.isZom && (userPreferences.eda == ASHKENAZ || userPreferences.eda == CHABAD || userPreferences.eda == SFARAD));
    f = f || (userPreferences.showIsraeli==1 && dayInfo.isAzmaut);
    f = f && userPreferences.haftara==1;
    return f || showAll2;
}
// תאריך עברי
void MainState::infoSet_2_Ivri()
{
    char date[50];
    dayInfo.hebrewDate.toString(date, 2);
    mainScreen.update(mainScreen.infoAll, date, txtInfoUp);
}
bool MainState::flaginfoSet_2_Ivri()
{
    bool f = userPreferences.ivri==1;
    return f || showAll2;
}
// ימי חב"ד
void MainState::infoSet_2_Chabad()
{
    char chabadStr[40];
    if(dayInfo.yom_chabad >= 20)
    {
        char chabadStr1[40];
        uint8_t idx = dayInfo.yom_chabad;
        idx = (idx-20)*2+20;
        dayInfo.getYomHabad(chabadStr, idx);
        dayInfo.getYomHabad(chabadStr1, idx+1);
        mainScreen.update(mainScreen.infoUp, chabadStr, txtInfoUp);
        mainScreen.update(mainScreen.infoDown, chabadStr1, txtInfoUp);
    }
    else
    {
        dayInfo.getYomHabad(chabadStr, dayInfo.yom_chabad);
        mainScreen.update(mainScreen.infoAll, chabadStr, txtInfoUp);
    }
}
bool MainState::flaginfoSet_2_Chabad()
{
    bool f = userPreferences.eda==6;
    f = f && dayInfo.isYomChabad;
    return f || showAll2;
}
