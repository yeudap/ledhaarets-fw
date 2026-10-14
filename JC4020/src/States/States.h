#ifndef States_H
#define States_H


#include "State.h"
#include "Preferences/UserPreferences.h"
#include "Screens/Screens.h"
#include "CLK/Clock.h"
#include "Digit.h"


extern MainScreen mainScreen;
extern MenuScreen menuScreen;
extern BrightnessScreen brightnessScreen;
extern ColorScreen colorScreen;
extern AboutScreen aboutScreen;
extern LogoScreen logoScreen;
extern VersionScreen versionScreen;
extern RegsScreen regsScreen;
extern SetClockScreen setClockScreen;
extern ResetScreen resetScreen;
extern MessageScreen messageScreen;

extern UserPreferences userPreferences;

//Main
const uint8_t numOfTimeSets = 33;
const uint8_t numOfNameSets = 0;

class MainState : public State 
{
    public:
    bool flagTimeSetChanged;
    uint8_t stateIndexTimeSet;
    int intervalTimeSet;
    DateTime nowPlusIntervalTimeSet;
    bool updateDayValues;
    Zman startTimeToNetz;
    bool showTimeToNetz;
    bool flagNetz;
    TimeSpan timeLeft;
    DateTime netz;
    Text txtTitle5;
    Text txtValue5;
    Text txtTitle6;
    Text txtValue6;
    Text txtZman;
    bool morphing;
    Digit h0 = Digit(5, 14);
    Digit h1 = Digit(5+18, 14);
    Digit m0 = Digit(5+18+26, 14);
    Digit m1 = Digit(5+18+26+18, 14);
    Digit s0 = Digit(5+18+26+18+26, 14);
    Digit s1 = Digit(5+18+26+18+26+18, 14);
    std::array<std::pair<void(MainState::*)(), bool(MainState::*)()>, numOfTimeSets> timeSetFuncList;

    // State logic
    MainState();
    void showStateScreen();
    void stateLoop();
    void setShaonKaitzHoref();
    void resetDevice();
    void timeToNetz();
    void setNewDay();
    void setText();

    // Static areas
    //up
    void updateUpLine();
    void updateHebDate(bool); 
    void updateLoaziDate();
    void updateDayUp();
    //middle
    void updateMiddleLine();
    void checkTimeChanged();
    void updateHour();
    void updateMinute();
    void updateSecond();
    void updateSecond1();
    void updateSecond2();
    void updateNekudotaim();
    void updateYom();
    //bottom
    void updateNetz(Zman);

    void initTimeSetFuncList();
    void UpdateTimeSet(uint8_t);

    void timeSetZrihaYomShkiaa();
    bool flagTimeSetZrihaYomShkiaa();
    void timeSetZrihaParashaShkiaa();
    bool flagTimeSetZrihaParashaShkiaa();
    void timeSetShabatInParashaShabatOut();
    bool flagTimeSetShabatInParashaShabatOut();
    void timeSetZrihaShkiaaCochavim();
    bool flagTimeSetZrihaShkiaaCochavim();
    void timeSetSozShma();
    bool flagTimeSetSozShma();
    void timeSetSozTfila();
    bool flagTimeSetSozTfila();
    void timeSetTalitAlotZriha();
    bool flagTimeSetTalitAlotZriha();
    void timeSetHazotMinhaGMinhaK();
    bool flagTimeSetHazotMinhaGMinhaK();
    void timeSetPlagShkiaaCochavim();
    bool flagTimeSetPlagShkiaaCochavim();
    void timeSetHazotAlotZriha();
    bool flagTimeSetHazotAlotZriha();
    
