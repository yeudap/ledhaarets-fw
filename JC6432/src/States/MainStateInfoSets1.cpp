#include "States.h"
extern DayInfo dayInfo;
extern DayInfo dayInfoTomorrow;

bool showAll = false;
void MainState::initInfoSetFuncList1()
{
    numOfInfoSets = 28;
    infoSetFuncList[0] = std::make_pair(&MainState::infoSet_1_RegularDay, &MainState::flaginfoSet_1_RegularDay);
    infoSetFuncList[1] = std::make_pair(&MainState::infoSet_1_Tahanun, &MainState::flaginfoSet_1_Tahanun);
    infoSetFuncList[2] = std::make_pair(&MainState::infoSet_1_ShabatInShabatOutRabenuTam, &MainState::flaginfoSet_1_ShabatInShabatOutRabenuTam);
    infoSetFuncList[3] = std::make_pair(&MainState::infoSet_1_HagInHagOutRabenuTam, &MainState::flaginfoSet_1_HagInHagOutRabenuTam);
    infoSetFuncList[4] = std::make_pair(&MainState::infoSet_1_RoshHodesh, &MainState::flaginfoSet_1_RoshHodesh);
    infoSetFuncList[5] = std::make_pair(&MainState::infoSet_1_RoshHodesh1, &MainState::flaginfoSet_1_RoshHodesh1);
    infoSetFuncList[6] = std::make_pair(&MainState::infoSet_1_ShabatPlus, &MainState::flaginfoSet_1_ShabatPlus);
    infoSetFuncList[5] = std::make_pair(&MainState::infoSet_1_NextRoshHodesh, &MainState::flaginfoSet_1_NextRoshHodesh);
    infoSetFuncList[6] = std::make_pair(&MainState::infoSet_1_Molad, &MainState::flaginfoSet_1_Molad);
    infoSetFuncList[7] = std::make_pair(&MainState::infoSet_1_HagYaaleVeyavoHalel, &MainState::flaginfoSet_1_HagYaaleVeyavoHalel);
    infoSetFuncList[8] = std::make_pair(&MainState::infoSet_1_HagYaaleVeyavoHalel1, &MainState::flaginfoSet_1_HagYaaleVeyavoHalel1);
    infoSetFuncList[9] = std::make_pair(&MainState::infoSet_1_ErevHagEruv, &MainState::flaginfoSet_1_ErevHagEruv);
    infoSetFuncList[10] = std::make_pair(&MainState::infoSet_1_Hametz, &MainState::flaginfoSet_1_Hametz);
    infoSetFuncList[11] = std::make_pair(&MainState::infoSet_1_Hametz1, &MainState::flaginfoSet_1_Hametz1);
    infoSetFuncList[12] = std::make_pair(&MainState::infoSet_1_Omer, &MainState::flaginfoSet_1_Omer);
    infoSetFuncList[13] = std::make_pair(&MainState::infoSet_1_Omer1, &MainState::flaginfoSet_1_Omer1);
    infoSetFuncList[14] = std::make_pair(&MainState::infoSet_1_Hanuka, &MainState::flaginfoSet_1_Hanuka);
    infoSetFuncList[15] = std::make_pair(&MainState::infoSet_1_Hanuka1, &MainState::flaginfoSet_1_Hanuka1);
    infoSetFuncList[16] = std::make_pair(&MainState::infoSet_1_Zom, &MainState::flaginfoSet_1_Zom);
    infoSetFuncList[17] = std::make_pair(&MainState::infoSet_1_Zom1, &MainState::flaginfoSet_1_Zom1);
    infoSetFuncList[18] = std::make_pair(&MainState::infoSet_1_Zom2, &MainState::flaginfoSet_1_Zom2);
    infoSetFuncList[19] = std::make_pair(&MainState::infoSet_1_TishaBeav, &MainState::flaginfoSet_1_TishaBeav);
    infoSetFuncList[20] = std::make_pair(&MainState::infoSet_1_Israeli, &MainState::flaginfoSet_1_Israeli);
    infoSetFuncList[21] = std::make_pair(&MainState::infoSet_1_AseretYemeTshuva, &MainState::flaginfoSet_1_AseretYemeTshuva);
    infoSetFuncList[22] = std::make_pair(&MainState::infoSet_1_AseretYemeTshuva1, &MainState::flaginfoSet_1_AseretYemeTshuva1);
    infoSetFuncList[23] = std::make_pair(&MainState::infoSet_1_Purim, &MainState::flaginfoSet_1_Purim);
    infoSetFuncList[24] = std::make_pair(&MainState::infoSet_1_Bavli, &MainState::flaginfoSet_1_Bavli);
    infoSetFuncList[25] = std::make_pair(&MainState::infoSet_1_Yerushalmi, &MainState::flaginfoSet_1_Yerushalmi);
    infoSetFuncList[26] = std::make_pair(&MainState::infoSet_1_Haftara, &MainState::flaginfoSet_1_Haftara);
    infoSetFuncList[27] = std::make_pair(&MainState::infoSet_1_Chabad, &MainState::flaginfoSet_1_Chabad);
    
}
// יום רגיל
void MainState::infoSet_1_RegularDay()
{
    //Serial.println("infoSet_1_RegularDay");
    // משיב/מוריד - ברכנו/ברך עלינו
    if(dayInfo.getBircatHashanim() != "")
    {
        mainScreen.update(mainScreen.infoRight, dayInfo.getGeshemTal(), txtInfo);
        mainScreen.update(mainScreen.infoLeft, dayInfo.getBircatHashanim(), txtInfo);
    }
    else{
        const char* str;
        if(dayInfo.isRoshHodesh && dayInfo.isShabat)
        {
            mainScreen.update(mainScreen.infoRight, dayInfo.getGeshemTal(), txtInfo);
            mainScreen.update(mainScreen.infoLeft, "wrmh vwt", txtInfo);
        }
        else {
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
            mainScreen.update(mainScreen.infoAll, str, txtInfo);
        }
    }
}
bool MainState::flaginfoSet_1_RegularDay()
{
    bool f = userPreferences.tfila==1;
    return f || showAll;
}
void MainState::infoSet_1_Tahanun()
{
    //Serial.println("infoSet_1_Tahanun");
    // א"א תחנון/צדקתך
    if(dayInfo.enTahanun)
    {
        mainScreen.updateInfo(strings.tahanun, strings.enOmrim);
    }
    if(dayInfo.enZidkatcha)
    {
        mainScreen.updateInfo(strings.zidkatcha, strings.enOmrim);
    }
}
bool MainState::flaginfoSet_1_Tahanun()
{
    bool f = (userPreferences.tfila==1 && (dayInfo.enZidkatcha || dayInfo.enTahanun));
    return f || showAll;
}
// שישי ושבת
void MainState::infoSet_1_ShabatInShabatOutRabenuTam()
{
    //Serial.println("infoSet_1_ShabatInShabatOutRabenuTam");
    // כניסת שבת - צאת שבת - רבינו תם
    char strAll[50];
    char zmanStr[6];
    dayInfo.nextShishi->shabatIn.toString(zmanStr);
    sprintf(strAll, "%s  %s", zmanStr, strings.shabatIn);
    mainScreen.update(mainScreen.infoRight, strAll, txtInfo);
    dayInfo.nextShabat->shabatOut.toString(zmanStr);
    sprintf(strAll, "%s  %s",zmanStr, strings.shabatOut);
    mainScreen.update(mainScreen.infoLeft, strAll, txtInfo);
}
bool MainState::flaginfoSet_1_ShabatInShabatOutRabenuTam()
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
void MainState::infoSet_1_ShabatPlus()
{
    //Serial.println("infoSet_1_ShabatPlus");
    // שבת מיוחדת
    char shabatPlus[50];
    dayInfo.getShabatPlus(shabatPlus);
    mainScreen.updateInfo(shabatPlus);
}
bool MainState::flaginfoSet_1_ShabatPlus()
{
    bool f = userPreferences.tfila==1;
    f = f && (dayInfo.isShabatPlus);
    f = f && dayInfo.isShabat;
    return f || showAll;
}
// יום טוב
void MainState::infoSet_1_HagInHagOutRabenuTam()
{
    //Serial.println("infoSet_1_HagInHagOutRabenuTam");
    // כניסת החג - צאת החג - רבינו תם
    char strAll[50];
    char zmanStr[6];
    dayInfo.getHagIn().toString(zmanStr);
    sprintf(strAll, "%s  %s", zmanStr, strings.hagIn);
    mainScreen.update(mainScreen.infoRight, strAll, txtInfo);
    dayInfo.getHagOut().toString(zmanStr);
    sprintf(strAll, "%s  %s",zmanStr, strings.HagOut);
    mainScreen.update(mainScreen.infoLeft, strAll, txtInfo);
}
bool MainState::flaginfoSet_1_HagInHagOutRabenuTam()
{
    bool f = userPreferences.moadim==1;
    f = f && (dayInfo.isHag || dayInfo.isErevHag);
    return f || showAll;
}
// ימים מיוחדים - שינויים בתפילה
void MainState::infoSet_1_HagYaaleVeyavoHalel()
{
    //Serial.println("infoSet_1_HagYaaleVeyavoHalel");
    char holiday[50];
    dayInfo.getHoliday(holiday);
    mainScreen.updateInfo(holiday);
}
bool MainState::flaginfoSet_1_HagYaaleVeyavoHalel()
{
    bool f = (dayInfo.isHag || dayInfo.isMoed);
    f = f && !( dayInfo.isZom || dayInfo.isErevZom || dayInfo.isMozaeiZom);
    return f || showAll;
}
void MainState::infoSet_1_HagYaaleVeyavoHalel1()
{
    //Serial.println("infoSet_1_HagYaaleVeyavoHalel1");
    if(dayInfo.halelDilug||dayInfo.halelShalem) // הלל וגם יעלה ויבוא
    {
        if(dayInfo.halelDilug) {mainScreen.update(mainScreen.infoRight, strings.halelBedilug, txtInfo);}
        if(dayInfo.halelShalem) {mainScreen.update(mainScreen.infoRight, strings.halelShalem, txtInfo);}
        mainScreen.update(mainScreen.infoLeft, strings.yaaleveyavo, txtInfo);
    }
    else if(dayInfo.yaaleVeyavo) // רק יעלה ויבוא
    {
        mainScreen.updateInfo(strings.yaaleveyavo);
    }
}
bool MainState::flaginfoSet_1_HagYaaleVeyavoHalel1()
{
    bool f = (dayInfo.isHag || dayInfo.isMoed);
    f = f & (dayInfo.halelDilug||dayInfo.halelShalem || dayInfo.yaaleVeyavo);
    f = f & userPreferences.tfila==1;
    return f || showAll;
}
// ערב חג - ערוב תבשילין
void MainState::infoSet_1_ErevHagEruv()
{
    //Serial.println("infoSet_1_ErevHagEruv");
    char holiday[50];
    dayInfo.getHoliday(holiday);
    if(dayInfo.eruvTavshilin)
    {
        mainScreen.updateInfo(strings.eruvTavshilin, holiday);
    }
    else 
    {
        mainScreen.updateInfo(holiday);
    }
}
bool MainState::flaginfoSet_1_ErevHagEruv()
{
    bool f = (dayInfo.isErevHag);
    return f || showAll;
}
// צום
void MainState::infoSet_1_Zom()
{
    //Serial.println("infoSet_1_Zom");
    char holiday[50];
    dayInfo.getHoliday(holiday);
    // ערב ט"ב ויו"כ
    if(dayInfo.isErevZom) 
    { 
        mainScreen.updateInfo(holiday, strings.erev);
    }
    // צום רגיל
    if(dayInfo.isZom||dayInfo.isMozaeiZom) 
    { 
        mainScreen.updateInfo(holiday); 
    }
}
bool MainState::flaginfoSet_1_Zom()
{
    bool f = dayInfo.isZom || dayInfo.isErevZom;
    f = f || (dayInfo.isMozaeiZom && isTimeInRange(Zman(14,0), zmanPlusDakot(dayInfo.rabenuTam, 5)));
    f = f && userPreferences.moadim==1;
    return f || showAll;
}
void MainState::infoSet_1_Zom1()
{
    //Serial.println("infoSet_1_Zom1");
    // כניסת הצום - שם הצום - צאת הצום
    char strAll[50];
    char zmanStr[6];
    dayInfo.tzomIn.toString(zmanStr);
    sprintf(strAll, "%s  %s", zmanStr, strings.ZomIn);
    mainScreen.update(mainScreen.infoRight, strAll, txtInfo);
    dayInfo.tzomOut.toString(zmanStr);
    sprintf(strAll, "%s  %s",zmanStr, strings.ZomOut);
    mainScreen.update(mainScreen.infoLeft, strAll, txtInfo);
}
bool MainState::flaginfoSet_1_Zom1()
{
    bool f = dayInfo.isZom || dayInfo.isErevZom;
    f = f || (dayInfo.isMozaeiZom && isTimeInRange(Zman(14,0), zmanPlusDakot(dayInfo.rabenuTam, 5)));
    f = f && userPreferences.moadim==1;
    return f || showAll;
}
void MainState::infoSet_1_Zom2()
{
    //Serial.println("infoSet_1_Zom2");
    // עננו / יעלה ויבוא
    if(dayInfo.anenu && userPreferences.tfila==1) { mainScreen.update(mainScreen.infoAll, strings.anenu, txtInfo); }
    if(dayInfo.yaaleVeyavo && userPreferences.tfila==1) { mainScreen.update(mainScreen.infoLeft, strings.yaaleveyavo, txtInfo); }
}
bool MainState::flaginfoSet_1_Zom2()
{
    bool f = dayInfo.isZom || dayInfo.isErevZom;
    f = f || (dayInfo.isMozaeiZom && isTimeInRange(Zman(14,0), zmanPlusDakot(dayInfo.rabenuTam, 5)));
    f = f && userPreferences.moadim==1;
    return f || showAll;
}
void MainState::infoSet_1_TishaBeav()
{
    //Serial.println("infoSet_1_TishaBeav");
    // צום תשעה באב
    // שנים לחורבן
    char shanim[50];
    dayInfo.getShanimLahurban(shanim);
    mainScreen.updateInfo(shanim);
}
bool MainState::flaginfoSet_1_TishaBeav()
{
    bool f = userPreferences.moadim==1;
    f = f && dayInfo.isTishaBeav;
    return f || showAll;
}

