#ifndef DB_H
#define DB_H

#include "LoaziDate.h"
#include "DataBase/common_db.h"
#include "DataBase/Tucazinsky_db.h"

#include "Place.h"

#define pgm_read_byte(addr)   (*(const unsigned char *)(addr))
#define pgm_read_byte_near(addr)  pgm_read_byte(addr)

const int commonValuesLen = 31;
enum CommonValuesDay {
    LOAZI_YOM,       // 0. יום לועזי
    LOAZI_HODESH,    // 1. חודש לועזי  
    LOAZI_SHANA,     // 2. שנה לועזי 
    IVRI_YOM,        // 3. יום עברי  
    IVRI_HODESH,     // 4. חודש עברי       
    IVRI_SHANA,      // 5. שנה עברי     
    PARASHA,         // 6. פרשה    
    PARASHA_HUL,     // 7. פרשה    
    PARASHA_BALADI,  // 8. פרשה    
    SHABAT_PLUS,     // 9. שבתות מיוחדות    
    DAF_YOMI_M_B,    // 10. דף יומי בבלי - מסכת    
    DAF_YOMI_D_B,    // 11. דף יומי בבלי - דף    
    DAF_YOMI_M_Y,    // 12. דף יומי ירושלמי - מסכת    
    DAF_YOMI_D_Y,    // 13. דף יומי ירושלמי - דף    
    ONA,             // 14. עונה    
    OMER,            // 15. יום בעומר    
    ROSH_HODESH,     // 16. ראש חודש    
    HOLIDAY,         // 17. יום מיוחד    
    HOLIDAY_TYPE,    // 18. סוג היום המיוחד
    // 1. ערב חג
    // 2. חג
    // 3. ערב צום
    // 4. צום
    // 5. חג ישראלי
    // 6. חנוכה
    // 7. פורים
    HOLIDAY_HUL,       // 19. יום מיוחד בחו"ל   
    HOLIDAY_TYPE_HUL,  // 20. סוג היום המיוחד בחו"ל
    TFILA_PLUS_1,      // 21. שיוניים בתפילה חלק א   
    // 1. משיב הרוח
    // 2. מוריד הטל
    // 3. ברך עלינו
    // 4. ברכנו
    // 5. א"א תחנון
    // 6. א"א צדקתך 
    // 7. יעלה ויבוא 
    // 8. על הניסים 
    TFILA_PLUS_2,       // 22. שינויים בתפילה חלק ב   
    // 1. עננו
    // 2. ערוב תבשילין
    // 3. הלל שלם
    // 4. הלל בדילוג
    // 5. אתה יצרת 
    // 6. ותודיענו 
    // 7. המלך הקדוש
    // 8. המלך המשפט  
    SHAON,            // 23. שעון משתנה לקיץ/חורף
    // 0. לא משתנה השעון היום
    // 1. משתנה לשעון חורף - שעה אחורה
    // 2. משתנה לשעון קיץ - שעה קדימה
    SHAON_KAIZ,       // 24. שעון קיץ/חורף
    // 0. שעון חורף
    // 1. שעון קיץ
    MOLAD_YOM,        // 25. מולד - יום
    MOLAD_SHAA,       // 26. מולד - שעה
    MOLAD_DAKA,       // 27. מולד - דקות
    MOLAD_HALAKIM,    // 28. מולד - חלקים
    NEXT_ROSH_HODESH, // 29. ימי ראש החודש הקרובים
    YOM_CHABAD,       // 30. יום חב"ד
};

const int kiseRachamimValuesLen = 30;
enum KiseRachamimValuesDay {
    KISE_ZRIHA_S,            // 0. זריחה שעות
    KISE_ZRIHA_D,            // 1. זריחה דקות
    KISE_SHKIAA_S,           // 2. שקיעה שעות 
    KISE_SHKIAA_D,           // 3. שקיעה דקות 
    KISE_ALOT_S,             // 4. עלות השחר שעות
    KISE_ALOT_D,             // 5. עלות השחר דקות
    KISE_TALIT_S,            // 6. זמן טלית ותפילין שעות
    KISE_TALIT_D,            // 7. זמן טלית ותפילין דקות
    KISE_SOZ_SHMA_MGA_S,     // 8. סוף זמן קריאת שמע מג"א שעות
    KISE_SOZ_SHMA_MGA_D,     // 9. סוף זמן קריאת שמע מג"א דקות
    KISE_SOZ_SHMA_GRA_S,     // 10. סוף זמן קריאת שמע גר"א שעות
    KISE_SOZ_SHMA_GRA_D,     // 11. סוף זמן קריאת שמע גר"א דקות
    KISE_SOZ_TFILA_GRA_S,    // 12. סוף זמן תפילה גר"א שעות
    KISE_SOZ_TFILA_GRA_D,    // 13. סוף זמן תפילה גר"א דקות
    KISE_HAZOT_S,            // 14. חצות היום שעות
    KISE_HAZOT_D,            // 15. חצות היום דקות
    KISE_MINHA_GDOLA_S,      // 16. מנחה גדולה שעות
    KISE_MINHA_GDOLA_D,      // 17. מנחה גדולה דקות
    KISE_MINHA_KTANA_S,      // 18. מנחה קטנה שעות
    KISE_MINHA_KTANA_D,      // 19. מנחה קטנה דקות
    KISE_PLAG_MINHA_S,       // 20. פלג המנחה שעות
    KISE_PLAG_MINHA_D,       // 21. פלג המנחה דקות
    KISE_TZET_COCHAVIM_S,    // 22. צאת הכוכבים שעות
    KISE_TZET_COCHAVIM_D,    // 23. צאת הכוכבים דקות
    KISE_SHABAT_IN_S,        // 24. כניסת שבת שעות
    KISE_SHABAT_IN_D,        // 25. כניסת שבת דקות
    KISE_SHABAT_OUT_S,       // 26. צאת שבת שעות
    KISE_SHABAT_OUT_D,       // 27. צאת שבת דקות
    KISE_RABENU_TAM_S,       // 28. צאת שבת רבינו תם שעות
    KISE_RABENU_TAM_D,       // 29. צאת שבת רבינו תם דקות
};

