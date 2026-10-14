#include "Logic.h"
#include "DB/Place.h"
#include "BLE/TextCommandHandler.h"

MainScreen mainScreen;
MenuScreen menuScreen;
BrightnessScreen brightnessScreen;
ColorScreen colorScreen;
AboutScreen aboutScreen;
LogoScreen logoScreen;
VersionScreen versionScreen;
RegsScreen regsScreen;
SetClockScreen setClockScreen;
ResetScreen resetScreen;
TextScreen textScreen;
MessageScreen messageScreen;

DayInfo dayInfo;
DayInfo dayInfoYesterday;
DayInfo dayInfoTomorrow;
DayInfo dayInfoNextShishi;
DayInfo dayInfoNextShabat;

extern GFXfont *font_list[2];
extern int lonPre;
extern int lonInt;
extern int lonFrac;
extern int latPre;
extern int latInt;
extern int latFrac;


void Logic::init()
{
    configStates();
    initStates();
}
void Logic::configStates()
{
    mainScreen.colorUp = color_list[userPreferences.colorUp];
    mainScreen.colorMiddle = color_list[userPreferences.colorMiddle];
    mainScreen.colorBottomT = color_list[userPreferences.colorBottomT];
    mainScreen.colorBottomV = color_list[userPreferences.colorBottomV];
    mainScreen.colorYom = color_list[userPreferences.colorYom];
    mainScreen.colorParasha = color_list[userPreferences.colorParasha];
    mainScreen.colorIluy = color_list[userPreferences.colorIluy];
    mainScreen.clockFont = font_list[userPreferences.clockFont];
    mainState.setText();
    this->setColorUpLine.colorTemp = userPreferences.colorUp;
    this->setColorMiddleLine.colorTemp = userPreferences.colorMiddle;
    this->setColorBottomTLine.colorTemp = userPreferences.colorBottomT;
    this->setColorBottomVLine.colorTemp = userPreferences.colorBottomV;
    this->setColorYomLine.colorTemp = userPreferences.colorYom;
    this->setColorParashaLine.colorTemp = userPreferences.colorParasha;
    this->setColorIluyLine.colorTemp = userPreferences.colorIluy;
    mainState.intervalTimeSet = userPreferences.intervalTime;
    mainState.intervalInfoSet = userPreferences.intervalInfo;
    mainState.intervalNameSet = userPreferences.intervalIluy;
    mainState.initTimeSetFuncList();
    mainState.initInfoSetFuncList();
    mainState.showTimeToNetz = userPreferences.netzClk != 0;
    mainState.numOfValidTexts = handleGetTextCount().data1;
    this->setBrightnessState.brightTemp = userPreferences.brightness;
    this->setOfekState.ofekTemp = userPreferences.ofek;
    this->showTime.showTemp = 1;
    this->setZrihaState.zrihaTemp = userPreferences.zriha;
    this->setShkiaaState.shkiaaTemp = userPreferences.shkiaa;
    this->setZeetCochavimState.zeetTemp = userPreferences.zeetCochavim;
    this->setRabenuTamState.rtTemp = userPreferences.rabenuTam;
    this->setTalitState.talitTemp = userPreferences.zmanTalit;
    this->setAlotState.alotTemp = userPreferences.alotHashahar;
    this->setSozksMgaState.sozksTemp = userPreferences.sozksMga;
    this->setSoztMgaState.soztTemp = userPreferences.soztMga;
    this->setMinhaGState.minhaTemp = userPreferences.minhaG;
    this->setPlagState.plagTemp = userPreferences.plagMinha;
    this->setZeetShabatState.zeetTemp = userPreferences.zeetShabat;
    this->setZeetZomState.zeetTemp = userPreferences.zeetZom;
    this->shitaState.shitaTemp = userPreferences.shita;
    this->setTohenInfoStateKezevSet.kezevTemp = userPreferences.intervalInfo;
    this->setTohenIluyStateKezevSet.kezevTemp = userPreferences.intervalIluy;
    this->setTohenTimeStateKezevSet.kezevTemp = userPreferences.intervalTime;
    this->minhagNerotSetState.nerotTemp = userPreferences.nerot;
    this->setFontState.fontTemp = userPreferences.clockFont;
    this->setTohenBottomLineNetzSet.netzTemp = userPreferences.netzClk;
    this->minhagShanimSetState.shanimTemp = userPreferences.shanim;
    this->setShaonKaiz.kaizTemp = userPreferences.shaonKaiz;
    this->setClkFormat.formatTemp = userPreferences.clockFormat;
    this->setDateFormat.formatTemp = userPreferences.dateFormat;
    this->setTimeZoneCustomState.tempTZ = userPreferences.timezone;
    this->setGovaCustomState.tempGova = userPreferences.nz_height;
    this->setLonPre.tempPre = lonPre;
    this->setLonInt.tempInt = lonInt;
    this->setLonFrac.tempFrac = lonFrac;
    this->setLatPre.tempPre = latPre;
    this->setLatInt.tempInt = latInt;
    this->setLatFrac.tempFrac = latFrac;
}
void Logic::initStates()
{
    mainState.setMenu(&menuClockState);

    menuClockState.setMenu(&mainState);
    menuTzugaState.setMenu(&mainState);
    menuEddotState.setMenu(&mainState);
    menuOffekState.setMenu(&mainState);
    menuMinagState.setMenu(&mainState);
    menuTohenState.setMenu(&mainState);
    menuConfgState.setMenu(&mainState);

    menuClockState.setUp(&menuConfgState);
    menuTzugaState.setUp(&menuClockState);
    menuEddotState.setUp(&menuTzugaState);
    menuOffekState.setUp(&menuEddotState);
    menuMinagState.setUp(&menuOffekState);
    menuTohenState.setUp(&menuMinagState);
    menuConfgState.setUp(&menuTohenState);

    menuClockState.setDown(&menuTzugaState);
    menuTzugaState.setDown(&menuEddotState);
    menuEddotState.setDown(&menuOffekState);
    menuOffekState.setDown(&menuMinagState);
    menuMinagState.setDown(&menuTohenState);
    menuTohenState.setDown(&menuConfgState);
    menuConfgState.setDown(&menuClockState);

    menuClockState.setOk(&clockSetTime);
    menuTzugaState.setOk(&brightnessState);
    menuEddotState.setOk(&edotMizrah);
    menuOffekState.setOk(&ofekState);
    menuMinagState.setOk(&minhagShitaState);
    menuTohenState.setOk(&tohenIluyState);
    menuConfgState.setOk(&bTState);
    
    clockSetTime.setMenu(&menuClockState);
    clkFormat.setMenu(&menuClockState);
    dateFormat.setMenu(&menuClockState);
    timeFormat.setMenu(&menuClockState);
    minhagLeilState.setMenu(&menuClockState);
    shaonKaiz.setMenu(&menuClockState);

    clockSetTime.setUp(&shaonKaiz);
    clkFormat.setUp(&clockSetTime);
    dateFormat.setUp(&clkFormat);
    timeFormat.setUp(&dateFormat);
    minhagLeilState.setUp(&timeFormat);
    shaonKaiz.setUp(&minhagLeilState);

    clockSetTime.setDown(&clkFormat);
    clkFormat.setDown(&dateFormat);
    dateFormat.setDown(&timeFormat);
    timeFormat.setDown(&minhagLeilState);
    minhagLeilState.setDown(&shaonKaiz);
    shaonKaiz.setDown(&clockSetTime);

    clockSetTime.setOk(&setHourState);
    clkFormat.setOk(&setClkFormat);
    dateFormat.setOk(&setDateFormat);
    timeFormat.setOk(&setTimeFormat);
    minhagLeilState.setOk(&minhagLeilState);
    shaonKaiz.setOk(&setShaonKaiz);

    setShaonKaiz.setMenu(&shaonKaiz);
    setShaonKaiz.setUp(&setShaonKaiz);
    setShaonKaiz.setDown(&setShaonKaiz);
    setShaonKaiz.setOk(&shaonKaiz);

    setClkFormat.setMenu(&clkFormat);
    setClkFormat.setUp(&setClkFormat);
    setClkFormat.setDown(&setClkFormat);
    setClkFormat.setOk(&clkFormat);

    setDateFormat.setMenu(&dateFormat);
    setDateFormat.setUp(&setDateFormat);
    setDateFormat.setDown(&setDateFormat);
    setDateFormat.setOk(&dateFormat);

    setTimeFormat.setMenu(&timeFormat);
    setTimeFormat.setUp(&setTimeFormat);
    setTimeFormat.setDown(&setTimeFormat);
    setTimeFormat.setOk(&timeFormat);

    setHourState.setMenu(&clockSetTime);
    setMinuteState.setMenu(&clockSetTime);
    setSecondState.setMenu(&clockSetTime);
    setDayState.setMenu(&clockSetTime);
    setMonthState.setMenu(&clockSetTime);
    setYearState.setMenu(&clockSetTime);

    setHourState.setUp(&setHourState);
    setMinuteState.setUp(&setMinuteState);
    setSecondState.setUp(&setSecondState);
    setDayState.setUp(&setDayState);
    setMonthState.setUp(&setMonthState);
    setYearState.setUp(&setYearState);

    setHourState.setDown(&setHourState);
    setMinuteState.setDown(&setMinuteState);
    setSecondState.setDown(&setSecondState);
    setDayState.setDown(&setDayState);
    setMonthState.setDown(&setMonthState);
    setYearState.setDown(&setYearState);

    setHourState.setOk(&setMinuteState);
    setMinuteState.setOk(&setSecondState);
    setSecondState.setOk(&setDayState);
    setDayState.setOk(&setMonthState);
    setMonthState.setOk(&setYearState);
    setYearState.setOk(&setHourState);

    colorState.setMenu(&menuTzugaState);
    brightnessState.setMenu(&menuTzugaState);
    fontState.setMenu(&menuTzugaState);

    colorState.setUp(&fontState);
    brightnessState.setUp(&colorState);
    fontState.setUp(&brightnessState);

    colorState.setDown(&brightnessState);
    brightnessState.setDown(&fontState);
    fontState.setDown(&colorState);

    colorState.setOk(&colorUpLine);
    brightnessState.setOk(&setBrightnessState);
    fontState.setOk(&setFontState);

    setFontState.setMenu(&fontState);
    setFontState.setUp(&setFontState);
    setFontState.setDown(&setFontState);
    setFontState.setOk(&fontState);

    setBrightnessState.setMenu(&brightnessState);
    setBrightnessState.setUp(&setBrightnessState);
    setBrightnessState.setDown(&setBrightnessState);
    setBrightnessState.setOk(&brightnessState);

    colorUpLine.setMenu(&colorState);
    colorMiddleLine.setMenu(&colorState);
    colorBottomTLine.setMenu(&colorState);
    colorBottomVLine.setMenu(&colorState);
    colorYomLine.setMenu(&colorState);
    colorParashaLine.setMenu(&colorState);
    colorIluyLine.setMenu(&colorState);

    colorUpLine.setUp(&colorBottomVLine);
    colorMiddleLine.setUp(&colorYomLine);
    colorBottomTLine.setUp(&colorParashaLine);
    colorBottomVLine.setUp(&colorBottomTLine);
    colorYomLine.setUp(&colorIluyLine);
    colorParashaLine.setUp(&colorMiddleLine);
    colorIluyLine.setUp(&colorUpLine);

    colorUpLine.setDown(&colorIluyLine);
    colorMiddleLine.setDown(&colorParashaLine);
    colorBottomTLine.setDown(&colorBottomVLine);
    colorBottomVLine.setDown(&colorUpLine);
    colorYomLine.setDown(&colorMiddleLine);
    colorParashaLine.setDown(&colorBottomTLine);
    colorIluyLine.setDown(&colorYomLine);

    colorUpLine.setOk(&setColorUpLine);
    colorMiddleLine.setOk(&setColorMiddleLine);
    colorBottomTLine.setOk(&setColorBottomTLine);
    colorBottomVLine.setOk(&setColorBottomVLine);
    colorYomLine.setOk(&setColorYomLine);
    colorParashaLine.setOk(&setColorParashaLine);
    colorIluyLine.setOk(&setColorIluyLine);

    setColorUpLine.setMenu(&colorUpLine);
    setColorUpLine.setUp(&setColorUpLine);
    setColorUpLine.setDown(&setColorUpLine);
    setColorUpLine.setOk(&colorUpLine);

    setColorMiddleLine.setMenu(&colorMiddleLine);
    setColorMiddleLine.setUp(&setColorMiddleLine);
    setColorMiddleLine.setDown(&setColorMiddleLine);
    setColorMiddleLine.setOk(&colorMiddleLine);

    setColorBottomTLine.setMenu(&colorBottomTLine);
    setColorBottomTLine.setUp(&setColorBottomTLine);
    setColorBottomTLine.setDown(&setColorBottomTLine);
    setColorBottomTLine.setOk(&colorBottomTLine);

    setColorBottomVLine.setMenu(&colorBottomVLine);
    setColorBottomVLine.setUp(&setColorBottomVLine);
    setColorBottomVLine.setDown(&setColorBottomVLine);
    setColorBottomVLine.setOk(&colorBottomVLine);

    setColorYomLine.setMenu(&colorYomLine);
    setColorYomLine.setUp(&setColorYomLine);
    setColorYomLine.setDown(&setColorYomLine);
    setColorYomLine.setOk(&colorYomLine);

    setColorParashaLine.setMenu(&colorParashaLine);
    setColorParashaLine.setUp(&setColorParashaLine);
    setColorParashaLine.setDown(&setColorParashaLine);
    setColorParashaLine.setOk(&colorParashaLine);

    setColorIluyLine.setMenu(&colorIluyLine);
    setColorIluyLine.setUp(&setColorIluyLine);
    setColorIluyLine.setDown(&setColorIluyLine);
    setColorIluyLine.setOk(&colorIluyLine);

    bTState.setMenu(&menuConfgState);
    resetPrefState.setMenu(&menuConfgState);
    resetTextState.setMenu(&menuConfgState);
    resetPassState.setMenu(&menuConfgState);
    aboutUsState.setMenu(&menuConfgState);
    aboutDeviceState.setMenu(&menuConfgState);
    memoryMapState.setMenu(&menuConfgState);

    bTState.setUp(&memoryMapState);
    resetPrefState.setUp(&bTState);
    resetTextState.setUp(&resetPrefState);
    resetPassState.setUp(&resetTextState);
    aboutUsState.setUp(&resetPassState);
    aboutDeviceState.setUp(&aboutUsState);
    memoryMapState.setUp(&aboutDeviceState);

    bTState.setDown(&resetPrefState);
    resetPrefState.setDown(&resetTextState);
    resetTextState.setDown(&resetPassState);
    resetPassState.setDown(&aboutUsState);
    aboutUsState.setDown(&aboutDeviceState);
    aboutDeviceState.setDown(&memoryMapState);
    memoryMapState.setDown(&bTState);

    bTState.setOk(&setBTState);
    resetPrefState.setOk(&setResetPrefState);
    resetTextState.setOk(&setResetTextState);
    resetPassState.setOk(&setResetPassState);
    aboutUsState.setOk(&viewAboutUsState);
    aboutDeviceState.setOk(&viewDeviceState);
    memoryMapState.setOk(&viewMemoryMapState);
    
    setBTState.setMenu(&bTState);
    setBTState.setUp(&setBTState);
    setBTState.setDown(&setBTState);
    setBTState.setOk(&bTState);

    viewAboutUsState.setMenu(&aboutUsState);
    viewAboutUsState.setUp(&viewAboutUsState);
    viewAboutUsState.setDown(&viewAboutUsState);
    viewAboutUsState.setOk(&aboutUsState);

    viewDeviceState.setMenu(&aboutDeviceState);
    viewDeviceState.setUp(&viewDeviceState);
    viewDeviceState.setDown(&viewDeviceState);
    viewDeviceState.setOk(&aboutDeviceState);

    viewMemoryMapState.setMenu(&memoryMapState);
    viewMemoryMapState.setUp(&viewMemoryMapState);
    viewMemoryMapState.setDown(&viewMemoryMapState);
    viewMemoryMapState.setOk(&memoryMapState);

    setResetPrefState.setMenu(&resetPrefState);
    setResetPrefState.setUp(&setResetPrefState);
    setResetPrefState.setDown(&setResetPrefState);
    setResetPrefState.setOk(&resetPrefState);

    setResetTextState.setMenu(&resetTextState);
    setResetTextState.setUp(&setResetTextState);
    setResetTextState.setDown(&setResetTextState);
    setResetTextState.setOk(&resetTextState);

    setResetPassState.setMenu(&resetPassState);
    setResetPassState.setUp(&setResetPassState);
    setResetPassState.setDown(&setResetPassState);
    setResetPassState.setOk(&resetPassState);

    ofekState.setMenu(&menuOffekState);
    customAreaState.setMenu(&menuOffekState);
    setHomaCustomState.setMenu(&menuOffekState);

    ofekState.setUp(&setHomaCustomState);
    customAreaState.setUp(&ofekState);
    setHomaCustomState.setUp(&customAreaState);

    ofekState.setDown(&customAreaState);
    customAreaState.setDown(&setHomaCustomState);
    setHomaCustomState.setDown(&ofekState);

    ofekState.setOk(&setOfekState);
    customAreaState.setOk(&lonCustomState);
    setHomaCustomState.setOk(&setHomaCustomState);

    setOfekState.setMenu(&ofekState);
    setOfekState.setUp(&setOfekState);
    setOfekState.setDown(&setOfekState);
    setOfekState.setOk(&ofekState);

    lonCustomState.setMenu(&customAreaState);
    latCustomState.setMenu(&customAreaState);
    govaCustomState.setMenu(&customAreaState);
    setHulCustomState.setMenu(&customAreaState);
    timeZoneCustomState.setMenu(&customAreaState);

    lonCustomState.setUp(&timeZoneCustomState);
    latCustomState.setUp(&lonCustomState);
    govaCustomState.setUp(&latCustomState);
    setHulCustomState.setUp(&govaCustomState);
    timeZoneCustomState.setUp(&setHulCustomState);

    lonCustomState.setDown(&latCustomState);
    latCustomState.setDown(&govaCustomState);
    govaCustomState.setDown(&setHulCustomState);
    setHulCustomState.setDown(&timeZoneCustomState);
    timeZoneCustomState.setDown(&lonCustomState);

    lonCustomState.setOk(&setLonPre);
    latCustomState.setOk(&setLatPre);
    govaCustomState.setOk(&setGovaCustomState);
    setHulCustomState.setOk(&setHulCustomState);
    timeZoneCustomState.setOk(&setTimeZoneCustomState);

    setLonPre.setMenu(&lonCustomState);
    setLonInt.setMenu(&lonCustomState);
    setLonFrac.setMenu(&lonCustomState);

    setLonPre.setUp(&setLonPre);
    setLonInt.setUp(&setLonInt);
    setLonFrac.setUp(&setLonFrac);

    setLonPre.setDown(&setLonPre);
    setLonInt.setDown(&setLonInt);
    setLonFrac.setDown(&setLonFrac);

    setLonPre.setOk(&setLonInt);
    setLonInt.setOk(&setLonFrac);
    setLonFrac.setOk(&setLonPre);

    setLatPre.setMenu(&latCustomState);
    setLatInt.setMenu(&latCustomState);
    setLatFrac.setMenu(&latCustomState);

    setLatPre.setUp(&setLatPre);
    setLatInt.setUp(&setLatInt);
    setLatFrac.setUp(&setLatFrac);

    setLatPre.setDown(&setLatPre);
    setLatInt.setDown(&setLatInt);
    setLatFrac.setDown(&setLatFrac);

    setLatPre.setOk(&setLatInt);
    setLatInt.setOk(&setLatFrac);
    setLatFrac.setOk(&setLatPre);

    setGovaCustomState.setMenu(&govaCustomState);
    setGovaCustomState.setUp(&setGovaCustomState);
    setGovaCustomState.setDown(&setGovaCustomState);
    setGovaCustomState.setOk(&govaCustomState);

    setTimeZoneCustomState.setMenu(&timeZoneCustomState);
    setTimeZoneCustomState.setUp(&setTimeZoneCustomState);
    setTimeZoneCustomState.setDown(&setTimeZoneCustomState);
    setTimeZoneCustomState.setOk(&timeZoneCustomState);

    edotMizrah.setMenu(&menuEddotState);
    edotSfarad.setMenu(&menuEddotState);
    edotAshkenaz.setMenu(&menuEddotState);
    edotTeimanBaladi.setMenu(&menuEddotState);
    edotTeimanShami.setMenu(&menuEddotState);
    edotChabad.setMenu(&menuEddotState);
    edotBneiHul.setMenu(&menuEddotState);

    edotMizrah.setUp(&edotBneiHul);
    edotSfarad.setUp(&edotMizrah);
    edotAshkenaz.setUp(&edotSfarad);
    edotTeimanBaladi.setUp(&edotAshkenaz);
    edotTeimanShami.setUp(&edotTeimanBaladi);
    edotChabad.setUp(&edotTeimanShami);
    edotBneiHul.setUp(&edotChabad);

    edotMizrah.setDown(&edotSfarad);
    edotSfarad.setDown(&edotAshkenaz);
    edotAshkenaz.setDown(&edotTeimanBaladi);
    edotTeimanBaladi.setDown(&edotTeimanShami);
    edotTeimanShami.setDown(&edotChabad);
    edotChabad.setDown(&edotBneiHul);
    edotBneiHul.setDown(&edotMizrah);

    edotMizrah.setOk(&edotMizrah);
    edotSfarad.setOk(&edotSfarad);
    edotAshkenaz.setOk(&edotAshkenaz);
    edotTeimanBaladi.setOk(&edotTeimanBaladi);
    edotTeimanShami.setOk(&edotTeimanShami);
    edotChabad.setOk(&edotChabad);
    edotBneiHul.setOk(&edotBneiHul);

    minhagShitaState.setMenu(&menuMinagState);
    minhagZrihaState.setMenu(&menuMinagState);
    minhagShkiaaState.setMenu(&menuMinagState);
    zeetCochavimState.setMenu(&menuMinagState);
    rabenuTamState.setMenu(&menuMinagState);
    alotState.setMenu(&menuMinagState);
    talitState.setMenu(&menuMinagState);
    sozksMgaState.setMenu(&menuMinagState);
    soztMgaState.setMenu(&menuMinagState);
    minhaGState.setMenu(&menuMinagState);
    plagState.setMenu(&menuMinagState);
    zeetShabatState.setMenu(&menuMinagState);
    zeetZomState.setMenu(&menuMinagState);
    minhagShanimState.setMenu(&menuMinagState);
    minhagNerotState.setMenu(&menuMinagState);
    showTimeState.setMenu(&menuMinagState);

    minhagShitaState.setUp(&showTimeState);
    minhagZrihaState.setUp(&minhagShitaState);
    minhagShkiaaState.setUp(&minhagZrihaState);
    zeetCochavimState.setUp(&minhagShkiaaState);
    rabenuTamState.setUp(&zeetCochavimState);
    alotState.setUp(&rabenuTamState);
    talitState.setUp(&alotState);
    sozksMgaState.setUp(&talitState);
    soztMgaState.setUp(&sozksMgaState);
    minhaGState.setUp(&soztMgaState);
    plagState.setUp(&minhaGState);
    zeetShabatState.setUp(&plagState);
    zeetZomState.setUp(&zeetShabatState);
    minhagShanimState.setUp(&zeetZomState);
    minhagNerotState.setUp(&minhagShanimState);
    showTimeState.setUp(&minhagNerotState);

    minhagShitaState.setDown(&minhagZrihaState);
    minhagZrihaState.setDown(&minhagShkiaaState);
    minhagShkiaaState.setDown(&zeetCochavimState);
    zeetCochavimState.setDown(&rabenuTamState);
    rabenuTamState.setDown(&alotState);
    alotState.setDown(&talitState);
    talitState.setDown(&sozksMgaState);
    sozksMgaState.setDown(&soztMgaState);
    soztMgaState.setDown(&minhaGState);
    minhaGState.setDown(&plagState);
    plagState.setDown(&zeetShabatState);
    zeetShabatState.setDown(&zeetZomState);
    zeetZomState.setDown(&minhagShanimState);
    minhagShanimState.setDown(&minhagNerotState);
    minhagNerotState.setDown(&showTimeState);
    showTimeState.setDown(&minhagShitaState);

    minhagShitaState.setOk(&shitaState);
    minhagZrihaState.setOk(&setZrihaState);
    minhagShkiaaState.setOk(&setShkiaaState);
    zeetCochavimState.setOk(&setZeetCochavimState);
    rabenuTamState.setOk(&setRabenuTamState);
    alotState.setOk(&setAlotState);
    talitState.setOk(&setTalitState);
    sozksMgaState.setOk(&setSozksMgaState);
    soztMgaState.setOk(&setSoztMgaState);
    minhaGState.setOk(&setMinhaGState);
    plagState.setOk(&setPlagState);
    zeetShabatState.setOk(&setZeetShabatState);
    zeetZomState.setOk(&setZeetZomState);
    minhagShanimState.setOk(&minhagShanimSetState);
    minhagNerotState.setOk(&minhagNerotSetState);
    showTimeState.setOk(&showTime);

    showTime.setMenu(&showTimeState);
    showTime.setUp(&showTime);
    showTime.setDown(&showTime);
    showTime.setOk(&showTimeState);

    setZrihaState.setMenu(&minhagZrihaState);
    setZrihaState.setUp(&setZrihaState);
    setZrihaState.setDown(&setZrihaState);
    setZrihaState.setOk(&minhagZrihaState);

    setShkiaaState.setMenu(&minhagShkiaaState);
    setShkiaaState.setUp(&setShkiaaState);
    setShkiaaState.setDown(&setShkiaaState);
    setShkiaaState.setOk(&minhagShkiaaState);

    minhagNerotSetState.setMenu(&minhagNerotState);
    minhagNerotSetState.setUp(&minhagNerotSetState);
    minhagNerotSetState.setDown(&minhagNerotSetState);
    minhagNerotSetState.setOk(&minhagNerotState);

    minhagShanimSetState.setMenu(&minhagShanimState);
    minhagShanimSetState.setUp(&minhagShanimSetState);
    minhagShanimSetState.setDown(&minhagShanimSetState);
    minhagShanimSetState.setOk(&minhagShanimState);

    setZeetCochavimState.setMenu(&zeetCochavimState);
    setZeetCochavimState.setUp(&setZeetCochavimState);
    setZeetCochavimState.setDown(&setZeetCochavimState);
    setZeetCochavimState.setOk(&zeetCochavimState);

    setRabenuTamState.setMenu(&rabenuTamState);
    setRabenuTamState.setUp(&setRabenuTamState);
    setRabenuTamState.setDown(&setRabenuTamState);
    setRabenuTamState.setOk(&rabenuTamState);

    setAlotState.setMenu(&alotState);
    setAlotState.setUp(&setAlotState);
    setAlotState.setDown(&setAlotState);
    setAlotState.setOk(&alotState);

    setTalitState.setMenu(&talitState);
    setTalitState.setUp(&setTalitState);
    setTalitState.setDown(&setTalitState);
    setTalitState.setOk(&talitState);

    setSozksMgaState.setMenu(&sozksMgaState);
    setSozksMgaState.setUp(&setSozksMgaState);
    setSozksMgaState.setDown(&setSozksMgaState);
    setSozksMgaState.setOk(&sozksMgaState);

    setSoztMgaState.setMenu(&soztMgaState);
    setSoztMgaState.setUp(&setSoztMgaState);
    setSoztMgaState.setDown(&setSoztMgaState);
    setSoztMgaState.setOk(&soztMgaState);

    setMinhaGState.setMenu(&minhaGState);
    setMinhaGState.setUp(&setMinhaGState);
    setMinhaGState.setDown(&setMinhaGState);
    setMinhaGState.setOk(&minhaGState);

    setPlagState.setMenu(&plagState);
    setPlagState.setUp(&setPlagState);
    setPlagState.setDown(&setPlagState);
    setPlagState.setOk(&plagState);

    setZeetShabatState.setMenu(&zeetShabatState);
    setZeetShabatState.setUp(&setZeetShabatState);
    setZeetShabatState.setDown(&setZeetShabatState);
    setZeetShabatState.setOk(&zeetShabatState);

    setZeetZomState.setMenu(&zeetZomState);
    setZeetZomState.setUp(&setZeetZomState);
    setZeetZomState.setDown(&setZeetZomState);
    setZeetZomState.setOk(&zeetZomState);

    shitaState.setMenu(&minhagShitaState);
    shitaState.setUp(&shitaState);
    shitaState.setDown(&shitaState);
    shitaState.setOk(&minhagShitaState);

    tohenIluyState.setMenu(&menuTohenState);
    tohenTimeState.setMenu(&menuTohenState);
    tohenInfoState.setMenu(&menuTohenState);

    tohenIluyState.setDown(&tohenTimeState);
    tohenTimeState.setDown(&tohenInfoState);
    tohenInfoState.setDown(&tohenIluyState);

    tohenIluyState.setUp(&tohenInfoState);
    tohenTimeState.setUp(&tohenIluyState);
    tohenInfoState.setUp(&tohenTimeState);

    tohenIluyState.setOk(&setTohenIluyStateKezev);
    tohenTimeState.setOk(&setTohenTimeStateKezev);
    tohenInfoState.setOk(&setTohenInfoStateKezev);

    setTohenIluyStateKezev.setMenu(&tohenIluyState);
    setTohenIluyStateKezev.setUp(&setTohenIluyStateText);
    setTohenIluyStateKezev.setDown(&setTohenIluyStateText);
    setTohenIluyStateKezev.setOk(&setTohenIluyStateKezevSet);

    // setTohenIluyStateInsert.setMenu(&tohenIluyState);
    // setTohenIluyStateInsert.setUp(&setTohenIluyStateKezev);
    // setTohenIluyStateInsert.setDown(&setTohenIluyStateText);
    // setTohenIluyStateInsert.setOk(&setTohenIluyStateInsertSet);

    setTohenIluyStateText.setMenu(&tohenIluyState);
    setTohenIluyStateText.setUp(&setTohenIluyStateKezev);
    setTohenIluyStateText.setDown(&setTohenIluyStateKezev);
    setTohenIluyStateText.setOk(&setTohenIluyStateTextList);

    // setTohenIluyStateInsertSet.setMenu(&setTohenIluyStateTextList);
    // setTohenIluyStateInsertSet.setUp(&setTohenIluyStateInsert);
    // setTohenIluyStateInsertSet.setDown(&setTohenIluyStateInsert);
    // setTohenIluyStateInsertSet.setOk(&setTohenIluyStateTextList);

    setTohenIluyStateTextList.setMenu(&setTohenIluyStateText);
    setTohenIluyStateTextList.setUp(&setTohenIluyStateTextList);
    setTohenIluyStateTextList.setDown(&setTohenIluyStateTextList);
    setTohenIluyStateTextList.setOk(&setTohenIluyStateDelete);

    // setTohenIluyStateEdit.setMenu(&setTohenIluyStateTextList);
    // setTohenIluyStateEdit.setUp(&setTohenIluyStateDelete);
    // setTohenIluyStateEdit.setDown(&setTohenIluyStateDelete);
    // setTohenIluyStateEdit.setOk(&setTohenIluyStateDelete);

    setTohenIluyStateDelete.setMenu(&setTohenIluyStateTextList);
    setTohenIluyStateDelete.setUp(&setTohenIluyStateDelete);
    setTohenIluyStateDelete.setDown(&setTohenIluyStateDelete);
    setTohenIluyStateDelete.setOk(&setTohenIluyStateTextList);

    setTohenIluyStateKezevSet.setMenu(&setTohenIluyStateKezev);
    setTohenIluyStateKezevSet.setUp(&setTohenIluyStateKezevSet);
    setTohenIluyStateKezevSet.setDown(&setTohenIluyStateKezevSet);
    setTohenIluyStateKezevSet.setOk(&setTohenIluyStateKezev);

    setTohenTimeStateKezevSet.setMenu(&setTohenTimeStateKezev);
    setTohenTimeStateKezevSet.setUp(&setTohenTimeStateKezevSet);
    setTohenTimeStateKezevSet.setDown(&setTohenTimeStateKezevSet);
    setTohenTimeStateKezevSet.setOk(&setTohenTimeStateKezev);

    setTohenInfoStateKezevSet.setMenu(&setTohenInfoStateKezev);
    setTohenInfoStateKezevSet.setUp(&setTohenInfoStateKezevSet);
    setTohenInfoStateKezevSet.setDown(&setTohenInfoStateKezevSet);
    setTohenInfoStateKezevSet.setOk(&setTohenInfoStateKezev);

    setTohenTimeStateKezev.setMenu(&tohenTimeState);
    setTohenBottomLineTAZ.setMenu(&tohenTimeState);
    setTohenBottomLineSOZT.setMenu(&tohenTimeState);
    setTohenBottomLineSOZKS.setMenu(&tohenTimeState);
    setTohenBottomLineHMM.setMenu(&tohenTimeState);
    setTohenBottomLinePSZ.setMenu(&tohenTimeState);
    setTohenBottomLineHAZ.setMenu(&tohenTimeState);
    setTohenBottomLineZSZ.setMenu(&tohenTimeState);
    setTohenBottomLineLoazi.setMenu(&tohenTimeState);

    setTohenTimeStateKezev.setUp(&setTohenBottomLineLoazi);
    setTohenBottomLineTAZ.setUp(&setTohenTimeStateKezev);
    setTohenBottomLineSOZT.setUp(&setTohenBottomLineTAZ);
    setTohenBottomLineSOZKS.setUp(&setTohenBottomLineSOZT);
    setTohenBottomLineHMM.setUp(&setTohenBottomLineSOZKS);
    setTohenBottomLinePSZ.setUp(&setTohenBottomLineHMM);
    setTohenBottomLineHAZ.setUp(&setTohenBottomLinePSZ);
    setTohenBottomLineZSZ.setUp(&setTohenBottomLineHAZ);
    setTohenBottomLineLoazi.setUp(&setTohenBottomLineZSZ);

    setTohenTimeStateKezev.setDown(&setTohenBottomLineTAZ);
    setTohenBottomLineTAZ.setDown(&setTohenBottomLineSOZT);
    setTohenBottomLineSOZT.setDown(&setTohenBottomLineSOZKS);
    setTohenBottomLineSOZKS.setDown(&setTohenBottomLineHMM);
    setTohenBottomLineHMM.setDown(&setTohenBottomLinePSZ);
    setTohenBottomLinePSZ.setDown(&setTohenBottomLineHAZ);
    setTohenBottomLineHAZ.setDown(&setTohenBottomLineZSZ);
    setTohenBottomLineZSZ.setDown(&setTohenBottomLineLoazi);
    setTohenBottomLineLoazi.setDown(&setTohenTimeStateKezev);

    setTohenTimeStateKezev.setOk(&setTohenTimeStateKezevSet);
    setTohenBottomLineTAZ.setOk(&setTohenBottomLineTAZ1);
    setTohenBottomLineSOZT.setOk(&setTohenBottomLineSOZT1);
    setTohenBottomLineSOZKS.setOk(&setTohenBottomLineSOZKS1);
    setTohenBottomLineHMM.setOk(&setTohenBottomLineHMM1);
    setTohenBottomLinePSZ.setOk(&setTohenBottomLinePSZ1);
    setTohenBottomLineHAZ.setOk(&setTohenBottomLineHAZ1);
    setTohenBottomLineZSZ.setOk(&setTohenBottomLineZSZ1);
    setTohenBottomLineLoazi.setOk(&setTohenBottomLineLoazi);

    setTohenBottomLineTAZ0.setMenu(&setTohenBottomLineTAZ);
    setTohenBottomLineSOZT0.setMenu(&setTohenBottomLineSOZT);
    setTohenBottomLineSOZKS0.setMenu(&setTohenBottomLineSOZKS);
    setTohenBottomLineHMM0.setMenu(&setTohenBottomLineHMM);
    setTohenBottomLinePSZ0.setMenu(&setTohenBottomLinePSZ);
    setTohenBottomLineHAZ0.setMenu(&setTohenBottomLineHAZ);
    setTohenBottomLineZSZ0.setMenu(&setTohenBottomLineZSZ);

    setTohenBottomLineTAZ0.setUp(&setTohenBottomLineTAZ1);
    setTohenBottomLineSOZT0.setUp(&setTohenBottomLineSOZT1);
    setTohenBottomLineSOZKS0.setUp(&setTohenBottomLineSOZKS1);
    setTohenBottomLineHMM0.setUp(&setTohenBottomLineHMM1);
    setTohenBottomLinePSZ0.setUp(&setTohenBottomLinePSZ1);
    setTohenBottomLineHAZ0.setUp(&setTohenBottomLineHAZ1);
    setTohenBottomLineZSZ0.setUp(&setTohenBottomLineZSZ1);

    setTohenBottomLineTAZ0.setDown(&setTohenBottomLineTAZ2);
    setTohenBottomLineSOZT0.setDown(&setTohenBottomLineSOZT2);
    setTohenBottomLineSOZKS0.setDown(&setTohenBottomLineSOZKS2);
    setTohenBottomLineHMM0.setDown(&setTohenBottomLineHMM2);
    setTohenBottomLinePSZ0.setDown(&setTohenBottomLinePSZ2);
    setTohenBottomLineHAZ0.setDown(&setTohenBottomLineHAZ2);
    setTohenBottomLineZSZ0.setDown(&setTohenBottomLineZSZ2);

    setTohenBottomLineTAZ0.setOk(&setTohenBottomLineTAZ0);
    setTohenBottomLineSOZT0.setOk(&setTohenBottomLineSOZT0);
    setTohenBottomLineSOZKS0.setOk(&setTohenBottomLineSOZKS0);
    setTohenBottomLineHMM0.setOk(&setTohenBottomLineHMM0);
    setTohenBottomLinePSZ0.setOk(&setTohenBottomLinePSZ0);
    setTohenBottomLineHAZ0.setOk(&setTohenBottomLineHAZ0);
    setTohenBottomLineZSZ0.setOk(&setTohenBottomLineZSZ0);

    setTohenBottomLineTAZ1.setMenu(&setTohenBottomLineTAZ);
    setTohenBottomLineSOZT1.setMenu(&setTohenBottomLineSOZT);
    setTohenBottomLineSOZKS1.setMenu(&setTohenBottomLineSOZKS);
    setTohenBottomLineHMM1.setMenu(&setTohenBottomLineHMM);
    setTohenBottomLinePSZ1.setMenu(&setTohenBottomLinePSZ);
    setTohenBottomLineHAZ1.setMenu(&setTohenBottomLineHAZ);
    setTohenBottomLineZSZ1.setMenu(&setTohenBottomLineZSZ);

    setTohenBottomLineTAZ1.setUp(&setTohenBottomLineTAZ2);
    setTohenBottomLineSOZT1.setUp(&setTohenBottomLineSOZT2);
    setTohenBottomLineSOZKS1.setUp(&setTohenBottomLineSOZKS2);
    setTohenBottomLineHMM1.setUp(&setTohenBottomLineHMM2);
    setTohenBottomLinePSZ1.setUp(&setTohenBottomLinePSZ2);
    setTohenBottomLineHAZ1.setUp(&setTohenBottomLineHAZ2);
    setTohenBottomLineZSZ1.setUp(&setTohenBottomLineZSZ2);

    setTohenBottomLineTAZ1.setDown(&setTohenBottomLineTAZ0);
    setTohenBottomLineSOZT1.setDown(&setTohenBottomLineSOZT0);
    setTohenBottomLineSOZKS1.setDown(&setTohenBottomLineSOZKS0);
    setTohenBottomLineHMM1.setDown(&setTohenBottomLineHMM0);
    setTohenBottomLinePSZ1.setDown(&setTohenBottomLinePSZ0);
    setTohenBottomLineHAZ1.setDown(&setTohenBottomLineHAZ0);
    setTohenBottomLineZSZ1.setDown(&setTohenBottomLineZSZ0);

    setTohenBottomLineTAZ1.setOk(&setTohenBottomLineTAZ1);
    setTohenBottomLineSOZT1.setOk(&setTohenBottomLineSOZT1);
    setTohenBottomLineSOZKS1.setOk(&setTohenBottomLineSOZKS1);
    setTohenBottomLineHMM1.setOk(&setTohenBottomLineHMM1);
    setTohenBottomLinePSZ1.setOk(&setTohenBottomLinePSZ1);
    setTohenBottomLineHAZ1.setOk(&setTohenBottomLineHAZ1);
    setTohenBottomLineZSZ1.setOk(&setTohenBottomLineZSZ1);

    setTohenBottomLineTAZ2.setMenu(&setTohenBottomLineTAZ);
    setTohenBottomLineSOZT2.setMenu(&setTohenBottomLineSOZT);
    setTohenBottomLineSOZKS2.setMenu(&setTohenBottomLineSOZKS);
    setTohenBottomLineHMM2.setMenu(&setTohenBottomLineHMM);
    setTohenBottomLinePSZ2.setMenu(&setTohenBottomLinePSZ);
    setTohenBottomLineHAZ2.setMenu(&setTohenBottomLineHAZ);
    setTohenBottomLineZSZ2.setMenu(&setTohenBottomLineZSZ);

    setTohenBottomLineTAZ2.setUp(&setTohenBottomLineTAZ0);
    setTohenBottomLineSOZT2.setUp(&setTohenBottomLineSOZT0);
    setTohenBottomLineSOZKS2.setUp(&setTohenBottomLineSOZKS0);
    setTohenBottomLineHMM2.setUp(&setTohenBottomLineHMM0);
    setTohenBottomLinePSZ2.setUp(&setTohenBottomLinePSZ0);
    setTohenBottomLineHAZ2.setUp(&setTohenBottomLineHAZ0);
    setTohenBottomLineZSZ2.setUp(&setTohenBottomLineZSZ0);

    setTohenBottomLineTAZ2.setDown(&setTohenBottomLineTAZ1);
    setTohenBottomLineSOZT2.setDown(&setTohenBottomLineSOZT1);
    setTohenBottomLineSOZKS2.setDown(&setTohenBottomLineSOZKS1);
    setTohenBottomLineHMM2.setDown(&setTohenBottomLineHMM1);
    setTohenBottomLinePSZ2.setDown(&setTohenBottomLinePSZ1);
    setTohenBottomLineHAZ2.setDown(&setTohenBottomLineHAZ1);
    setTohenBottomLineZSZ2.setDown(&setTohenBottomLineZSZ1);

    setTohenBottomLineTAZ2.setOk(&setTohenBottomLineTAZ2);
    setTohenBottomLineSOZT2.setOk(&setTohenBottomLineSOZT2);
    setTohenBottomLineSOZKS2.setOk(&setTohenBottomLineSOZKS2);
    setTohenBottomLineHMM2.setOk(&setTohenBottomLineHMM2);
    setTohenBottomLinePSZ2.setOk(&setTohenBottomLinePSZ2);
    setTohenBottomLineHAZ2.setOk(&setTohenBottomLineHAZ2);
    setTohenBottomLineZSZ2.setOk(&setTohenBottomLineZSZ2);

    setTohenInfoStateKezev.setMenu(&tohenInfoState);
    setTohenBottomLineNetz.setMenu(&tohenInfoState);
    setTohenBottomLineTfila.setMenu(&tohenInfoState);
    setTohenBottomLineHagim.setMenu(&tohenInfoState);
    setTohenBottomLineIsraeli.setMenu(&tohenInfoState);
    setTohenBottomLineDafYomi.setMenu(&tohenInfoState);
    setTohenBottomLineYerushalmi.setMenu(&tohenInfoState);
    setTohenBottomLineHaftara.setMenu(&tohenInfoState);
    setTohenBottomLineIvri.setMenu(&tohenInfoState);
    setTohenBottomLineIORT.setMenu(&tohenInfoState);

    setTohenInfoStateKezev.setUp(&setTohenBottomLineIORT);
    setTohenBottomLineNetz.setUp(&setTohenInfoStateKezev);
    setTohenBottomLineTfila.setUp(&setTohenBottomLineNetz);
    setTohenBottomLineHagim.setUp(&setTohenBottomLineTfila);
    setTohenBottomLineIsraeli.setUp(&setTohenBottomLineHagim);
    setTohenBottomLineDafYomi.setUp(&setTohenBottomLineIsraeli);
    setTohenBottomLineYerushalmi.setUp(&setTohenBottomLineDafYomi);
    setTohenBottomLineHaftara.setUp(&setTohenBottomLineYerushalmi);
    setTohenBottomLineIvri.setUp(&setTohenBottomLineHaftara);
    setTohenBottomLineIORT.setUp(&setTohenBottomLineIvri);

    setTohenInfoStateKezev.setDown(&setTohenBottomLineNetz);
    setTohenBottomLineNetz.setDown(&setTohenBottomLineTfila);
    setTohenBottomLineTfila.setDown(&setTohenBottomLineHagim);
    setTohenBottomLineHagim.setDown(&setTohenBottomLineIsraeli);
    setTohenBottomLineIsraeli.setDown(&setTohenBottomLineDafYomi);
    setTohenBottomLineDafYomi.setDown(&setTohenBottomLineYerushalmi);
    setTohenBottomLineYerushalmi.setDown(&setTohenBottomLineHaftara);
    setTohenBottomLineHaftara.setDown(&setTohenBottomLineIvri);
    setTohenBottomLineIvri.setDown(&setTohenBottomLineIORT);
    setTohenBottomLineIORT.setDown(&setTohenInfoStateKezev);

    setTohenInfoStateKezev.setOk(&setTohenInfoStateKezevSet);
    setTohenBottomLineNetz.setOk(&setTohenBottomLineNetzSet);
    setTohenBottomLineTfila.setOk(&setTohenBottomLineTfila);
    setTohenBottomLineHagim.setOk(&setTohenBottomLineHagim);
    setTohenBottomLineIsraeli.setOk(&setTohenBottomLineIsraeli);
    setTohenBottomLineDafYomi.setOk(&setTohenBottomLineDafYomi);
    setTohenBottomLineYerushalmi.setOk(&setTohenBottomLineYerushalmi);
    setTohenBottomLineHaftara.setOk(&setTohenBottomLineHaftara);
    setTohenBottomLineIvri.setOk(&setTohenBottomLineIvri);
    setTohenBottomLineIORT.setOk(&setTohenBottomLineIORT1);

    setTohenBottomLineIORT0.setMenu(&setTohenBottomLineIORT);
    setTohenBottomLineIORT0.setUp(&setTohenBottomLineIORT1);
    setTohenBottomLineIORT0.setDown(&setTohenBottomLineIORT1);
    setTohenBottomLineIORT0.setOk(&setTohenBottomLineIORT0);
    setTohenBottomLineIORT1.setMenu(&setTohenBottomLineIORT);
    setTohenBottomLineIORT1.setUp(&setTohenBottomLineIORT0);
    setTohenBottomLineIORT1.setDown(&setTohenBottomLineIORT0);
    setTohenBottomLineIORT1.setOk(&setTohenBottomLineIORT1);

    setTohenBottomLineNetzSet.setMenu(&setTohenBottomLineNetz);
    setTohenBottomLineNetzSet.setUp(&setTohenBottomLineNetzSet);
    setTohenBottomLineNetzSet.setDown(&setTohenBottomLineNetzSet);
    setTohenBottomLineNetzSet.setOk(&setTohenBottomLineNetz);
}