// ראש חודש
void MainState::infoSet_1_RoshHodesh()
{
    //Serial.println("infoSet_1_RoshHodesh");
    // 
    char roshHodesh[50];
    dayInfo.tomorrow->getRoshHodesh(roshHodesh);
    mainScreen.updateInfo(roshHodesh);
}
bool MainState::flaginfoSet_1_RoshHodesh()
{
    bool f = userPreferences.moadim==1;
    f = f && dayInfo.isRoshHodesh;
    return f || showAll;
}
void MainState::infoSet_1_RoshHodesh1()
{
    //Serial.println("infoSet_1_RoshHodesh1");
    //
    mainScreen.update(mainScreen.infoRight, strings.yaaleveyavo, txtInfo);
    if(dayInfo.halelDilug) {mainScreen.update(mainScreen.infoLeft, strings.halelBedilug, txtInfo);}
    if(dayInfo.halelShalem) {mainScreen.update(mainScreen.infoLeft, strings.halelShalem, txtInfo);}

}
bool MainState::flaginfoSet_1_RoshHodesh1()
{
    bool f = userPreferences.moadim==1;
    f = f && userPreferences.tfila==1;
    f = f && dayInfo.isRoshHodesh;
    return f || showAll;
}
// ראש חודש קרוב
void MainState::infoSet_1_NextRoshHodesh()
{
    // 
    char nextRoshHodeshUp[50];
    char nextRoshHodeshDown[50];
    dayInfo.getNextRoshHodesh(nextRoshHodeshUp, nextRoshHodeshDown);
    mainScreen.updateInfo(nextRoshHodeshDown, nextRoshHodeshUp);
}
bool MainState::flaginfoSet_1_NextRoshHodesh()
{
    bool f = userPreferences.moadim==1;
    f = f && userPreferences.tfila==1;
    f = f && dayInfo.isShabatMevarchin;
    f = f && dayInfo.isShabat;
    f = f && isTimeInRange(Zman(5,0),Zman(14,0));
    return f || showAll;
}

