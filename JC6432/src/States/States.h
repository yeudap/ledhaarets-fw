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
extern TextScreen textScreen;
extern UserPreferences userPreferences;

//Main
const uint8_t numOfTimeSets = 7;
const uint8_t numOfNameSets = 50;
class MainState : public State 
{
    public:
    uint8_t numOfInfoSets;
    bool flagTimeSetChanged;
    bool flagNameSetChanged;
    bool flagInfoSetChanged;
    uint8_t stateIndexTimeSet;
    uint8_t stateIndexInfoSet;
    uint8_t stateIndexNameSet;
    int intervalTimeSet;
    int intervalInfoSet;
    int intervalNameSet;
    DateTime nowPlusIntervalTimeSet;
    DateTime nowPlusIntervalInfoSet;
    DateTime nowPlusIntervalNameSet;
	bool updateDayValues;
    int indexText;
    int numOfValidTexts;
    Zman startTimeToNetz;
    bool showTimeToNetz;
    bool flagNetz;
    TimeSpan timeLeft;
    DateTime netz;
    Text txtTitle5;
    Text txtValue5;
    Text txtTitle6;
    Text txtValue6;
    Text txtInfo;
    Text txtInfoUp;
    Text txtInfoDown;
    Text txtZmanV;
    Text txtZmanT;

    bool morphing;

    std::array<std::pair<void(MainState::*)(), bool(MainState::*)()>, numOfTimeSets> timeSetFuncList;
    std::array<std::pair<void(MainState::*)(), bool(MainState::*)()>, 50> infoSetFuncList;
    // State logic
    MainState();
    void showStateScreen();
    void upAction();
    void downAction();
    void stateLoop();
    void setShaonKaitzHoref();
    void resetDevice();
    void timeToNetz();
    void setNewDay();
    void setText();

    // Static areas
    void updateUpLine();
    void updateHebDate(); 
    void updateLoaziDate();
    void updateClockOnScreen();
    void updateMiddleLine();
    void checkTimeChanged();
    void updateHour();
    void updateMinute();
    void updateSecond();
    void updateSecond1();
    void updateSecond2();
    void updateNekudotaim();
    void updateYom();
    void updateParashaTitle(); 
    void updateParashaValue(); 
    void updateNetz(Zman);

    // Time sets
    bool dynamicAllDay;
    bool OnceEveryTime;
    bool staticScreen;
    void initTimeSetFuncList();
    void UpdateTimeSet(uint8_t);

    void timeSetPlagShkiaaCochavim();
    bool flagTimeSetPlagShkiaaCochavim();
    void timeSetSozShma();
    bool flagTimeSetSozShma();
    void timeSetSozTfila();
    bool flagTimeSetSozTfila();
    void timeSetTalitAlotZriha();
    bool flagTimeSetTalitAlotZriha();
    void timeSetHazotMinhaGMinhaK();
    bool flagTimeSetHazotMinhaGMinhaK();
    void timeSetHazotAlotZriha();
    bool flagTimeSetHazotAlotZriha();
    void timeSetZrihaShkiaaCochavim();
    bool flagTimeSetZrihaShkiaaCochavim();
    void infoSetLoazi();
    bool flagInfoSetLoazi();

    // Info sets
    void initInfoSetFuncList();
    void initInfoSetFuncList1();
    void initInfoSetFuncList2();
    void UpdateInfoSet(uint8_t);