State *Logic::goToMenuCommand(MenuCommand command)
{
    switch (command) {
        // MainMenu Commands
        case MenuCommand::NONE: return &mainState;              // 0 - ללא פקודה
        case MenuCommand::MAIN: return &menuClockState;         // 1 - תפריט ראשי
        case MenuCommand::CLOCK: return &clockSetTime;          // 2 - שעה ותאריך
        case MenuCommand::DISPLAY1: return &brightnessState;    // 3 - תצוגה
        case MenuCommand::MINHAG: return &edotMizrah;           // 4 - מנהגים
        case MenuCommand::MAKOM: return &ofekState;             // 5 - מקום
        case MenuCommand::ZMANIM: return &minhagShitaState;     // 6 - זמנים
        case MenuCommand::CONTENT: return &tohenIluyState;      // 7 - תוכן
        case MenuCommand::RESET: return &menuConfgState;        // 8 - איפוס
        case MenuCommand::ABOUT: return &menuConfgState;        // 9 - אודות
        case MenuCommand::MAIN_SCREEN: return &mainState;       // 10 - מסך ראשי
        case MenuCommand::CONFIG: return &bTState;              // 11 - הגדרות

        // ClockMenu Commands
        case MenuCommand::SET_DATETIME: return &setHourState;   // 20 - כיוון שעה ותאריך
        case MenuCommand::TIME_FORMAT: return &setClkFormat;    // 21 - מבנה שעה
        case MenuCommand::DATE_FORMAT: return &setDateFormat;   // 22 - מבנה תאריך
        case MenuCommand::TIME_STRUCT: return &setTimeFormat;   // 23 - מבנה זמן
        case MenuCommand::NEXT_DAY: return &minhagLeilState;    // 24 - ליל מחר לאחר שקיעה
        case MenuCommand::DST: return &setShaonKaiz;            // 25 - שעון קיץ

        // DisplayMenu Commands
        case MenuCommand::COLOR: return &colorUpLine;              // 40 - צבע
        case MenuCommand::BRIGHTNESS: return &setBrightnessState;  // 41 - בהירות
        case MenuCommand::FONT: return &setFontState;              // 42 - גופן

        // LocationMenu Commands
        case MenuCommand::SELECT_HORIZON: return &setOfekState;     // 50 - בחר אופק
        case MenuCommand::SET_LOCATION: return &lonCustomState;     // 51 - הגדר מקום
        case MenuCommand::WALLED_CITY: return &setHomaCustomState;  // 52 - עיר מוקפת חומה

        // TimeSettingsMenu Commands
        case MenuCommand::SELECT_METHOD: return &shitaState;               // 60 - בחר שיטה
        case MenuCommand::DESTRUCTION_YEARS: return &minhagShanimSetState; // 61 - שנים לחורבן
        case MenuCommand::EARLY_CANDLES: return &minhagNerotSetState;      // 62 - הדלקת נרות מוקדמת
        case MenuCommand::SHOW_ALL_TIMES: return &showTime;                // 63 - הצגת כל זמני היום

        // ContentMenu Commands
        case MenuCommand::EDIT: return &setTohenIluyStateTextList;           // 70 - ערוך
        case MenuCommand::ENTER_TEXT: return &setTohenIluyStateTextList;    // 71 - הזן טקסט
        case MenuCommand::CHANGE_RATE: return &setTohenIluyStateKezev;       // 72 - קצב החלפה

        // CustomsMenu Commands
        case MenuCommand::DIASPORA: return &edotBneiHul;   // 80 - בני חו"ל

        // Default case
        default: return &mainState;  // ברירת מחדל
    }
}