// מולד
void MainState::infoSet_1_Molad()
{
    char moladUp[50];
    char moladDown[50];
    dayInfo.getMolad(moladUp, moladDown);
    mainScreen.updateInfo(moladDown, moladUp);
}
bool MainState::flaginfoSet_1_Molad()
{
    bool f = userPreferences.moadim==1;
    f = f && userPreferences.tfila==1;
    f = f && dayInfo.isShabatMevarchin;
    f = f && dayInfo.isShabat;
    f = f && isTimeInRange(Zman(5,0),Zman(14,0));
    return f || showAll;
}
// מועד ישראלי
void MainState::infoSet_1_Israeli()
{
    //Serial.println("infoSet_1_Israeli");
    // יום העצמאות/השואה/הזיכרון/ירושלים
    char buffer[50];
    dayInfo.getHoliday(buffer);
    if(dayInfo.isAzmaut) 
    {
        char buffer1[50];
        dayInfo.getShanimLamdina(buffer1);
        mainScreen.updateInfo(buffer1, buffer); 
    }
    if(dayInfo.isYomYerushalaim) 
    {
        mainScreen.updateInfo(buffer); 
    }
    if(dayInfo.isYomHashoa) { mainScreen.updateInfo(strings.LashoaVelagvura, buffer); }
    if(dayInfo.isYomHazicaron) { mainScreen.updateInfo(strings.LehalleMaarchotIsrael, buffer); }
}
bool MainState::flaginfoSet_1_Israeli()
{
    bool f = userPreferences.showIsraeli==1;
    f = f && dayInfo.isHagIsraeli;
    return f || showAll;
}
// ספירת העומר
void MainState::infoSet_1_Omer()
{
    //Serial.println("infoSet_1_Omer");
    // היום ... ימים לעומר
    char line1[50];
    char line2[50];
    dayInfo.getOmer(line1, line2);
    mainScreen.updateInfo(line1);
}
bool MainState::flaginfoSet_1_Omer()
{
    bool f = dayInfo.isOmer;
    return f || showAll;
}
void MainState::infoSet_1_Omer1()
{
    //Serial.println("infoSet_1_Omer");
    // היום ... ימים לעומר
    char line1[50];
    char line2[50];
    dayInfo.getOmer(line1, line2);
    mainScreen.updateInfo(line2);
}
bool MainState::flaginfoSet_1_Omer1()
{
    bool f = dayInfo.isOmer;
    f && dayInfo.omer >= 7;
    return f || showAll;
}
// סוף זמן חמץ
void MainState::infoSet_1_Hametz()
{
    //Serial.println("infoSet_1_Hametz");
    // סוף זמן אכילת חמץ
    char strAll[100];
    sprintf(strAll, "%s :%s",dayInfo.sofZmanTfilaMga, strings.sofZmanAchilatHametz);
    mainScreen.updateInfo(strAll);
}
bool MainState::flaginfoSet_1_Hametz()
{
    bool f = userPreferences.moadim==1;
    f = f && dayInfo.tomorrow->isPesach;
    f = f && isTimeInRange(Zman(1,0), Zman(13,0));
    return f || showAll;
}
void MainState::infoSet_1_Hametz1()
{
    //Serial.println("infoSet_1_Hametz1");
    // סוף זמן ביעור חמץ
    char strAll[100];
    sprintf(strAll, "%s :%s",dayInfo.sofZmanBeiurHamez, strings.sofZmanBeiurHametz);
    mainScreen.updateInfo(strAll);
}
bool MainState::flaginfoSet_1_Hametz1()
{
    bool f = userPreferences.moadim==1;
    f = f && dayInfo.tomorrow->isPesach;
    f = f && isTimeInRange(Zman(1,0), Zman(13,0));
    return f || showAll;
}
// חנוכה
void MainState::infoSet_1_Hanuka()
{
    //Serial.println("infoSet_1_Hanuka");
    // נר [יום] של חנוכה
    char hanuka[50];
    dayInfo.getHanuka(hanuka);
    mainScreen.updateInfo(hanuka);
}
bool MainState::flaginfoSet_1_Hanuka()
{
    bool f = userPreferences.moadim==1;
    f = f && dayInfo.isHanuka;
    return f || showAll;
}
void MainState::infoSet_1_Hanuka1()
{
    //Serial.println("infoSet_1_Hanuka1");
    // על הניסים - הלל שלם
    mainScreen.update(mainScreen.infoRight, strings.alHanisim, txtInfo); 
    mainScreen.update(mainScreen.infoLeft, strings.halelShalem, txtInfo); 
}
bool MainState::flaginfoSet_1_Hanuka1()
{
    bool f = userPreferences.moadim==1;
    f = f && userPreferences.tfila==1;
    f = f && dayInfo.isHanuka;
    return f || showAll;
}
// עשרת ימי תשובה
void MainState::infoSet_1_AseretYemeTshuva()
{
    //Serial.println("infoSet_1_AseretYemeTshuva");
    // עשרת ימי תשובה
    if(dayInfo.hamelechHamishpat)
    {
        mainScreen.update(mainScreen.infoAll, strings.aseretYemeTshuva, txtInfo);
    }
    else{
        mainScreen.update(mainScreen.infoRight, strings.aseretYemeTshuva, txtInfo);
        mainScreen.update(mainScreen.infoLeft, strings.hamelechHakadosh, txtInfo);
    }
}
bool MainState::flaginfoSet_1_AseretYemeTshuva()
{
    bool f = userPreferences.tfila==1;
    f = f && dayInfo.hamelechHakadosh;
    return f || showAll;
}
// עשרת ימי תשובה
void MainState::infoSet_1_AseretYemeTshuva1()
{
    //Serial.println("infoSet_1_AseretYemeTshuva1");
    // המלך הקדוש - המלך המשפט
    mainScreen.update(mainScreen.infoRight, strings.hamelechHakadosh, txtInfo);
    mainScreen.update(mainScreen.infoLeft, strings.hamelechHamishpat, txtInfo);
}
bool MainState::flaginfoSet_1_AseretYemeTshuva1()
{
    bool f = userPreferences.tfila==1;
    f = f && dayInfo.hamelechHakadosh;
    f = f && dayInfo.hamelechHamishpat;
    return f || showAll;
}
// פורים
void MainState::infoSet_1_Purim()
{
    //Serial.println("infoSet_1_Purim");
    // פורים שמח
    // על הניסים
    if(userPreferences.tfila==1) { 
        mainScreen.update(mainScreen.infoRight, strings.purimSameah, txtInfo); 
        mainScreen.update(mainScreen.infoLeft, strings.alHanisim, txtInfo); 
    }
    else { mainScreen.update(mainScreen.infoAll, strings.purimSameah, txtInfo); }
}
bool MainState::flaginfoSet_1_Purim()
{
    bool f = userPreferences.moadim==1;
    f = f && ((userPreferences.homa==1 && dayInfo.isShushanPurim)||(userPreferences.homa==0 && dayInfo.isPurim));
    return f || showAll;
}
// דף היומי בבלי
void MainState::infoSet_1_Bavli()
{
    //Serial.println("infoSet_1_Bavli");
    char dafStr[100];
    dayInfo.dafYomiBavli.toString(dafStr, 1);
    mainScreen.updateInfo(dafStr);
}
bool MainState::flaginfoSet_1_Bavli()
{
    bool f = userPreferences.dafHayomi==1;
    return f || showAll;
}
// דף היומי ירושלמי
void MainState::infoSet_1_Yerushalmi()
{
    //Serial.println("infoSet_1_Yerushalmi");
    char dafStr[50];
    dayInfo.dafYomiYerushalmi.toString(dafStr, 2);
    mainScreen.updateInfo(dafStr);
}
bool MainState::flaginfoSet_1_Yerushalmi()
{
    bool f = userPreferences.yerushalmi==1;
    return f || showAll;
}