    // Info sets
    void infoSetShabatInShabatOutRabenuTam();
    bool flagInfoSetShabatInShabatOutRabenuTam();
    void infoSetShabatPlus();
    bool flagInfoSetShabatPlus();
    void infoSetHagInHagOutRabenuTam();
    bool flagInfoSetHagInHagOutRabenuTam();
    void infoSetHagYaaleVeyavoHalel();
    bool flagInfoSetHagYaaleVeyavoHalel();
    void infoSetErevHagEruv();
    bool flagInfoSetErevHagEruv();
    void infoSetRegularDay();
    bool flagInfoSetRegularDay();
    void infoSetRoshHodesh();
    bool flagInfoSetRoshHodesh();
    void infoSetNextRoshHodesh();
    bool flagInfoSetNextRoshHodesh();
    void infoSetMolad();
    bool flagInfoSetMolad();
    void infoSetZom();
    bool flagInfoSetZom();
    void infoSetTishaBeav();
    bool flagInfoSetTishaBeav();
    void infoSetIsraeli();
    bool flagInfoSetIsraeli();
    void infoSetOmer();
    bool flagInfoSetOmer();
    void infoSetHametz();
    bool flagInfoSetHametz();
    void infoSetAseretYemeTshuva();
    bool flagInfoSetAseretYemeTshuva();
    void infoSetPurim();
    bool flagInfoSetPurim();
    void infoSetHanuka();
    bool flagInfoSetHanuka();
    void infoSetYom();
    bool flagInfoSetYom();
    void infoSetParasha();
    bool flagInfoSetParasha();
    void infoSetHaftara();
    bool flagInfoSetHaftara();
    void infoSetYerushalmi();
    bool flagInfoSetYerushalmi();
    void infoSetChabad();
    bool flagInfoSetChabad();
    void infoSetIvri();
    bool flagInfoSetIvri();
};

//Menu
class MenuClockState : public State 
{
    public:
    void showStateScreen();
};
class MenuTzugaState : public State
{
    public:
    void showStateScreen();
};
class MenuConfgState : public State
{
    public:
    void showStateScreen();
};
class MenuOffekState : public State
{
    public:
    void showStateScreen();
};
class MenuTohenState : public State
{
    public:
    void showStateScreen();
};
class MenuEddotState : public State
{
    public:
    void showStateScreen();
};
class MenuMinagState : public State
{
    public:
    void showStateScreen();
};

