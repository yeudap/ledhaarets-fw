#ifndef DAYINFO_H
#define DAYINFO_H

#include "LoaziDate.h"
#include "HebrewDate.h"
#include "Zman.h"
#include "DafYomi.h"
#include "Strings.h"
#include "Preferences/UserPreferences.h"
#include "Sun.h"
#include "DB.h"
#include "Shita.h"

extern UserPreferences userPreferences;


class DayInfo
{
public:
    DayInfo* yesterday = nullptr;
    DayInfo* tomorrow = nullptr;
    DayInfo* nextShishi = nullptr;
    DayInfo* nextShabat = nullptr;
    Sun sun;
    
    // Common values
    uint8_t dayInWeek;
    LoaziDate loaziDate;
    HebrewDate hebrewDate;
    HebrewDate hebrewDateTomorrow;
    uint8_t parasha;
    uint8_t parasha_hul;
    uint8_t parasha_baladi;
    uint8_t haftara;
    uint8_t shabatPlus;
    DafYomi dafYomiBavli;
    DafYomi dafYomiYerushalmi;
    uint8_t ona;
    uint8_t omer;
    uint8_t holiday;
    uint8_t holidayType;
    uint8_t holiday_hul;
    uint8_t holidayType_hul;
    uint8_t roshHodesh;
    uint8_t tfilaPlus1;
    uint8_t tfilaPlus2;
    uint8_t shaon;
    uint8_t shaonKaiz;
    uint8_t molad_yom;
    uint8_t molad_shaa;
    uint8_t molad_daka;
    uint8_t molad_halakim;
    uint8_t next_rosh_hodesh;
    uint8_t next_hodesh;
    uint8_t yom_chabad;
    uint16_t shanimLahurban;
    bool isRoshHodesh;
    bool isShabatMevarchin;
    bool isShabatPlus;
    bool isShishi;
    bool isShabat;
    bool isMozaeiShabat;
    bool isErevHag;
    bool isHag;
    bool isMozaeiHag;
    bool isHagIsraeli;
    bool isAzmaut;
    bool isYomHashoa;
    bool isYomHazicaron;
    bool isYomYerushalaim;
    bool isErevZom;
    bool isZom;
    bool isMozaeiZom;
    bool isErevTishaBeav;
    bool isTishaBeav;
    bool isMozaeiTishaBeav;
    bool isPesach;
    bool isSimchatTora;
    bool isShminiAzeret;
    bool isHanuka; 
    bool isPurim; 
    bool isShushanPurim; 
    bool isErevRoshHashana; 
    bool isErevYomKipur; 
    bool isYomKipur; 
    bool isMozaeiKipur; 
    bool isPesachSheni; 
    bool isMoed; 
    bool isOmer; 
    bool isYomChabad; 
    bool mashivHaruah;
    bool moridHatal;
    bool barechAlenu;
    bool barchenu;
    bool enTahanun;
    bool enZidkatcha;
    bool yaaleVeyavo;
    bool alHanisim;
    bool anenu;
    bool eruvTavshilin;
    bool halelShalem;
    bool halelDilug;
    bool ataYazarta;
    bool vatodienu;
    bool hamelechHakadosh;
    bool hamelechHamishpat;
    bool afterShkiaa;
    bool zmanMinha;
    bool isShaonKaiz;
    
    // Ofek values
    Zman zrihaM;
    Zman zrihaN;
    Zman zrihaA;
    Zman shkiaaM;
    Zman shkiaaN;
    Zman shkiaaA;
    Zman alot90;
    Zman alot72;
    Zman alotHashahar;
    Zman zmanTalit;
    Zman sofZmanShmaMga;
    Zman sofZmanShmaGra;
    Zman sofZmanTfilaMga;
    Zman sofZmanTfilaGra;
    Zman hatsot;
    Zman minhaGdola;
    Zman minhaKtana;
    Zman plagHaminha;
    Zman zeet90;
    Zman zeet72;
    Zman tzetHacochavim;
    Zman shabatIn;
    Zman shabatOut;
    Zman rabenuTam;
    Zman tzomIn;
    Zman tzomOut;
    Zman tBeavIn;
    Zman tBeavOut;
    Zman kipurIn;
    Zman kipurOut;
    Zman sofZmanBeiurHamez;
    Zman hazotLayla;
    