// הפטרה
void MainState::infoSet_1_Haftara()
{
    //Serial.println("infoSet_1_Haftara");
    char shem[50];
    char makor[50];
    dayInfo.getHaftara(shem, makor);
    Text shem_txt = Text(mainScreen.colorUp, &ariel10, 0, 13, LEFT);
    Text makor_txt = Text(mainScreen.colorUp, &font9, 0, 13, LEFT); 
    uint16_t shem_w = mainScreen.infoAll->calculateTextLength(shem, shem_txt);
    uint16_t makor_w = mainScreen.infoAll->calculateTextLength(makor, makor_txt);
    int8_t space = 10;
    uint16_t shem_makor_w = shem_w + makor_w + space;
    int16_t blank = mainScreen.infoAll->width - shem_makor_w;

    if (blank <= 1)
    {
        mainScreen.updateInfo(makor, shem);
    }
    else
    {
        uint16_t makor_margin = blank/2;
        makor_txt.marginX = makor_margin;
        uint16_t shem_margin = makor_margin + makor_w + space;
        shem_txt.marginX = shem_margin;

        mainScreen.infoAll->clearArea();
        mainScreen.infoAll->addText(makor, makor_txt);
        mainScreen.infoAll->addText(shem, shem_txt);
    }
}
bool MainState::flaginfoSet_1_Haftara()
{
    bool f = dayInfo.isShabat || dayInfo.isHag || dayInfo.isYomKipur || dayInfo.isTishaBeav;
    f = f || (dayInfo.isZom && (userPreferences.eda == ASHKENAZ || userPreferences.eda == CHABAD || userPreferences.eda == SFARAD));
    f = f || (userPreferences.showIsraeli==1 && dayInfo.isAzmaut);
    f = f && userPreferences.haftara==1;
    return f || showAll;
}

// ימי חב"ד
void MainState::infoSet_1_Chabad()
{
    //Serial.println("infoSet_1_Chabad");
    char chabadStr[40];
    if(dayInfo.yom_chabad >= 20)
    {
        char chabadStr1[40];
        uint8_t idx = dayInfo.yom_chabad;
        idx = (idx-20)*2+20;
        dayInfo.getYomHabad(chabadStr, idx);
        dayInfo.getYomHabad(chabadStr1, idx+1);
        mainScreen.updateInfo(chabadStr, chabadStr1);
    }
    else
    {
        dayInfo.getYomHabad(chabadStr, dayInfo.yom_chabad);
        mainScreen.updateInfo(chabadStr);
    }
}
bool MainState::flaginfoSet_1_Chabad()
{
    bool f = userPreferences.eda==6;
    f = f && dayInfo.isYomChabad;
    return f || showAll;
}