//Clock
class ClockSetTime : public State
{
    public:
    void showStateScreen();
};
class ClkFormat : public State
{
    public:
    void showStateScreen();
};
class DateFormat : public State
{
    public:
    void showStateScreen();
};
class TimeFormat : public State
{
    public:
    void showStateScreen();
};
class ShaonKaiz : public State
{
    public:
    void showStateScreen();
};
class SetShaonKaiz : public State
{
    public:
    uint8_t kaizTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetClkFormat : public State
{
    public:
    uint8_t formatTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetDateFormat : public State
{
    public:
    uint8_t formatTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetTimeFormat : public State
{
    public:
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetClockState : public State 
{
    public:
    int tempHour;
    int tempMinute;
    int tempSecond;
    int tempDay;
    int tempMonth;
    int tempYear;
    bool changed = false;
    virtual void showStateScreen();
    virtual void stateLoop();
    virtual void updateHour();
    virtual void updateMinute();
    virtual void updateSecond();
    virtual void updateDay(); 
    virtual void updateMonth(); 
    virtual void updateYear(); 
    virtual void focusArea() {}
    void commonMenuAction();
    void validateAndSetTime(DateTime);
};
class SetHour : public SetClockState
{
    public:
    void updateHour();
    void focusArea();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetMinute : public SetClockState
{
    public:
    void updateMinute();
    void focusArea();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetSecond : public SetClockState
{
    public:
    void updateSecond();
    void focusArea();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetDay : public SetClockState
{
    public:
    void updateDay(); 
    void focusArea();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetMonth : public SetClockState
{
    public:
    void updateMonth(); 
    void focusArea();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetYear : public SetClockState
{
    public:
    void focusArea();
    void updateYear(); 
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};

//Brightness
class BrightnessState : public State
{
    public:
    void showStateScreen();
};
class SetBrightnessState : public State
{
    public:
    int brightTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
//Font
class FontState : public State
{
    public:
    void showStateScreen();
};
class SetFontState : public State
{
    public:
    int fontTemp;
    void stateLoop();
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
//Color
class ColorState : public State
{
    public:
    void showStateScreen();
};
class ColorUpLine : public State
{
    public:
    void showStateScreen();
};
class ColorMiddleLine : public State
{
    public:
    void showStateScreen();
};
class ColorBottomTLine : public State
{
    public:
    void showStateScreen();
};
class ColorBottomVLine : public State
{
    public:
    void showStateScreen();
};
class SetColorUpLine : public State
{
    public:
    int colorTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetColorMiddleLine : public State
{
    public:
    int colorTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetColorBottomTLine : public State
{
    public:
    int colorTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetColorBottomVLine : public State
{
    public:
    int colorTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};

//Reset
//Config
class BTState : public State
{
    public:
    void showStateScreen();
};
class SetBTState : public State
{
    public:
    uint8_t btTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class ResetPrefState : public State
{
    public:
    void showStateScreen();
};
class SetResetPrefState : public State
{
    public:
    bool reset = false;
    void showStateScreen();
    void focusArea();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class ResetTextState : public State
{
    public:
    void showStateScreen();
};
class SetResetTextState : public State
{
    public:
    bool reset = false;
    void showStateScreen();
    void focusArea();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class ResetPassState : public State
{
    public:
    void showStateScreen();
};
class SetResetPassState : public State
{
    public:
    bool reset = false;
    void showStateScreen();
    void focusArea();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class AboutUsState : public State
{
    public:
    void showStateScreen();
};
class ViewAboutUsState : public State
{
    public:
    void showStateScreen();
};
class AboutDeviceState : public State
{
    public:
    void showStateScreen();
};
class ViewDeviceState : public State
{
    public:
    void showStateScreen();
};
class MemoryMapState : public State
{
    public:
    void showStateScreen();
};
class ViewMemoryMapState : public State
{
    public:
    void showStateScreen();
};
class UpdateState : public State
{
    public:
    void showStateScreen();
    void updateScreen(uint8_t percent);
    void showUpdateStartScreen();
    void showUpdateProgress(uint8_t percent);
    void showUpdateSuccessScreen();
    void showUpdateCancelledScreen();
    void showUpdateFailedScreen(bool willRestart);
};

//Ofek
class OfekState : public State
{
    public:
    void showStateScreen();
};
class SetOfekState : public State
{
    public:
    uint8_t ofekTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class CustomAreaState : public State
{
    public:
    void showStateScreen();
};
class SetHomaCustomState : public State
{
    public:
    void showStateScreen();
    void okAction();
};

class LonCustomState : public State 
{
    public:
    void showStateScreen();
};
class SetCustomState : public State 
{
    public:
    void printUpdateSucc();
};
class SetLonCustomState : public SetCustomState 
{
    public:
    int tempPre;
    int tempInt;
    int tempFrac;
    void menuAction();
    bool changed = false;
    virtual void showStateScreen();
    virtual void updatePre();
    virtual void updateInt();
    virtual void updateFrac();
    virtual void focusArea() {}
};
class SetLonPre : public SetLonCustomState
{
    public:
    void updatePre();
    void focusArea();
    void upAction();
    void downAction();
    void okAction();
};
class SetLonInt : public SetLonCustomState
{
    public:
    void updateInt();
    void focusArea();
    void upAction();
    void downAction();
    void okAction();
};
class SetLonFrac : public SetLonCustomState
{
    public:
    void updateFrac();
    void focusArea();
    void upAction();
    void downAction();
    void okAction();
};
class LatCustomState : public State 
{
    public:
    void showStateScreen();
};
class SetLatCustomState : public SetCustomState 
{
    public:
    int tempPre;
    int tempInt;
    int tempFrac;
    void menuAction();
    bool changed = false;
    virtual void showStateScreen();
    virtual void updatePre();
    virtual void updateInt();
    virtual void updateFrac();
    virtual void focusArea() {}
};
class SetLatPre : public SetLatCustomState
{
    public:
    void updatePre();
    void focusArea();
    void upAction();
    void downAction();
    void okAction();
};
class SetLatInt : public SetLatCustomState
{
    public:
    void updateInt();
    void focusArea();
    void upAction();
    void downAction();
    void okAction();
};
class SetLatFrac : public SetLatCustomState
{
    public:
    void updateFrac();
    void focusArea();
    void upAction();
    void downAction();
    void okAction();
};
class GovaCustomState : public State 
{
    public:
    void showStateScreen();
};
class SetGovaCustomState : public State 
{
    public:
    int tempGova;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetHulCustomState : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class TimeZoneCustomState : public State 
{
    public:
    void showStateScreen();
};
class SetTimeZoneCustomState : public State 
{
    public:
    int tempTZ;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
//Shitot
class ShitaState : public State
{
    public:
    uint8_t shitaTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};

// Edot
class EdotMizrah : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class EdotSfarad : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class EdotAshkenaz : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class EdotTeimanBaladi : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class EdotTeimanShami : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class EdotChabad : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class EdotBneiHul : public State
{
    public:
    void showStateScreen();
    void okAction();
};
// Minhagim
class MinhagShitaState : public State
{
    public:
    void showStateScreen();
};
class MinhagZrihaState : public State
{
    public:
    void showStateScreen();
};
class MinhagShkiaaState : public State
{
    public:
    void showStateScreen();
};
class MinhagNerotState : public State
{
    public:
    void showStateScreen();
};
class MinhagShanimState : public State
{
    public:
    void showStateScreen();
};
class MinhagLeilState : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetZrihaState : public State
{
    public:
    uint8_t zrihaTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetShkiaaState : public State
{
    public:
    uint8_t shkiaaTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class MinhagNerotSetState : public State
{
    public:
    int nerotTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class MinhagShanimSetState : public State
{
    public:
    int shanimTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class ZeetCochavimState : public State
{
    public:
    void showStateScreen();
};
class SetZeetCochavimState : public State
{
    public:
    uint8_t zeetTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class RabenuTamState : public State
{
    public:
    void showStateScreen();
};
class SetRabenuTamState : public State
{
    public:
    uint8_t rtTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class TalitState : public State
{
    public:
    void showStateScreen();
};
class SetTalitState : public State
{
    public:
    uint8_t talitTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class AlotState : public State
{
    public:
    void showStateScreen();
};
class SetAlotState : public State
{
    public:
    uint8_t alotTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SozksMgaState : public State
{
    public:
    void showStateScreen();
};
class SetSozksMgaState : public State
{
    public:
    uint8_t sozksTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SoztMgaState : public State
{
    public:
    void showStateScreen();
};
class SetSoztMgaState : public State
{
    public:
    uint8_t soztTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class MinhaGState : public State
{
    public:
    void showStateScreen();
};
class SetMinhaGState : public State
{
    public:
    uint8_t minhaTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class PlagState : public State
{
    public:
    void showStateScreen();
};
class SetPlagState : public State
{
    public:
    uint8_t plagTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class ZeetShabatState : public State
{
    public:
    void showStateScreen();
};
class SetZeetShabatState : public State
{
    public:
    uint8_t zeetTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class ZeetZomState : public State
{
    public:
    void showStateScreen();
};
class SetZeetZomState : public State
{
    public:
    uint8_t zeetTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class ShowTimeState : public State
{
    public:
    void showStateScreen();
};
class ShowTime : public State
{
    public:
    uint8_t showTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
// Tochen
class TohenUpLine : public State
{
    public:
    void showStateScreen();
    void stateLoop();
};
class TohenMiddleLine : public State
{
    public:
    void showStateScreen();
    void stateLoop();
};
class TohenBottomLine : public State
{
    public:
    void showStateScreen();
    void stateLoop();
};

class SetTohenUpLine : public State
{
    public:
    int currentState;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};

class SetTohenMiddleLine : public State
{
    public:
    int currentState;
    void stateLoop();
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};

class SetTohenBottomLineKezev : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineKezevSet : public State
{
    public:
    uint16_t kezevTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetTohenBottomLineNetz : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineNetzSet : public State
{
    public:
    int netzTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetTohenBottomLineTAZ : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineSOZT : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineSOZKS : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineHMM : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLinePSZ : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineHAZ : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineZSZ : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineZSZh : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineZSZs : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineZYS : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineZPS : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineIPO : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineIORT : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineTfila : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineHagim : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineIsraeli : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineDafYomi : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineYerushalmi : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineYom : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineYomLezad : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineParasha : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineHaftara : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineIvri : public State
{
    public:
    void showStateScreen();
    void okAction();
};

class SetTohenBottomLineTAZ0 : public State
{
    public:
    int TAZtemp;
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineSOZT0 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineSOZKS0 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineHMM0 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLinePSZ0 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineHAZ0 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineZSZh0 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineZSZs0 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineTAZ1 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineSOZT1 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineSOZKS1 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineHMM1 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLinePSZ1 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineHAZ1 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineZSZh1 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineZSZs1 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineTAZ2 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineSOZT2 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineSOZKS2 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineHMM2 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLinePSZ2 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineHAZ2 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineZSZh2 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineZSZs2 : public State
{
    public:
    void showStateScreen();
    void okAction();
};

class SetTohenBottomLineZYS0 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineZPS0 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineIPO0 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineIORT0 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineZYS1 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineZPS1 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineIPO1 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenBottomLineIORT1 : public State
{
    public:
    void showStateScreen();
    void okAction();
};
#endif