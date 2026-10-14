#include "DB.h"

DB db;

void DB::LoadCommonValues(int days)
{
    const uint8_t* i = common_db + days * commonValuesLen;
    this->ivriYom = pgm_read_byte_near(i + IVRI_YOM);
    this->ivriHodesh = pgm_read_byte_near(i + IVRI_HODESH);
    this->ivriShana = pgm_read_byte_near(i + IVRI_SHANA);
    this->ivriYomTomorrow = pgm_read_byte_near(i + commonValuesLen + IVRI_YOM);
    this->ivriHodeshTomorrow = pgm_read_byte_near(i + commonValuesLen + IVRI_HODESH);
    this->ivriShanaTomorrow = pgm_read_byte_near(i + commonValuesLen + IVRI_SHANA);
    this->parasha = pgm_read_byte_near(i + PARASHA);
    this->parasha_hul = pgm_read_byte_near(i + PARASHA_HUL);
    this->parasha_baladi = pgm_read_byte_near(i + PARASHA_BALADI);
    this->shabatPlus = pgm_read_byte_near(i + SHABAT_PLUS);
    this->dafYomiBM = pgm_read_byte_near(i + DAF_YOMI_M_B);
    this->dafYomiBD = pgm_read_byte_near(i + DAF_YOMI_D_B);
    this->dafYomiYM = pgm_read_byte_near(i + DAF_YOMI_M_Y);
    this->dafYomiYD = pgm_read_byte_near(i + DAF_YOMI_D_Y);
    this->ona = pgm_read_byte_near(i + ONA);
    this->omer = pgm_read_byte_near(i + OMER);
    this->roshHodesh = pgm_read_byte_near(i + ROSH_HODESH);
    this->holiday = pgm_read_byte_near(i + HOLIDAY);
    this->holidayType = pgm_read_byte_near(i + HOLIDAY_TYPE);
    this->holiday_hul = pgm_read_byte_near(i + HOLIDAY_HUL);
    this->holidayType_hul = pgm_read_byte_near(i + HOLIDAY_TYPE_HUL);
    this->tfilaPlus1 = pgm_read_byte_near(i + TFILA_PLUS_1);
    this->tfilaPlus2 = pgm_read_byte_near(i + TFILA_PLUS_2);
    this->shaon = pgm_read_byte_near(i + SHAON);
    this->shaonKaiz = pgm_read_byte_near(i + SHAON_KAIZ);
    this->molad_yom = pgm_read_byte_near(i + MOLAD_YOM);
    this->molad_shaa = pgm_read_byte_near(i + MOLAD_DAKA);
    this->molad_daka = pgm_read_byte_near(i + MOLAD_SHAA);
    this->molad_halakim = pgm_read_byte_near(i + MOLAD_HALAKIM);
    this->next_rosh_hodesh = pgm_read_byte_near(i + NEXT_ROSH_HODESH);
    this->yom_chabad = pgm_read_byte_near(i + YOM_CHABAD);
    this->next_rosh_hodesh_tomorrow = pgm_read_byte_near(i + commonValuesLen + NEXT_ROSH_HODESH);
    this->next_hodesh = 0;
    if(this->next_rosh_hodesh || this->next_rosh_hodesh_tomorrow) 
    {
        this->next_hodesh = pgm_read_byte_near(i + commonValuesLen*8 + IVRI_HODESH); 
    }
}