    // Chai values
    Zman chai_zrihaN;
    Zman chai_zrihaM;
    Zman chai_zrihaA;
    Zman chai_shkiaaN;
    Zman chai_shkiaaM;
    Zman chai_shkiaaA;
    
    // Or hachaim values
    Zman or_zriha;
    Zman or_shkiaa;
    Zman or_alotHashahar;
    Zman or_sofZmanShmaMga;
    Zman or_sofZmanShmaGra;
    Zman or_sofZmanTfilaGra;
    Zman or_minhaGdola;
    Zman or_minhaKtana;
    Zman or_plagHaminha;
    Zman or_tzetHacochavim;

    // Kise rhachmim values
    Zman kise_zriha;
    Zman kise_shkiaa;
    Zman kise_alotHashahar;
    Zman kise_zmanTalit;
    Zman kise_sofZmanShmaMga;
    Zman kise_sofZmanShmaGra;
    Zman kise_sofZmanTfilaGra;
    Zman kise_hatsot;
    Zman kise_minhaGdola;
    Zman kise_minhaKtana;
    Zman kise_plagHaminha;
    Zman kise_tzetHacochavim;
    Zman kise_shabatIn;
    Zman kise_shabatOut;
    Zman kise_rabenuTam;
    
    void UpdateCommonValues();
    void UpdateChaiValues(bool kaiz);
    void UpdateOrValues(bool kaiz);
    void UpdateKiseValues(bool kaiz);
    void UpdateTucValues(bool kaiz);

    void initDay(int idx, DateTime* now, Place* place);
    void initSun(Place*);
    void setHul();
    void setZom();
    void setHazot();
    void setTahanun();
    void setBaladi();
    void setShanimLahurban();
    void setNewDay();
    void setAfterShkiaa();
    void setZmanMinha();
    void setBooleanValues(uint8_t _shabatPlus, uint8_t _holidayType, uint8_t _roshHodesh, uint8_t _tpilaPlus1, uint8_t _tfilaPlus2, uint8_t _shaonKaiz);
    void setShita(Shita*);
    void setLoaziDate(DateTime*);
    void setShaonKaitzHoref(LoaziDate&, Place*);
    void setHaftara();
    void setHftaraMinha();
    void printDayInfo();
    
    void getParasha(char* _parasha);
    void getHaftara(char* shem, char* makor);
    void getShabatPlus(char* _shabatPlus);
    void getOna(char* _ona);
    void getDayPart(char* _part, uint8_t hour);
    void getOmer(char* line1, char* line2);
    void getHanuka(char* _ner);
    void getShanimLamdina(char* _txt);
    void getShanimLyerushalaim(char* _txt);
    void getShanimLahurban(char* _txt);
    void getYomOrLeil(char* _yomTitle);
    void getDayInWeek(char* _dayInWeek, bool yom);
    void getHoliday(char* _holiday);
    void getRoshHodesh(char* _hodesh);
    void getMolad(char* _moladUp, char* _moladDown);
    void getNextRoshHodesh(char* _hodesh_up, char* _hodesh_down);
    void getYomHabad(char* _cahbad, uint8_t idx);
    const char* getBircatHashanim();
    const char* getGeshemTal();

    Zman getZriha();
    Zman getShkiaa();
    Zman getZeetCochavim();
    Zman getHagIn();
    Zman getHagOut();

    bool isNewDay(DateTime*);
    bool isAfterShkiaa(DateTime*);
    bool isZmanMinha(DateTime*);
    bool isYomHol();

};

#endif