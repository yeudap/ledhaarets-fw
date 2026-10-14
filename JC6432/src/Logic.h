#ifndef Logic_H
#define Logic_H


#include "States/States.h"
#include "BLE/Commands.h"


class Logic
{
 public:
    //States
    MainState mainState;

    MenuClockState menuClockState;
    MenuTzugaState menuTzugaState;
    MenuConfgState menuConfgState;
    MenuOffekState menuOffekState;
    MenuTohenState menuTohenState;
    MenuEddotState menuEddotState;
    MenuMinagState menuMinagState;

    SetClockState setClockState;
    ClockSetTime clockSetTime;
    ClkFormat clkFormat;
    SetClkFormat setClkFormat;
    DateFormat dateFormat;
    SetDateFormat setDateFormat;
    TimeFormat timeFormat;
    SetTimeFormat setTimeFormat;
    ShaonKaiz shaonKaiz;
    SetShaonKaiz setShaonKaiz;
    SetHour setHourState;
    SetMinute setMinuteState;
    SetSecond setSecondState;
    SetDay setDayState;
    SetMonth setMonthState;
    SetYear setYearState;
    
    FontState fontState;
    SetFontState setFontState;

    BrightnessState brightnessState;
    SetBrightnessState setBrightnessState;

    ColorState colorState;
    ColorUpLine colorUpLine;
    ColorMiddleLine colorMiddleLine;
    ColorBottomTLine colorBottomTLine;
    ColorBottomVLine colorBottomVLine;
    ColorYomLine colorYomLine;
    ColorIvriLine colorIvriLine;
    ColorLoaziLine colorLoaziLine;
    ColorParashaLine colorParashaLine;
    ColorParashaVLine colorParashaVLine;
    ColorIluyLine colorIluyLine;
    SetColorUpLine setColorUpLine;
    SetColorMiddleLine setColorMiddleLine;
    SetColorBottomTLine setColorBottomTLine;
    SetColorBottomVLine setColorBottomVLine;
    SetColorYomLine setColorYomLine;
    SetColorIvriLine setColorIvriLine;
    SetColorLoaziLine setColorLoaziLine;
    SetColorParashaLine setColorParashaLine;
    SetColorParashaVLine setColorParashaVLine;
    SetColorIluyLine setColorIluyLine;

    BTState bTState;
    SetBTState setBTState;
    ResetPrefState resetPrefState;
    SetResetPrefState setResetPrefState;
    ResetTextState resetTextState;
    SetResetTextState setResetTextState;
    ResetPassState resetPassState;
    SetResetPassState setResetPassState;
    AboutUsState aboutUsState;
    ViewAboutUsState viewAboutUsState;
    AboutDeviceState aboutDeviceState;
    ViewDeviceState viewDeviceState;
    MemoryMapState memoryMapState;
    ViewMemoryMapState viewMemoryMapState;
    
    UpdateState updateState;

    OfekState ofekState;
    SetOfekState setOfekState;
    CustomAreaState customAreaState;
    SetHomaCustomState setHomaCustomState;
    SetCustomState setCustomState;
    LonCustomState lonCustomState;
    SetLonPre setLonPre;
    SetLonInt setLonInt;
    SetLonFrac setLonFrac;
    LatCustomState latCustomState;
    SetLatPre setLatPre;
    SetLatInt setLatInt;
    SetLatFrac setLatFrac;
    GovaCustomState govaCustomState;
    SetGovaCustomState setGovaCustomState;
    SetHulCustomState setHulCustomState;
    TimeZoneCustomState timeZoneCustomState;
    SetTimeZoneCustomState setTimeZoneCustomState;

    ShitaState shitaState;

    EdotMizrah edotMizrah;
    EdotSfarad edotSfarad;
    EdotAshkenaz edotAshkenaz;
    EdotTeimanBaladi edotTeimanBaladi;
    EdotTeimanShami edotTeimanShami;
    EdotChabad edotChabad;
    EdotBneiHul edotBneiHul;

    MinhagShitaState minhagShitaState;
    MinhagShanimState minhagShanimState;
    MinhagZrihaState minhagZrihaState;
    SetZrihaState setZrihaState;
    MinhagShkiaaState minhagShkiaaState;
    SetShkiaaState setShkiaaState;
    MinhagNerotState minhagNerotState;
    MinhagLeilState minhagLeilState;
    MinhagNerotSetState minhagNerotSetState;
    MinhagShanimSetState minhagShanimSetState;
    ZeetCochavimState zeetCochavimState;
    SetZeetCochavimState setZeetCochavimState;
    RabenuTamState rabenuTamState;
    SetRabenuTamState setRabenuTamState;
    TalitState talitState;
    SetTalitState setTalitState;
    AlotState alotState;
    SetAlotState setAlotState;
    MinhaGState minhaGState;
    SetMinhaGState setMinhaGState;
    PlagState plagState;
    SetPlagState setPlagState;
    SozksMgaState sozksMgaState;
    SetSozksMgaState setSozksMgaState;
    SoztMgaState soztMgaState;
    SetSoztMgaState setSoztMgaState;
    ZeetShabatState zeetShabatState;
    SetZeetShabatState setZeetShabatState;
    ZeetZomState zeetZomState;
    SetZeetZomState setZeetZomState;
    ShowTimeState showTimeState;
    ShowTime showTime;