const int orHachaimValuesLen = 20;
enum OrHachaimValuesDay {
    OR_ALOT_S,             // 0. עלות השחר שעות
    OR_ALOT_D,             // 1. עלות השחר דקות
    OR_ZRIHA_S,            // 2. זריחה שעות
    OR_ZRIHA_D,            // 3. זריחה דקות
    OR_SHKIAA_S,           // 4. שקיעה שעות 
    OR_SHKIAA_D,           // 5. שקיעה דקות 
    OR_TZET_COCHAVIM_S,    // 6. צאת הכוכבים שעות
    OR_TZET_COCHAVIM_D,    // 7. צאת הכוכבים דקות
    OR_SOZ_SHMA_MGA_S,     // 8. סוף זמן קריאת שמע מג"א שעות
    OR_SOZ_SHMA_MGA_D,     // 9. סוף זמן קריאת שמע מג"א דקות
    OR_SOZ_SHMA_GRA_S,     // 10. סוף זמן קריאת שמע גר"א שעות
    OR_SOZ_SHMA_GRA_D,     // 11. סוף זמן קריאת שמע גר"א דקות
    OR_SOZ_TFILA_GRA_S,    // 12. סוף זמן תפילה גר"א שעות
    OR_SOZ_TFILA_GRA_D,    // 13. סוף זמן תפילה גר"א דקות
    OR_MINHA_GDOLA_S,      // 14. מנחה גדולה שעות
    OR_MINHA_GDOLA_D,      // 15. מנחה גדולה דקות
    OR_MINHA_KTANA_S,      // 16. מנחה קטנה שעות
    OR_MINHA_KTANA_D,      // 17. מנחה קטנה דקות
    OR_PLAG_MINHA_S,       // 18. פלג המנחה שעות
    OR_PLAG_MINHA_D,       // 19. פלג המנחה דקות
};

const int chaiValuesLen = 6;
enum ChaiValuesDay {
    CHAI_ZRIHA_N_H,          // 0. זריחה הנראית שעות
    CHAI_ZRIHA_N_M,          // 1. זריחה הנראית דקות
    CHAI_ZRIHA_N_S,          // 2. זריחה הנראית שניות
    CHAI_SHKIAA_N_H,         // 3. שקיעה הנראית שעות 
    CHAI_SHKIAA_N_M,         // 4. שקיעה הנראית דקות 
    CHAI_SHKIAA_N_S,         // 5. שקיעה הנראית שניות 
};

const int tucValuesLen = 12;
enum TucValuesDay {
    TUC_ALOT_H,          // 0. עלות השחר שעות
    TUC_ALOT_M,          // 1. עלות השחר דקות
    TUC_ZRIHA_H,         // 2. זריחה שעות
    TUC_ZRIHA_M,         // 3. זריחה דקות 
    TUC_SOZKS_H,         // 4. סוף זמן קריאת שמע שעות
    TUC_SOZKS_M,         // 5. סוף זמן קריאת שמע דקות 
    TUC_MINHAG_H,        // 6. מנחה גדולה שעות
    TUC_MINHAG_M,        // 7. מנחה גדולה דקות 
    TUC_SHKIAA_H,        // 8. שקיעה שעות 
    TUC_SHKIAA_M,        // 9. שקיעה דקות 
    TUC_MOZSH_H,         // 10. צאת שבת שעות
    TUC_MOZSH_M,         // 11. צאת שבת דקות 
};