    void infoSet_1_ShabatInShabatOutRabenuTam();
    bool flaginfoSet_1_ShabatInShabatOutRabenuTam();
    void infoSet_1_ShabatPlus();
    bool flaginfoSet_1_ShabatPlus();
    void infoSet_1_HagInHagOutRabenuTam();
    bool flaginfoSet_1_HagInHagOutRabenuTam();
    void infoSet_1_HagYaaleVeyavoHalel();
    bool flaginfoSet_1_HagYaaleVeyavoHalel();
    void infoSet_1_HagYaaleVeyavoHalel1();
    bool flaginfoSet_1_HagYaaleVeyavoHalel1();
    void infoSet_1_ErevHagEruv();
    bool flaginfoSet_1_ErevHagEruv();
    void infoSet_1_RegularDay();
    bool flaginfoSet_1_RegularDay();
    void infoSet_1_Tahanun();
    bool flaginfoSet_1_Tahanun();
    void infoSet_1_RoshHodesh();
    bool flaginfoSet_1_RoshHodesh();
    void infoSet_1_RoshHodesh1();
    bool flaginfoSet_1_RoshHodesh1();
    void infoSet_1_NextRoshHodesh();
    bool flaginfoSet_1_NextRoshHodesh();
    void infoSet_1_Molad();
    bool flaginfoSet_1_Molad();
    void infoSet_1_Zom();
    bool flaginfoSet_1_Zom();
    void infoSet_1_Zom1();
    bool flaginfoSet_1_Zom1();
    void infoSet_1_Zom2();
    bool flaginfoSet_1_Zom2();
    void infoSet_1_TishaBeav();
    bool flaginfoSet_1_TishaBeav();
    void infoSet_1_Israeli();
    bool flaginfoSet_1_Israeli();
    void infoSet_1_Omer();
    bool flaginfoSet_1_Omer();
    void infoSet_1_Omer1();
    bool flaginfoSet_1_Omer1();
    void infoSet_1_Hametz();
    bool flaginfoSet_1_Hametz();
    void infoSet_1_Hametz1();
    bool flaginfoSet_1_Hametz1();
    void infoSet_1_AseretYemeTshuva();
    bool flaginfoSet_1_AseretYemeTshuva();
    void infoSet_1_AseretYemeTshuva1();
    bool flaginfoSet_1_AseretYemeTshuva1();
    void infoSet_1_Purim();
    bool flaginfoSet_1_Purim();
    void infoSet_1_Hanuka();
    bool flaginfoSet_1_Hanuka();
    void infoSet_1_Hanuka1();
    bool flaginfoSet_1_Hanuka1();
    void infoSet_1_Haftara();
    bool flaginfoSet_1_Haftara();
    void infoSet_1_Yerushalmi();
    bool flaginfoSet_1_Yerushalmi();
    void infoSet_1_Bavli();
    bool flaginfoSet_1_Bavli();
    void infoSet_1_Ivri();
    bool flaginfoSet_1_Ivri();
    void infoSet_1_Chabad();
    bool flaginfoSet_1_Chabad();

    void infoSet_2_ShabatInShabatOutRabenuTam();
    bool flaginfoSet_2_ShabatInShabatOutRabenuTam();
    void infoSet_2_ShabatPlus();
    bool flaginfoSet_2_ShabatPlus();
    void infoSet_2_HagInHagOutRabenuTam();
    bool flaginfoSet_2_HagInHagOutRabenuTam();
    void infoSet_2_HagYaaleVeyavoHalel();
    bool flaginfoSet_2_HagYaaleVeyavoHalel();
    void infoSet_2_ErevHagEruv();
    bool flaginfoSet_2_ErevHagEruv();
    void infoSet_2_RegularDay();
    bool flaginfoSet_2_RegularDay();
    void infoSet_2_RoshHodesh();
    bool flaginfoSet_2_RoshHodesh();
    void infoSet_2_NextRoshHodesh();
    bool flaginfoSet_2_NextRoshHodesh();
    void infoSet_2_Molad();
    bool flaginfoSet_2_Molad();
    void infoSet_2_Zom();
    bool flaginfoSet_2_Zom();
    void infoSet_2_TishaBeav();
    bool flaginfoSet_2_TishaBeav();
    void infoSet_2_Israeli();
    bool flaginfoSet_2_Israeli();
    void infoSet_2_Omer();
    bool flaginfoSet_2_Omer();
    void infoSet_2_Hametz();
    bool flaginfoSet_2_Hametz();
    void infoSet_2_AseretYemeTshuva();
    bool flaginfoSet_2_AseretYemeTshuva();
    void infoSet_2_Purim();
    bool flaginfoSet_2_Purim();
    void infoSet_2_Hanuka();
    bool flaginfoSet_2_Hanuka();
    void infoSet_2_Haftara();
    bool flaginfoSet_2_Haftara();
    void infoSet_2_Yerushalmi();
    bool flaginfoSet_2_Yerushalmi();
    void infoSet_2_Ivri();
    bool flaginfoSet_2_Ivri();
    void infoSet_2_Chabad();
    bool flaginfoSet_2_Chabad();