    TohenIluyState tohenIluyState;
    TohenTimeState tohenTimeState;
    TohenInfoState tohenInfoState;
    
    SetTohenIluyStateKezev setTohenIluyStateKezev;
    SetTohenIluyStateKezevSet setTohenIluyStateKezevSet;
    // SetTohenIluyStateInsert setTohenIluyStateInsert;
    SetTohenIluyStateText setTohenIluyStateText;
    // SetTohenIluyStateInsertSet setTohenIluyStateInsertSet;
    SetTohenIluyStateTextList setTohenIluyStateTextList;
    // SetTohenIluyStateEdit setTohenIluyStateEdit;
    SetTohenIluyStateDelete setTohenIluyStateDelete;

    SetTohenTimeStateKezev setTohenTimeStateKezev;
    SetTohenTimeStateKezevSet setTohenTimeStateKezevSet;

    SetTohenInfoStateKezev setTohenInfoStateKezev;
    SetTohenInfoStateKezevSet setTohenInfoStateKezevSet;
    SetTohenBottomLineLoazi setTohenBottomLineLoazi;

    SetTohenBottomLineTAZ setTohenBottomLineTAZ;
    SetTohenBottomLineSOZT setTohenBottomLineSOZT;
    SetTohenBottomLineSOZKS setTohenBottomLineSOZKS;
    SetTohenBottomLineHMM setTohenBottomLineHMM;
    SetTohenBottomLinePSZ setTohenBottomLinePSZ;
    SetTohenBottomLineHAZ setTohenBottomLineHAZ;
    SetTohenBottomLineZSZ setTohenBottomLineZSZ;
    SetTohenBottomLineTAZ0 setTohenBottomLineTAZ0;
    SetTohenBottomLineSOZT0 setTohenBottomLineSOZT0;
    SetTohenBottomLineSOZKS0 setTohenBottomLineSOZKS0;
    SetTohenBottomLineHMM0 setTohenBottomLineHMM0;
    SetTohenBottomLinePSZ0 setTohenBottomLinePSZ0;
    SetTohenBottomLineHAZ0 setTohenBottomLineHAZ0;
    SetTohenBottomLineZSZ0 setTohenBottomLineZSZ0;
    SetTohenBottomLineTAZ1 setTohenBottomLineTAZ1;
    SetTohenBottomLineSOZT1 setTohenBottomLineSOZT1;
    SetTohenBottomLineSOZKS1 setTohenBottomLineSOZKS1;
    SetTohenBottomLineHMM1 setTohenBottomLineHMM1;
    SetTohenBottomLinePSZ1 setTohenBottomLinePSZ1;
    SetTohenBottomLineHAZ1 setTohenBottomLineHAZ1;
    SetTohenBottomLineZSZ1 setTohenBottomLineZSZ1;
    SetTohenBottomLineTAZ2 setTohenBottomLineTAZ2;
    SetTohenBottomLineSOZT2 setTohenBottomLineSOZT2;
    SetTohenBottomLineSOZKS2 setTohenBottomLineSOZKS2;
    SetTohenBottomLineHMM2 setTohenBottomLineHMM2;
    SetTohenBottomLinePSZ2 setTohenBottomLinePSZ2;
    SetTohenBottomLineHAZ2 setTohenBottomLineHAZ2;
    SetTohenBottomLineZSZ2 setTohenBottomLineZSZ2;
    
    SetTohenBottomLineIORT setTohenBottomLineIORT;
    SetTohenBottomLineNetz setTohenBottomLineNetz;
    SetTohenBottomLineTfila setTohenBottomLineTfila;
    SetTohenBottomLineHagim setTohenBottomLineHagim;
    SetTohenBottomLineIsraeli setTohenBottomLineIsraeli;
    SetTohenBottomLineDafYomi setTohenBottomLineDafYomi;
    SetTohenBottomLineYerushalmi setTohenBottomLineYerushalmi;
	SetTohenBottomLineHaftara setTohenBottomLineHaftara;
    SetTohenBottomLineIvri setTohenBottomLineIvri;
   
    SetTohenBottomLineNetzSet setTohenBottomLineNetzSet;

    SetTohenBottomLineIORT0 setTohenBottomLineIORT0;
    SetTohenBottomLineIORT1 setTohenBottomLineIORT1;

    void init();
    void initStates();
    void configStates();
    State* goToMenuCommand(MenuCommand command);
};

#endif