//void LoadDayValuse(int, DayInfo*, const Place&);
class DB
{
public:
    //common
    uint8_t ivriYom;
    uint8_t ivriHodesh;
    uint8_t ivriShana;
    uint8_t ivriYomTomorrow;
    uint8_t ivriHodeshTomorrow;
    uint8_t ivriShanaTomorrow;
    uint8_t parasha;
    uint8_t parasha_hul;
    uint8_t parasha_baladi;
    uint8_t shabatPlus;
    uint8_t dafYomiBM;
    uint8_t dafYomiBD;
    uint8_t dafYomiYM;
    uint8_t dafYomiYD;
    uint8_t ona;
    uint8_t omer;
    uint8_t roshHodesh;
    uint8_t holiday;
    uint8_t holidayType;
    uint8_t holiday_hul;
    uint8_t holidayType_hul;
    uint8_t tfilaPlus1;
    uint8_t tfilaPlus2;
    uint8_t shaon;
    uint8_t shaonKaiz;
    uint8_t molad_yom;
    uint8_t molad_shaa;
    uint8_t molad_daka;
    uint8_t molad_halakim;
    uint8_t next_rosh_hodesh;
    uint8_t next_rosh_hodesh_tomorrow;
    uint8_t next_hodesh;
    uint8_t yom_chabad;

    // or hachaim
    uint8_t OR_zrihaShaa;
    uint8_t OR_zrihaDaka;
    uint8_t OR_shkiaaShaa;
    uint8_t OR_shkiaaDaka;
    uint8_t OR_alotHashaharShaa;
    uint8_t OR_alotHashaharDaka;
    uint8_t OR_sofZmanShmaMgaShaa;
    uint8_t OR_sofZmanShmaMgaDaka;
    uint8_t OR_sofZmanShmaGraShaa;
    uint8_t OR_sofZmanShmaGraDaka;
    uint8_t OR_sofZmanTfilaGraShaa;
    uint8_t OR_sofZmanTfilaGraDaka;
    uint8_t OR_minhaGdolaShaa;
    uint8_t OR_minhaGdolaDaka;
    uint8_t OR_minhaKtanaShaa;
    uint8_t OR_minhaKtanaDaka;
    uint8_t OR_plagHaminhaShaa;
    uint8_t OR_plagHaminhaDaka;
    uint8_t OR_tzetHacochavimShaa;
    uint8_t OR_tzetHacochavimDaka;

    // kise rachamim
    uint8_t KISE_zrihaShaa;
    uint8_t KISE_zrihaDaka;
    uint8_t KISE_shkiaaShaa;
    uint8_t KISE_shkiaaDaka;
    uint8_t KISE_alotHashaharShaa;
    uint8_t KISE_alotHashaharDaka;
    uint8_t KISE_zmanTalitShaa;
    uint8_t KISE_zmanTalitDaka;
    uint8_t KISE_sofZmanShmaMgaShaa;
    uint8_t KISE_sofZmanShmaMgaDaka;
    uint8_t KISE_sofZmanShmaGraShaa;
    uint8_t KISE_sofZmanShmaGraDaka;
    uint8_t KISE_sofZmanTfilaGraShaa;
    uint8_t KISE_sofZmanTfilaGraDaka;
    uint8_t KISE_hatsotShaa;
    uint8_t KISE_hatsotDaka;
    uint8_t KISE_minhaGdolaShaa;
    uint8_t KISE_minhaGdolaDaka;
    uint8_t KISE_minhaKtanaShaa;
    uint8_t KISE_minhaKtanaDaka;
    uint8_t KISE_plagHaminhaShaa;
    uint8_t KISE_plagHaminhaDaka;
    uint8_t KISE_tzetHacochavimShaa;
    uint8_t KISE_tzetHacochavimDaka;
    uint8_t KISE_shabatInShaa;
    uint8_t KISE_shabatInDaka;
    uint8_t KISE_shabatOutShaa;
    uint8_t KISE_shabatOutDaka;
    uint8_t KISE_rabenuTamShaa;
    uint8_t KISE_rabenuTamDaka;

    // luchot chai
    uint8_t CHAI_zrihaNShaa;
    uint8_t CHAI_zrihaNDaka;
    uint8_t CHAI_zrihaNSnia;
    uint8_t CHAI_shkiaaNShaa;
    uint8_t CHAI_shkiaaNDaka;
    uint8_t CHAI_shkiaaNSnia;

    // Tucazinsky
    uint8_t TUC_alotShaa;
    uint8_t TUC_alotDaka;
    uint8_t TUC_zrihaShaa;
    uint8_t TUC_zrihaDaka;
    uint8_t TUC_sozksShaa;
    uint8_t TUC_sozksDaka;
    uint8_t TUC_minhaGShaa;
    uint8_t TUC_minhaGDaka;
    uint8_t TUC_shkiaaShaa;
    uint8_t TUC_shkiaaDaka;
    uint8_t TUC_mozshShaa;
    uint8_t TUC_mozshDaka;

    // loaders
    void LoadCommonValues(int days);
    void LoadOrValues(const LoaziDate& date, const uint8_t* db);
    void LoadKiseValues(const LoaziDate& date, const uint8_t* db);
    void LoadChaiValues(const LoaziDate& date, const uint8_t* db);
    void LoadTucValues(const LoaziDate& date, const uint8_t* db);
    void LoadDayValues(const LoaziDate& date, int days, const Place* ofek);
};


#endif