    // Names set
    void UpdateNameSet(uint8_t);
    void updateIluyNishmat(String*); 
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
    void stateLoop();
};
class ColorMiddleLine : public State
{
    public:
    void showStateScreen();
    void stateLoop();
};
class ColorBottomTLine : public State
{
    public:
    void showStateScreen();
    void stateLoop();
};
class ColorBottomVLine : public State
{
    public:
    void showStateScreen();
    void stateLoop();
};
class ColorYomLine : public State
{
    public:
    void showStateScreen();
    void stateLoop();
};
class ColorIvriLine : public State
{
    public:
    void showStateScreen();
    void stateLoop();
};
class ColorLoaziLine : public State
{
    public:
    void showStateScreen();
    void stateLoop();
};
class ColorParashaLine : public State
{
    public:
    void showStateScreen();
    void stateLoop();
};
class ColorParashaVLine : public State
{
    public:
    void showStateScreen();
    void stateLoop();
};
class ColorIluyLine : public State
{
    public:
    void showStateScreen();
    void stateLoop();
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
class SetColorYomLine : public State
{
    public:
    int colorTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetColorIvriLine : public State
{
    public:
    int colorTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetColorLoaziLine : public State
{
    public:
    int colorTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetColorParashaLine : public State
{
    public:
    int colorTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetColorParashaVLine : public State
{
    public:
    int colorTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};
class SetColorIluyLine : public State
{
    public:
    int colorTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};

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
class TohenIluyState : public State
{
    public:
    void showStateScreen();
    void stateLoop();
};
class TohenTimeState : public State
{
    public:
    void showStateScreen();
    void stateLoop();
};
class TohenInfoState : public State
{
    public:
    void showStateScreen();
    void stateLoop();
};
// class SetTohenIluyStateInsert : public State
// {
//     public:
//     void showStateScreen();
//     void okAction();
// };
class SetTohenIluyStateText : public State
{
    public:
    void showStateScreen();
};
// class SetTohenIluyStateInsertSet : public State
// {
//     public:
//     String txtTemp="";
//     String txt="";
//     bool sending;
//     uint8_t counter;
//     void clear_BT_buffer(bool messages);
//     void ExceptionBT(const char*);
//     void showStateScreen();
//     void stateLoop();
//     void menuAction();
//     void okAction();
// };
class SetTohenIluyStateTextList : public State
{
    public:
    bool validateTextLen(String*);
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
};
// class SetTohenIluyStateEdit : public State
// {
//     public:
//     void showStateScreen();
// };
class SetTohenIluyStateDelete : public State
{
    public:
    void showStateScreen();
    void okAction();
};

class SetTohenIluyStateKezev : public State
{
    public:
    void showStateScreen();
};
class SetTohenIluyStateKezevSet : public State
{
    public:
    uint16_t kezevTemp;
    void showStateScreen();
    void menuAction();
    void upAction();
    void downAction();
    void okAction();
};

class SetTohenBottomLineLoazi : public State
{
    public:
    void showStateScreen();
    void okAction();
};
class SetTohenTimeStateKezev : public State
{
    public:
    void showStateScreen();
};
class SetTohenTimeStateKezevSet : public State
{
    public:
    uint16_t kezevTemp;
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
class SetTohenBottomLineZSZ0 : public State
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
class SetTohenBottomLineZSZ1 : public State
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
class SetTohenBottomLineZSZ2 : public State
{
    public:
    void showStateScreen();
    void okAction();
};

class SetTohenInfoStateKezev : public State
{
    public:
    void showStateScreen();
};
class SetTohenInfoStateKezevSet : public State
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
class SetTohenBottomLineIORT : public State
{
    public:
    void showStateScreen();
};
class SetTohenBottomLineIORT0 : public State
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