void DB::LoadOrValues(const LoaziDate& date, const uint8_t* db)
{
    int dayOffset = (date.day-1) * 12 * orHachaimValuesLen;
    int monthOffset = (date.month-1) * orHachaimValuesLen;
    const uint8_t* i = db + dayOffset + monthOffset;
    OR_zrihaShaa = pgm_read_byte_near(i + OR_ZRIHA_S);
    OR_zrihaDaka = pgm_read_byte_near(i + OR_ZRIHA_D);
    OR_shkiaaShaa = pgm_read_byte_near(i + OR_SHKIAA_S);
    OR_shkiaaDaka = pgm_read_byte_near(i + OR_SHKIAA_D);
    OR_alotHashaharShaa = pgm_read_byte_near(i + OR_ALOT_S);
    OR_alotHashaharDaka = pgm_read_byte_near(i + OR_ALOT_D);
    OR_sofZmanShmaMgaShaa = pgm_read_byte_near(i + OR_SOZ_SHMA_MGA_S);
    OR_sofZmanShmaMgaDaka = pgm_read_byte_near(i + OR_SOZ_SHMA_MGA_D);
    OR_sofZmanShmaGraShaa = pgm_read_byte_near(i + OR_SOZ_SHMA_GRA_S);
    OR_sofZmanShmaGraDaka = pgm_read_byte_near(i + OR_SOZ_SHMA_GRA_D);
    OR_sofZmanTfilaGraShaa = pgm_read_byte_near(i + OR_SOZ_TFILA_GRA_S);
    OR_sofZmanTfilaGraDaka = pgm_read_byte_near(i + OR_SOZ_TFILA_GRA_D);
    OR_minhaGdolaShaa = pgm_read_byte_near(i + OR_MINHA_GDOLA_S);
    OR_minhaGdolaDaka = pgm_read_byte_near(i + OR_MINHA_GDOLA_D);
    OR_minhaKtanaShaa = pgm_read_byte_near(i + OR_MINHA_KTANA_S);
    OR_minhaKtanaDaka = pgm_read_byte_near(i + OR_MINHA_KTANA_D);
    OR_plagHaminhaShaa = pgm_read_byte_near(i + OR_PLAG_MINHA_S);
    OR_plagHaminhaDaka = pgm_read_byte_near(i + OR_PLAG_MINHA_D);
    OR_tzetHacochavimShaa = pgm_read_byte_near(i + OR_TZET_COCHAVIM_S);
    OR_tzetHacochavimDaka = pgm_read_byte_near(i + OR_TZET_COCHAVIM_D);
}
void DB::LoadKiseValues(const LoaziDate& date, const uint8_t* db)
{
    int dayOffset = (date.day-1) * 12 * 4 * kiseRachamimValuesLen; // 12 month, 4 years period, one row
    int monthOffset = (date.month-1) * kiseRachamimValuesLen;
    int yearOffset = (date.year%4) * 12 * kiseRachamimValuesLen;
    const uint8_t* i = db + dayOffset + monthOffset + yearOffset;
    KISE_zrihaShaa = pgm_read_byte_near(i + KISE_ZRIHA_S);
    KISE_zrihaDaka = pgm_read_byte_near(i + KISE_ZRIHA_D);
    KISE_shkiaaShaa = pgm_read_byte_near(i + KISE_SHKIAA_S);
    KISE_shkiaaDaka = pgm_read_byte_near(i + KISE_SHKIAA_D);
    KISE_alotHashaharShaa = pgm_read_byte_near(i + KISE_ALOT_S);
    KISE_alotHashaharDaka = pgm_read_byte_near(i + KISE_ALOT_D);
    KISE_zmanTalitShaa = pgm_read_byte_near(i + KISE_TALIT_S);
    KISE_zmanTalitDaka = pgm_read_byte_near(i + KISE_TALIT_D);
    KISE_sofZmanShmaMgaShaa = pgm_read_byte_near(i + KISE_SOZ_SHMA_MGA_S);
    KISE_sofZmanShmaMgaDaka = pgm_read_byte_near(i + KISE_SOZ_SHMA_MGA_D);
    KISE_sofZmanShmaGraShaa = pgm_read_byte_near(i + KISE_SOZ_SHMA_GRA_S);
    KISE_sofZmanShmaGraDaka = pgm_read_byte_near(i + KISE_SOZ_SHMA_GRA_D);
    KISE_sofZmanTfilaGraShaa = pgm_read_byte_near(i + KISE_SOZ_TFILA_GRA_S);
    KISE_sofZmanTfilaGraDaka = pgm_read_byte_near(i + KISE_SOZ_TFILA_GRA_D);
    KISE_hatsotShaa = pgm_read_byte_near(i + KISE_HAZOT_S);
    KISE_hatsotDaka = pgm_read_byte_near(i + KISE_HAZOT_D);
    KISE_minhaGdolaShaa = pgm_read_byte_near(i + KISE_MINHA_GDOLA_S);
    KISE_minhaGdolaDaka = pgm_read_byte_near(i + KISE_MINHA_GDOLA_D);
    KISE_minhaKtanaShaa = pgm_read_byte_near(i + KISE_MINHA_KTANA_S);
    KISE_minhaKtanaDaka = pgm_read_byte_near(i + KISE_MINHA_KTANA_D);
    KISE_plagHaminhaShaa = pgm_read_byte_near(i + KISE_PLAG_MINHA_S);
    KISE_plagHaminhaDaka = pgm_read_byte_near(i + KISE_PLAG_MINHA_D);
    KISE_tzetHacochavimShaa = pgm_read_byte_near(i + KISE_TZET_COCHAVIM_S);
    KISE_tzetHacochavimDaka = pgm_read_byte_near(i + KISE_TZET_COCHAVIM_D);
    KISE_shabatInShaa = pgm_read_byte_near(i + KISE_SHABAT_IN_S);
    KISE_shabatInDaka = pgm_read_byte_near(i + KISE_SHABAT_IN_D);
    KISE_shabatOutShaa = pgm_read_byte_near(i + KISE_SHABAT_OUT_S);
    KISE_shabatOutDaka = pgm_read_byte_near(i + KISE_SHABAT_OUT_D);
    KISE_rabenuTamShaa = pgm_read_byte_near(i + KISE_RABENU_TAM_S);
    KISE_rabenuTamDaka = pgm_read_byte_near(i + KISE_RABENU_TAM_D);
}
void DB::LoadChaiValues(const LoaziDate& date, const uint8_t* db)
{
    int dayOffset = (date.day-1) * 12 * 4 * chaiValuesLen;
    int monthOffset = (date.month-1) * chaiValuesLen;
    int yearOffset = (date.year%4) * 12 * chaiValuesLen;
    const uint8_t* i = db + dayOffset + monthOffset + yearOffset;
    CHAI_zrihaNShaa = pgm_read_byte_near(i + CHAI_ZRIHA_N_H);
    CHAI_zrihaNDaka = pgm_read_byte_near(i + CHAI_ZRIHA_N_M);
    CHAI_zrihaNSnia = pgm_read_byte_near(i + CHAI_ZRIHA_N_S);
    CHAI_shkiaaNShaa = pgm_read_byte_near(i + CHAI_SHKIAA_N_H);
    CHAI_shkiaaNDaka = pgm_read_byte_near(i + CHAI_SHKIAA_N_M);
    CHAI_shkiaaNSnia = pgm_read_byte_near(i + CHAI_SHKIAA_N_S);

    // Serial.println("______________________________");
    // Serial.println("LoadChaiValues");
    // char buf[10];
    // sprintf(buf, "%02d:%02d:%02d", CHAI_zrihaNShaa, CHAI_zrihaNDaka, CHAI_zrihaNSnia);
    // Serial.print("CHAI_zriha: ");
    // Serial.println(buf);
    // sprintf(buf, "%02d:%02d:%02d", CHAI_shkiaaNShaa, CHAI_shkiaaNDaka, CHAI_shkiaaNSnia);
    // Serial.print("CHAI_shkiaa: ");
    // Serial.println(buf);
    // Serial.println("______________________________");
}
void DB::LoadTucValues(const LoaziDate& date, const uint8_t* db)
{
    int dayOffset = (date.day-1) * 12 * tucValuesLen;
    int monthOffset = (date.month-1) * tucValuesLen;
    const uint8_t* i = db + dayOffset + monthOffset;
    TUC_alotShaa   = pgm_read_byte_near(i + TUC_ALOT_H);
    TUC_alotDaka   = pgm_read_byte_near(i + TUC_ALOT_M);
    TUC_zrihaShaa  = pgm_read_byte_near(i + TUC_ZRIHA_H);
    TUC_zrihaDaka  = pgm_read_byte_near(i + TUC_ZRIHA_M);
    TUC_sozksShaa  = pgm_read_byte_near(i + TUC_SOZKS_H);
    TUC_sozksDaka  = pgm_read_byte_near(i + TUC_SOZKS_M);
    TUC_minhaGShaa = pgm_read_byte_near(i + TUC_MINHAG_H);
    TUC_minhaGDaka = pgm_read_byte_near(i + TUC_MINHAG_M);
    TUC_shkiaaShaa = pgm_read_byte_near(i + TUC_SHKIAA_H);
    TUC_shkiaaDaka = pgm_read_byte_near(i + TUC_SHKIAA_M);
    TUC_mozshShaa  = pgm_read_byte_near(i + TUC_MOZSH_H);
    TUC_mozshDaka  = pgm_read_byte_near(i + TUC_MOZSH_M);
}
void DB::LoadDayValues(const LoaziDate& date, int days, const Place* place)
{
    LoadCommonValues(days);
    // Serial.println("LoadCommonValues done.");
    if(place->luchotChai != nullptr) 
    { 
        LoadChaiValues(date, place->luchotChai); 
        // Serial.println("LoadChaiValues done.");
    }
    if(place->orHachaim != nullptr && userPreferences.shita == 0) 
    { 
        LoadOrValues(date, place->orHachaim); 
        // Serial.println("LoadOrValues done.");
    }
    if(place->kiseRahamim != nullptr && userPreferences.shita == 1) 
    { 
        LoadKiseValues(date, place->kiseRahamim); 
        // Serial.println("LoadKiseValues done.");
    }
    if(userPreferences.shita == 6) 
    { 
        LoadTucValues(date, Tucazinsky_db); 
        // Serial.println("LoadTucValues done.");
    }
}
