#include "DayInfo.h"

extern Strings strings;
extern DB db;
extern Shita* shitot[numOfShitot];

void DayInfo::initDay(int idx, DateTime* now, Place* place)
{
    LoaziDate date = LoaziDate(now->day(), now->month(), now->year());
    db.LoadDayValues(date, idx, place);
    // Serial.println("LoadDayValues done.");
    UpdateCommonValues();
    // Serial.println("UpdateCommonValues done.");
    setLoaziDate(now);
    setShaonKaitzHoref(date, place);
    initSun(place);
    // Serial.println("initSun done.");
    if(place->luchotChai) { UpdateChaiValues(place->kaiz); }
    if(userPreferences.shita == 5) { zrihaA = zrihaA-4; shkiaaA = shkiaaA+4; }
    setShita(shitot[userPreferences.shita]);
    // Serial.println("setShita done.");
    if(place->orHachaim && userPreferences.shita == 0) { UpdateOrValues(place->kaiz); }
    if(place->kiseRahamim && userPreferences.shita == 1) { UpdateKiseValues(place->kaiz); }
    if(userPreferences.shita == 6) { UpdateTucValues(place->kaiz); }
}
void DayInfo::UpdateCommonValues()
{
    this->hebrewDate.yom = db.ivriYom;
    this->hebrewDate.hodesh = db.ivriHodesh;
    this->hebrewDate.shana = db.ivriShana;
    this->hebrewDateTomorrow.yom = db.ivriYomTomorrow;
    this->hebrewDateTomorrow.hodesh = db.ivriHodeshTomorrow;
    this->hebrewDateTomorrow.shana = db.ivriShanaTomorrow;
    this->parasha = db.parasha;
    this->parasha_hul = db.parasha_hul;
    this->parasha_baladi = db.parasha_baladi;
    this->shabatPlus = db.shabatPlus%20;
    this->dafYomiBavli.masechet = db.dafYomiBM;
    this->dafYomiBavli.daf = db.dafYomiBD;
    this->dafYomiYerushalmi.masechet = db.dafYomiYM;
    this->dafYomiYerushalmi.daf = db.dafYomiYD;
    this->omer = db.omer;
    this->ona = db.ona;
    this->holiday = db.holiday;
    this->holidayType = db.holidayType;
    this->holiday_hul = db.holiday_hul;
    this->holidayType_hul = db.holidayType_hul;
    this->roshHodesh = db.roshHodesh;
    this->tfilaPlus1 = db.tfilaPlus1;
    this->tfilaPlus2 = db.tfilaPlus2;
    this->shaon = db.shaon;
    this->shaonKaiz = db.shaonKaiz;
    this->molad_yom = db.molad_yom;
    this->molad_shaa = db.molad_shaa;
    this->molad_daka = db.molad_daka;
    this->molad_halakim = db.molad_halakim;
    this->next_rosh_hodesh = db.next_rosh_hodesh;
    this->next_hodesh = db.next_hodesh;
    this->yom_chabad = db.yom_chabad;
    setBooleanValues(db.shabatPlus, db.holidayType, db.roshHodesh, db.tfilaPlus1, db.tfilaPlus2, db.shaonKaiz);
}

void DayInfo::UpdateChaiValues(bool kaiz)
{
    this->zrihaN = Zman(db.CHAI_zrihaNShaa, db.CHAI_zrihaNDaka, db.CHAI_zrihaNSnia);
    this->shkiaaN = Zman(db.CHAI_shkiaaNShaa, db.CHAI_shkiaaNDaka, db.CHAI_shkiaaNSnia);
    if (kaiz)
    {
        this->zrihaN = this->zrihaN+60;
        this->shkiaaN = this->shkiaaN+60;
    }
}

void DayInfo::UpdateOrValues(bool kaiz)
{
    this->zrihaN = Zman(db.OR_zrihaShaa, db.OR_zrihaDaka);
    this->shkiaaA = Zman(db.OR_shkiaaShaa, db.OR_shkiaaDaka);
    this->alotHashahar = Zman(db.OR_alotHashaharShaa, db.OR_alotHashaharDaka);
    this->sofZmanShmaGra = Zman(db.OR_sofZmanShmaGraShaa, db.OR_sofZmanShmaGraDaka);
    this->sofZmanShmaMga = Zman(db.OR_sofZmanShmaMgaShaa, db.OR_sofZmanShmaMgaDaka);
    this->sofZmanTfilaGra = Zman(db.OR_sofZmanTfilaGraShaa, db.OR_sofZmanTfilaGraDaka);
    this->minhaGdola = Zman(db.OR_minhaGdolaShaa, db.OR_minhaGdolaDaka);
    this->minhaKtana = Zman(db.OR_minhaKtanaShaa, db.OR_minhaKtanaDaka);
    this->plagHaminha = Zman(db.OR_plagHaminhaShaa, db.OR_plagHaminhaDaka);
    this->tzetHacochavim = Zman(db.OR_tzetHacochavimShaa, db.OR_tzetHacochavimDaka);
    if (kaiz)
    {
        this->zrihaN = this->zrihaN+60;
        this->shkiaaA = this->shkiaaA+60;
        this->alotHashahar = this->alotHashahar+60;
        this->sofZmanShmaGra = this->sofZmanShmaGra+60;
        this->sofZmanShmaMga = this->sofZmanShmaMga+60;
        this->sofZmanTfilaGra = this->sofZmanTfilaGra+60;
        this->minhaGdola = this->minhaGdola+60;
        this->minhaKtana = this->minhaKtana+60;
        this->plagHaminha = this->plagHaminha+60;
        this->tzetHacochavim = this->tzetHacochavim+60;
    }
}

void DayInfo::UpdateKiseValues(bool kaiz)
{
    this->zrihaN = Zman(db.KISE_zrihaShaa, db.KISE_zrihaDaka);
    this->shkiaaA = Zman(db.KISE_shkiaaShaa, db.KISE_shkiaaDaka);
    this->alotHashahar = Zman(db.KISE_alotHashaharShaa, db.KISE_alotHashaharDaka);
    this->zmanTalit = Zman(db.KISE_zmanTalitShaa, db.KISE_zmanTalitDaka);
    this->sofZmanShmaGra = Zman(db.KISE_sofZmanShmaGraShaa, db.KISE_sofZmanShmaGraDaka);
    this->sofZmanShmaMga = Zman(db.KISE_sofZmanShmaMgaShaa, db.KISE_sofZmanShmaMgaDaka);
    this->sofZmanTfilaGra = Zman(db.KISE_sofZmanTfilaGraShaa, db.KISE_sofZmanTfilaGraDaka);
    this->hatsot = Zman(db.KISE_hatsotShaa, db.KISE_hatsotDaka);
    this->minhaGdola = Zman(db.KISE_minhaGdolaShaa, db.KISE_minhaGdolaDaka);
    this->minhaKtana = Zman(db.KISE_minhaKtanaShaa, db.KISE_minhaKtanaDaka);
    this->plagHaminha = Zman(db.KISE_plagHaminhaShaa, db.KISE_plagHaminhaDaka);
    this->tzetHacochavim = Zman(db.KISE_tzetHacochavimShaa, db.KISE_tzetHacochavimDaka);
    this->shabatIn = Zman(db.KISE_shabatInShaa, db.KISE_shabatInDaka);
    this->shabatOut = Zman(db.KISE_shabatOutShaa, db.KISE_shabatOutDaka);
    this->rabenuTam = Zman(db.KISE_rabenuTamShaa, db.KISE_rabenuTamDaka);
    if (kaiz)
    {
        this->zrihaN = this->zrihaN+60;
        this->shkiaaA = this->shkiaaA+60;
        this->zmanTalit = this->zmanTalit+60;
        this->alotHashahar = this->alotHashahar+60;
        this->sofZmanShmaGra = this->sofZmanShmaGra+60;
        this->sofZmanShmaMga = this->sofZmanShmaMga+60;
        this->sofZmanTfilaGra = this->sofZmanTfilaGra+60;
        this->hatsot = this->hatsot+60;
        this->minhaGdola = this->minhaGdola+60;
        this->minhaKtana = this->minhaKtana+60;
        this->plagHaminha = this->plagHaminha+60;
        this->tzetHacochavim = this->tzetHacochavim+60;
        this->shabatIn = this->shabatIn+60;
        this->shabatOut = this->shabatOut+60;
        this->rabenuTam = this->rabenuTam+60;
    }
}

void DayInfo::UpdateTucValues(bool kaiz)
{
    this->zrihaN = Zman(db.TUC_zrihaShaa, db.TUC_zrihaDaka);
    this->shkiaaA = Zman(db.TUC_shkiaaShaa, db.TUC_shkiaaDaka);
    this->alotHashahar = Zman(db.TUC_alotShaa, db.TUC_alotDaka);
    this->sofZmanShmaMga = Zman(db.TUC_sozksShaa, db.TUC_sozksDaka);
    this->minhaGdola = Zman(db.TUC_minhaGShaa, db.TUC_minhaGDaka);
    this->shabatOut = Zman(db.TUC_mozshShaa, db.TUC_mozshDaka);
    if (kaiz)
    {
        this->zrihaN = this->zrihaN+60;
        this->shkiaaA = this->shkiaaA+60;
        this->alotHashahar = this->alotHashahar+60;
        this->sofZmanShmaMga = this->sofZmanShmaMga+60;
        this->minhaGdola = this->minhaGdola+60;
        this->shabatOut = this->shabatOut+60;
    }
}

void DayInfo::setLoaziDate(DateTime* now)
{
    this->loaziDate.day = now->day();
    this->loaziDate.month= now->month();
    this->loaziDate.year =  now->year();
    this->dayInWeek = now->dayOfTheWeek();
    this->isShabat = false;
    this->isShishi = false;
    if(dayInWeek==5) { this->isShishi = true; }
    if(dayInWeek==6) { this->isShabat = true; }
}

uint8_t rtcDowOf(uint8_t day, uint8_t month, uint16_t year) {
    DateTime dt(year, month, day, 0, 0, 0);
    return dt.dayOfTheWeek();
}
// ===== ישראל =====
void israelTimeStateToday(const LoaziDate& date, LoaziDate& dstStart, LoaziDate& dstEnd)
{
    uint8_t dow31Mar = rtcDowOf(31, 3, date.year);
    uint8_t dow31Oct = rtcDowOf( 31, 10, date.year);
    LoaziDate lastSunMar( static_cast<uint8_t>(31 - ((dow31Mar + 7) % 7)), 3, date.year );   // יום ראשון האחרון במרץ
    dstStart = LoaziDate( static_cast<uint8_t>(lastSunMar.day - 2), 3, date.year );          // יום שישי שלפניו
    dstEnd = LoaziDate( static_cast<uint8_t>(31 - ((dow31Oct + 7) % 7)), 10, date.year );    // יום ראשון האחרון באוקטובר
}

// ===== ארה"ב =====
void usaTimeStateToday(const LoaziDate& date, LoaziDate& dstStart, LoaziDate& dstEnd)
{
    uint8_t dow1Mar  = rtcDowOf(1,  3, date.year);
    uint8_t dow1Nov  = rtcDowOf(1, 11, date.year);
    uint8_t firstSunMar = static_cast<uint8_t>(1 + ((7 - (dow1Mar % 7)) % 7));               // יום ראשון הראשון במרץ 
    dstStart = LoaziDate( static_cast<uint8_t>(firstSunMar + 7), 3, date.year );              // יום ראשון השני במרץ
    dstEnd = LoaziDate( static_cast<uint8_t>(1 + ((7 - (dow1Nov % 7)) % 7)), 11, date.year ); // יום ראשון הראשון בנובמבר
}

void DayInfo::setShaonKaitzHoref(LoaziDate& date, Place* place)
{
    LoaziDate dstStart;
    LoaziDate dstEnd;

    if(userPreferences.nz_hul==0) israelTimeStateToday(date, dstStart, dstEnd);
    if(userPreferences.nz_hul==1) usaTimeStateToday(date, dstStart,  dstEnd);

    if (date == dstStart) shaon=1;  // יום תחילת שעון קיץ
    if (date == dstEnd)   shaon=2;  // יום תחילת שעון חורף
    if (date >=  dstStart && date <  dstEnd) isShaonKaiz = true;
    else isShaonKaiz = false;


    if(userPreferences.shaonKaiz ==1)
    {
        if(isShaonKaiz) { place->kaiz = true; }
        if(!isShaonKaiz) { place->kaiz = false; }
    }
    if(userPreferences.shaonKaiz==2){ place->kaiz = true; }
    if(userPreferences.shaonKaiz==3){ place->kaiz = false; }
}

void DayInfo::initSun(Place* place)
{
    // Serial.println("Sun: start init.");
    // Serial.print("place->timezone: ");
    // Serial.println(place->timezone);
    sun.place = place;
    // Serial.println("Sun: sun->place init.");
    sun.date = loaziDate;
    // Serial.println("Sun: sun->date init.");
    sun.init();
    this->alot90 = sun.calcAlot90();
    this->zeet90 = sun.calcZeet90();
    this->alot72 = sun.calcAlot72();
    this->zeet72 = sun.calcZeet72();
    this->zrihaM = sun.calcZrihaMishorit();
    this->shkiaaM = sun.calcShkiaaMishorit();
    this->zrihaN = this->zrihaA = sun.calcZrihaAstronomit();
    this->shkiaaN = this->shkiaaA = sun.calcShkiaaAstronomit();

    // Serial.println("______________________________");
    // Serial.println("initSun");

    // Zman zman;
    // char buf[50];
    // loaziDate.toString(buf,1);
    // Serial.println(buf);

    // zman = sun.calcZrihaAstronomit();
    // zman.toStringSec(buf);
    // Serial.print("calcZrihaAstronomit: ");
    // Serial.println(buf);

    // zman = sun.calcZrihaMishorit();
    // zman.toStringSec(buf);
    // Serial.print("calcZrihaMishorit: ");
    // Serial.println(buf);
    
    // zman = sun.calcZrihaAstronomit();
    // zman.toStringSec(buf);
    // Serial.print("calcZrihaAstronomit: ");
    // Serial.println(buf);

    // zman = sun.calcShkiaaAstronomit();
    // zman.toStringSec(buf);
    // Serial.print("calcShkiaaAstronomit: ");
    // Serial.println(buf);

    // Serial.println("______________________________");

}

void DayInfo::setShita(Shita* shita)
{
    shita->calc(&sun, zrihaA, shkiaaA, zrihaM, shkiaaM, zrihaN, shkiaaN);
    this->alot90 = shita->alot90;
    this->zeet90 = shita->zeet90;
    this->alot72 = shita->alot72;
    this->zeet72 = shita->zeet72;
    this->alotHashahar = shita->alotHashahar;
    this->zmanTalit = shita->zmanTalit;
    this->sofZmanShmaMga = shita->sofZmanShmaMga;
    this->sofZmanShmaGra = shita->sofZmanShmaGra;
    this->sofZmanTfilaMga = shita->sofZmanTfilaMga;
    this->sofZmanTfilaGra = shita->sofZmanTfilaGra;
    this->hatsot = shita->hatsot;
    this->minhaGdola = shita->minhaGdola;
    this->minhaKtana = shita->minhaKtana;
    this->plagHaminha = shita->plagHaminha;
    this->tzetHacochavim = shita->tzetHacochavim;
    this->shabatIn = shita->shabatIn;
    this->shabatOut = shita->shabatOut;
    this->rabenuTam = shita->rabenuTam;
    this->tzomIn = shita->tzomIn;
    this->tzomOut = shita->tzomOut;
    this->tBeavIn = shita->tBeavIn;
    this->tBeavOut = shita->tBeavOut;
    this->kipurIn = shita->shabatIn;
    this->kipurOut = shita->shabatOut;
    this->sofZmanBeiurHamez = shita->sofZmanBeiurHamez;
}

void DayInfo::printDayInfo()
{
    Serial.println("==============================");
	char buffer[20];
    loaziDate.toString(buffer, 1);
    Serial.println(buffer);

    hebrewDate.toString(buffer, 1);
    Serial.println(buffer);
    Serial.println("");

    Zman zrihaSun = sun.calcZrihaMishorit();
    Serial.print("zrihaSun mishorit sun: ");
    zrihaSun.toString(buffer);
    Serial.println(buffer);

    Serial.print("zriha mishorit: ");
    zrihaM.toString(buffer);
    Serial.println(buffer);

    Serial.print("zriha haniret: ");
    zrihaN.toString(buffer);
    Serial.println(buffer);

    Serial.print("zriha astronomit: ");
    zrihaA.toString(buffer);
    Serial.println(buffer);

    Zman shkiaaSun = sun.calcShkiaaMishorit();
    Serial.print("shkiaaSun mishorit sun: ");
    shkiaaSun.toString(buffer);
    Serial.println(buffer);

    Serial.print("shkiaa mishorit: ");
    shkiaaM.toString(buffer);
    Serial.println(buffer);

    Serial.print("shkiaa haniret: ");
    shkiaaN.toString(buffer);
    Serial.println(buffer);

    Serial.print("shkiaa astronomit: ");
    shkiaaA.toString(buffer);
    Serial.println(buffer);

    Serial.print("alotHashahar: ");
    alotHashahar.toString(buffer);
    Serial.println(buffer);

    Serial.print("zmanTalit: ");
    zmanTalit.toString(buffer);
    Serial.println(buffer);

    Serial.print("sofZmanShmaMga: ");
    sofZmanShmaMga.toString(buffer);
    Serial.println(buffer);

    Serial.print("sofZmanShmaGra: ");
    sofZmanShmaGra.toString(buffer);
    Serial.println(buffer);

    Serial.print("sofZmanTfilaMga: ");
    sofZmanTfilaMga.toString(buffer);
    Serial.println(buffer);

    Serial.print("sofZmanTfilaGra: ");
    sofZmanTfilaGra.toString(buffer);
    Serial.println(buffer);

    Serial.print("hatsot: ");
    hatsot.toString(buffer);
    Serial.println(buffer);

    Serial.print("minhaGdola: ");
    minhaGdola.toString(buffer);
    Serial.println(buffer);

    Serial.print("minhaKtana: ");
    minhaKtana.toString(buffer);
    Serial.println(buffer);

    Serial.print("plagHaminha: ");
    plagHaminha.toString(buffer);
    Serial.println(buffer);

    Serial.print("tzetHacochavim: ");
    tzetHacochavim.toString(buffer);
    Serial.println(buffer);

    Serial.print("shabatIn: ");
    shabatIn.toString(buffer);
    Serial.println(buffer);

    Serial.print("shabatOut: ");
    shabatOut.toString(buffer);
    Serial.println(buffer);

    Serial.print("rabenuTam: ");
    rabenuTam.toString(buffer);
    Serial.println(buffer);

    Serial.print("tzomIn: ");
    tzomIn.toString(buffer);
    Serial.println(buffer);

    Serial.print("tzomOut: ");
    tzomOut.toString(buffer);
    Serial.println(buffer);

    Serial.print("tBeavIn: ");
    tBeavIn.toString(buffer);
    Serial.println(buffer);

    Serial.print("tBeavOut: ");
    tBeavOut.toString(buffer);
    Serial.println(buffer);

    Serial.print("sofZmanBeiurHamez: ");
    sofZmanBeiurHamez.toString(buffer);
    Serial.println(buffer);
	
    Serial.println("==============================");
}
uint8_t haftaraByEda(uint8_t edotMizrah, uint8_t ashkenaz, uint8_t teiman_baladi, uint8_t teiman_shami)
{
    if(userPreferences.eda == EDOT_MIZRAH) return edotMizrah;
    if(userPreferences.eda == ASHKENAZ || userPreferences.eda == CHABAD || userPreferences.eda == SFARAD) return ashkenaz;
    if(userPreferences.eda == TEIMAN_BALADI) return teiman_baladi;
    if(userPreferences.eda == TEIMAN_SHAMI) return teiman_shami;
    return edotMizrah;
}
void DayInfo::setHaftara()
{
    // Serial.print("parasha: ");
    // Serial.println(parasha);
    switch (parasha)
    {
		case 12:  // בראשית
			haftara = haftaraByEda(0, 0, 86, 86);
			break;
		case 13:  // נח
			haftara = 1;
			break;
		case 14:  // לך לך
			haftara = haftaraByEda(2, 2, 87, 87);
			break;
		case 15:  // וירא
			haftara = 3;
			break;
		case 16:  // חיי שרה
			haftara = 4;
			break;
		case 17:  // תולדות
			haftara = 5;
			break;
		case 18:  // ויצא
			haftara = haftaraByEda(6, 75, 6, 6);
			break;
		case 19:  // וישלח
			haftara = 7;
			break;
		case 20:  // וישב
			haftara = 8;
			break;
		case 21:  // מקץ
			haftara = 9;
			break;
		case 22:  // ויגש
			haftara = 10;
			break;
		case 23:  // ויחי
			haftara = 11;
			break;
		case 24:  // שמות
			haftara = haftaraByEda(12, 76, 88, 88);
			break;
		case 25:  // וארא
			haftara = haftaraByEda(13, 13, 89, 89);
			break;
		case 26:  // בא
			haftara = haftaraByEda(14, 14, 90, 90);
			break;
		case 27:  // בשלח
			haftara = haftaraByEda(15, 77, 91, 91);
			break;
		case 28:  // יתרו
			haftara = 16;
			break;
		case 29:  // משפטים
			haftara = 17;
			break;
		case 30:  // תרומה
			haftara = 18;
			break;
		case 31:  // תצוה
			haftara = 19;
			break;
		case 32:  // כי תשא
			haftara = haftaraByEda(20, 78, 78, 78);
			break;
		case 67:  // ויקהל
			haftara = haftaraByEda(21, 22, 21, 21);
			break;
		case 68:  // פקודי
			haftara = haftaraByEda(22, 79, 22, 22);
			break;
		case 33:  // ויקהל פקודי
			haftara = haftaraByEda(22, 79, 22, 22);
			break;
		case 34:  // ויקרא
			haftara = 23;
			break;
		case 35:  // צו
			haftara = 24;
			break;
		case 41:  // שמיני
			haftara = 25;
			break;
		case 69:  // תזריע
			haftara = 26;
			break;
		case 70:  // מצורע
			haftara = haftaraByEda(27, 80, 80, 80);
			break;
		case 42:  // תזריע מצורע
			haftara = haftaraByEda(27, 80, 80, 80);
			break;
		case 71:  // אחרי מות
			haftara = haftaraByEda(28, 81, 28, 28);
			break;
		case 72:  // קדושים
			haftara = haftaraByEda(29, 28, 92, 92);
			break;
		case 43:  // אחרי מות קדושים
			haftara = haftaraByEda(29, 28, 92, 92);
			break;
		case 44:  // אמור
			haftara = 30;
			break;
		case 73:  // בהר
			haftara = haftaraByEda(31, 31, 32, 32);
			break;
		case 74:  // בחוקותי
			haftara = haftaraByEda(32, 32, 93, 93);
			break;
		case 45:  // בהר בחוקותי
			haftara = haftaraByEda(32, 32, 93, 93);
			break;
		case 46:  // במדבר
			haftara = 33;
			break;
		case 49:  // נשא
			haftara = 34;
			break;
		case 50:  // בהעלותך
			haftara = 35;
			break;
		case 51:  // שלח
			haftara = 36;
			break;
		case 52:  // קרח
			haftara = 37;
			break;
		case 75:  // חקת
			haftara = 38;
			break;
		case 76:  // בלק
			haftara = 39;
			break;
		case 53:  // חקת בלק
			haftara = 39;
			break;
		case 54:  // פנחס
            if(nextShabat->hebrewDate.yom>=17) { haftara=12; }
            else { haftara = 40; }
			break;
		case 55:  // מטות
			haftara = 12;
			break;
		case 56:  // מסעי
			haftara = haftaraByEda(41, 41, 42, 41);
			break;
		case 57:  // מטות מסעי
			haftara = haftaraByEda(41, 41, 42, 41);
			break;
		case 58:  // דברים
			haftara = haftaraByEda(42, 42, 94, 42);
			break;
		case 59:  // ואתחנן
			haftara = 43;
			break;
		case 61:  // עקב
			haftara = 44;
			break;
		case 62:  // ראה
			haftara = 45;
			break;
		case 63:  // שופטים
			haftara = 46;
			break;
		case 64:  // כי תצא
			haftara = 1;
			break;
		case 65:  // כי תבוא
			haftara = 47;
			break;
		case 77:  // ניצבים
			haftara = haftaraByEda(48, 48, 95, 95);
			break;
		case 3:  // וילך
			haftara = 49;
			break;
		case 66:  // ניצבים וילך
			haftara = haftaraByEda(48, 48, 95, 95);
			break;
		case 4:  // האזינו
			haftara = haftaraByEda(50, 50, 96, 96);
			break;
        default:
            break;
    }
    if(nextShabat->isRoshHodesh && (nextShabat->hebrewDate.hodesh)!=13 && (nextShabat->hebrewDate.hodesh)!=14) { haftara = 51; }  // שבת וראש חודש
    if(nextShabat->hebrewDate.yom==29 && (nextShabat->hebrewDate.hodesh)!=13) { haftara = 52; }  // מחר חודש
    if(shabatPlus==7) { haftara = haftaraByEda(53, 82, 82, 82); }  // שקלים
    if(shabatPlus==8) { haftara = haftaraByEda(55, 55, 55, 55); }  // פרה
    if(shabatPlus==9) { haftara = haftaraByEda(56, 84, 98, 98); }  // החודש
    if(shabatPlus==11) { haftara = haftaraByEda(54, 83, 97, 97); }  // זכור
    if(shabatPlus==15) { haftara = haftaraByEda(49, 49, 49, 49); }  // תשובה
    if(shabatPlus==12 && userPreferences.eda!=TEIMAN_BALADI) { haftara = 57; }  // הגדול
    if(nextShabat->parasha==6 && hebrewDate.hodesh==1) { haftara = haftaraByEda(71, 71, 103, 103);}  // שבת חוה"מ סוכות
    if(nextShabat->parasha==6 && hebrewDate.hodesh==9) { haftara = haftaraByEda(60, 60, 100, 100);}  // שבת חוה"מ פסח
    if(nextShabat->isHanuka && !(nextShabat->holiday==21)) { haftara = 35; }  // שבת חנוכה ראשונה
    if(nextShabat->isHanuka && nextShabat->holiday==21) { haftara = 22; }  // שבת חנוכה שניה
    if(nextShabat->holiday==26 && userPreferences.homa == 1) { haftara = haftaraByEda(54, 83, 97, 97); }  // שושן פורים
    if(userPreferences.showIsraeli==1 && isAzmaut) { haftara=61; }  // עצמאות

    switch (parasha)
    {
		case 36:  // פסח
			haftara = 58;
			break;
		case 37:  // פסח חו"ל
			haftara = haftaraByEda(59, 59, 99, 99);
			break;
		case 39:  // שביעי של פסח
			haftara = 50;
			break;
		case 40:  // שביעי של פסח חו"ל
			haftara = 61;
			break;
		case 48:  // שבועות
			haftara = 62;
			break;
		case 47:  // שבועות חו"ל
			haftara = haftaraByEda(63, 85, 63, 63);
			break;
		case 60:  // תשעה באב
			haftara = haftaraByEda(64, 64, 101, 101);
			break;
		case 1:  // א ראש השנה
			haftara = 65;
			break;
		case 2:  // ב ראש השנה
			haftara = 66;
			break;
		case 5:  // כיפור
			haftara = 67;
			break;
		case 7:  // סוכות
			haftara = haftaraByEda(69, 69, 102, 102);
			break;
		case 8:  // סוכות חו"ל
			haftara = haftaraByEda(70, 70, 79, 79);
			break;
		case 10:  // שמיני עצרת חו"ל
			haftara = 72;
			break;
		case 11:  // שמחת תורה
			haftara = 73;
			break;
        default:
            break;
    }
    // Serial.print("haftara: ");
    // Serial.println(haftara);
}
void DayInfo::setHftaraMinha()
{
    if(isZom && (userPreferences.eda == ASHKENAZ || userPreferences.eda == CHABAD || userPreferences.eda == SFARAD)) { haftara=74; }
    if(isYomKipur) { haftara=68; }
    if(isTishaBeav) { haftara = haftaraByEda(49, 74, 49, 49); }
}
void DayInfo::setBooleanValues(uint8_t _shabatPlus, uint8_t _holidayType, uint8_t _roshHodesh, 
uint8_t _tfilaPlus1, uint8_t _tfilaPlus2, uint8_t _shaonKaiz)
{
    this->isRoshHodesh = _roshHodesh == 1;
    this->isShabatMevarchin = _shabatPlus >=20;
    this->isShabatPlus = shabatPlus != 0;
    this->isErevHag = _holidayType == 1;
    this->isHag = _holidayType == 2;
    this->isErevZom = _holidayType == 3;
    this->isZom = _holidayType == 4;
    this->isHagIsraeli = _holidayType == 5;
    this->isHanuka = _holidayType == 6;
    this->isPurim = _holidayType == 7;
    this->isShushanPurim = holiday == 26;
    this->isMoed = _holidayType == 8 || isPurim;
    this->mashivHaruah = (_tfilaPlus1&128)>0;
    this->moridHatal = (_tfilaPlus1&64)>0;
    this->barechAlenu = (_tfilaPlus1&32)>0;
    this->barchenu = (_tfilaPlus1&16)>0;
    this->enTahanun = (_tfilaPlus1&8)>0;
    this->enZidkatcha = (_tfilaPlus1&4)>0;
    this->yaaleVeyavo = (_tfilaPlus1&2)>0;
    this->alHanisim = (_tfilaPlus1&1)>0;
    this->anenu = (_tfilaPlus2&128)>0;
    this->eruvTavshilin = (_tfilaPlus2&64)>0;
    this->halelShalem = (_tfilaPlus2&32)>0;
    this->halelDilug = (_tfilaPlus2&16)>0;
    this->ataYazarta = (_tfilaPlus2&8)>0;
    this->vatodienu = (_tfilaPlus2&4)>0;
    this->hamelechHakadosh = (_tfilaPlus2&2)>0;
    this->hamelechHamishpat = (_tfilaPlus2&1)>0;
    this->isOmer = this->omer != 0;
    this->afterShkiaa = false;
    this->zmanMinha = false;
    this->isMozaeiShabat = false;
    this->isMozaeiHag = false;
    this->isMozaeiZom = false;
    this->isMozaeiKipur = false;
    this->isMozaeiTishaBeav = false;
    this->isAzmaut = holiday==31;
    this->isYomYerushalaim = holiday==34;
    this->isYomHashoa = holiday==44;
    this->isYomHazicaron = holiday==30;
    this->isErevTishaBeav = holiday==38;
    this->isTishaBeav = holiday==39;
    this->isPesach = holiday==28;
    this->isSimchatTora = holiday==13;
    this->isShminiAzeret = holiday==47;
    this->isErevYomKipur = holiday==3;
    this->isYomKipur = holiday==4;
    this->isErevRoshHashana = holiday==40;
    this->isPesachSheni = holiday==45;
    this->isShaonKaiz = _shaonKaiz;
    this->isYomChabad = this->yom_chabad != 0;
}
void DayInfo::setHul()
{
    this->holiday = this->holiday_hul;
    this->parasha = this->parasha_hul;
    setBooleanValues(this->shabatPlus, this->holidayType_hul, this->roshHodesh, this->tfilaPlus1, this->tfilaPlus2, this->shaonKaiz);
    tomorrow->holiday = tomorrow->holiday_hul;
    tomorrow->parasha = tomorrow->parasha_hul;
    tomorrow->setBooleanValues(this->shabatPlus, this->holidayType_hul, this->roshHodesh, this->tfilaPlus1, this->tfilaPlus2, this->shaonKaiz);
    uint8_t leap = 0;
    if (loaziDate.isLeap()) { leap = 1; }
    if(loaziDate.month == 9 || loaziDate.month == 10 || loaziDate.month == 11 || (loaziDate.month == 12 && loaziDate.day<5+leap))
    {
        barchenu=true;
        barechAlenu=false;
    }
}
void DayInfo::setTahanun()
{
    if(userPreferences.showIsraeli==0 && (isAzmaut || isYomYerushalaim))
    {
        enTahanun = false;
    }
    if(!enTahanun && tomorrow->enTahanun && !isErevRoshHashana && !isErevYomKipur && !isPesachSheni && !isShabat && !isZom && !isHag)
    {
        enTahanun = true;
    }
    if(tomorrow->isAzmaut || tomorrow->isYomYerushalaim)
    {
        enTahanun = false;
    }
}
void DayInfo::setBaladi()
{
    if(isShabat && (halelDilug||halelShalem))
    {
        enZidkatcha = true;
    }
    else
    {
        enZidkatcha = false;
    }
    parasha = parasha_baladi;
    tomorrow->parasha = tomorrow->parasha_baladi;
}
void DayInfo::setHazot()
{
    uint8_t shaot = this->hatsot.getShaot();
    shaot += 12;
    shaot %= 24;
    this->hazotLayla = Zman(shaot, this->hatsot.getDakot());   
}
void DayInfo::setZom()
{
    // if(isErevZom) { tzomOut = tomorrow->tzomOut; }
    // if(isZom) { tzomIn = yesterday->tzomIn; }
    if(isErevTishaBeav) 
    { 
        tzomIn = tBeavIn; 
        tzomOut = tomorrow->tBeavOut; 
    }
    if(isTishaBeav) 
    { 
        tzomIn = yesterday->tBeavIn; 
        tzomOut = tBeavOut; 
    }
    if(isErevYomKipur) 
    { 
        tzomIn = kipurIn; 
        tzomOut = tomorrow->kipurOut; 
    }
    if(isYomKipur || isMozaeiKipur) 
    { 
        tzomIn = yesterday->kipurIn; 
        tzomOut = kipurOut; 
    }
}
void DayInfo::setShanimLahurban()
{
    this->shanimLahurban = loaziDate.year-69 + userPreferences.shanim;
}
void DayInfo::getParasha(char* buffer)
{ 
    sprintf(buffer, "%s", parasha_list[parasha]);
}
void DayInfo::getHaftara(char* shem, char* makor)
{ 
    sprintf(shem, "%s :%s", hafrara_shem_list[haftara], strings.haftara);
    sprintf(makor, "%s", hafrara_makor_list[haftara]);
}
void DayInfo::getShabatPlus(char* _shabatPlus)
{ 
    sprintf(_shabatPlus, "%s", shabat_plus_list[shabatPlus]);
}
void DayInfo::getOna(char* _ona)
{ 
    sprintf(_ona, "%s", ona_list[ona]);
}
void DayInfo::getDayPart(char* _part, uint8_t hour)
{ 
    uint8_t idx = 0;

    if (hour >= 6 && hour < 12)
        idx = 1; // בוקר
    else if (hour >= 12 && hour < 16)
        idx = 2; // צהריים
    else if (hour >= 16 && hour < 18)
        idx = 3; // אחר הצהריים
    else if (hour >= 18 && hour < 22)
        idx = 4; // ערב
    else // 22-6
        idx = 5; // לילה

    sprintf(_part, "%s", day_part_list[idx]);
}
void DayInfo::getHoliday(char* _holiday)
{ 
    sprintf(_holiday, "%s", holidays[holiday]);
}
void DayInfo::getRoshHodesh(char* _hodesh)
{ 
    sprintf(_hodesh, "%s %s", hodesh_list_l[hebrewDateTomorrow.hodesh] ,strings.roshHodesh);
}
void DayInfo::getNextRoshHodesh(char* _hodesh_up, char* _hodesh_down)
{
    uint8_t firstDay = next_rosh_hodesh / 10;
    uint8_t secondDay = next_rosh_hodesh % 10;
    sprintf(_hodesh_up, "%s asuj -  %s", hodesh_list_l[next_hodesh], strings.shabatMevarchin);
    if(next_rosh_hodesh >10) // rosh hodesh is 2 days
    {
        sprintf(_hodesh_down, "%su %s %s", daysOfTheWeek[secondDay], daysOfTheWeek[firstDay], "ohnhc kujh j\"r");
    }
    else{
        sprintf(_hodesh_down, "%s %s", daysOfTheWeek[secondDay], "ouhc kujh j\"r");
    }
}
void DayInfo::getYomHabad(char* _chabad, uint8_t idx)
{ 
    sprintf(_chabad, "%s", chabad_list[idx]);
}
void DayInfo::getMolad(char* _moladUp, char* _moladDown)
{ 
    sprintf(_moladUp, "%s %s %s", "ouhc vhvh", hodesh_list_l[next_hodesh], "asuj skun");
    sprintf(_moladDown, "%s %d u %02d:%02d %s %s", "ohekj", molad_halakim, molad_daka, molad_shaa, "vgac", daysOfTheWeek[molad_yom]);
}
void DayInfo::getHanuka(char* _ner)
{ 
    sprintf(_ner, "%s %s %s", "vfubj ka ", holidays[holiday], " rb");
}
void DayInfo::getShanimLamdina(char* _txt)
{ 
    sprintf(_txt, "%s %d %s", "ktrah wbhsnk", loaziDate.year-1948, "v ");
}
void DayInfo::getShanimLyerushalaim(char* _txt)
{ 
    sprintf(_txt, "%s %d", "ohkaurh rurjak ohba ", loaziDate.year-1967);
}
void DayInfo::getShanimLahurban(char* _txt)
{ 
    sprintf(_txt, "%s %d", "hba whc icrujk ohba ", shanimLahurban);
}
void DayInfo::getYomOrLeil(char* _yomTitle)
{
    if(userPreferences.leil==1 && afterShkiaa)
    {
        sprintf(_yomTitle, "%s", strings.leil);
    }
    else
    {
        sprintf(_yomTitle, "%s", strings.yom);
    }
}
void DayInfo::getDayInWeek(char* _dayInWeek, bool yom)
{
    uint8_t dayIndex = dayInWeek+1;
    if(userPreferences.leil==1 && afterShkiaa)
    {
        dayIndex %= 7;
        dayIndex += 1;
    }
    if(yom)
    {
        char yomTitle[4];
        getYomOrLeil(yomTitle);
        sprintf(_dayInWeek, "%s %s", daysOfTheWeek[dayIndex], yomTitle);
    }
    else{
        sprintf(_dayInWeek, "%s", daysOfTheWeek[dayIndex]);
    }
}
bool DayInfo::isNewDay(DateTime* _now)
{
   return _now->day() != loaziDate.day || _now->month() != loaziDate.month || _now->year() != loaziDate.year;
}
bool DayInfo::isAfterShkiaa(DateTime* _now)
{
    return 60*_now->hour()+_now->minute() > 60*shkiaaM.shaot+shkiaaM.dakot && !afterShkiaa;
}
bool DayInfo::isZmanMinha(DateTime* _now)
{
    return 60*_now->hour()+_now->minute() > 60*minhaGdola.shaot+minhaGdola.dakot && !zmanMinha;
}
void DayInfo::setAfterShkiaa()
{
    afterShkiaa = true;
    hebrewDate.yom = hebrewDateTomorrow.yom;
    hebrewDate.hodesh = hebrewDateTomorrow.hodesh;
    hebrewDate.shana = hebrewDateTomorrow.shana;
    isMozaeiShabat = isShabat;
    isMozaeiHag = isHag;
    isMozaeiZom = isZom;
    isMozaeiTishaBeav = isTishaBeav;
    isMozaeiKipur = isYomKipur;
    isShishi = false;
    isErevHag = false;
    isErevYomKipur = false;
    isErevTishaBeav = false;
    eruvTavshilin = false;
    enTahanun = false;
    enZidkatcha = false;
    halelDilug = false;
    halelShalem = false;
    anenu = false;
    if(isPesach) { halelShalem=true; }

    holiday = tomorrow->holiday;
    omer = tomorrow->omer;
    isAzmaut = tomorrow->isAzmaut;
    isRoshHodesh= tomorrow->isRoshHodesh;
    isHag = tomorrow->isHag;
    isZom = tomorrow->isZom;
    tzomIn = tomorrow->tzomIn;
    tzomOut = tomorrow->tzomOut;
    isShabat = tomorrow->isShabat;
    isHagIsraeli = tomorrow->isHagIsraeli;
    isYomHashoa= tomorrow->isYomHashoa;
    isYomHazicaron = tomorrow->isYomHazicaron;
    isTishaBeav = tomorrow->isTishaBeav;
    isHanuka = tomorrow->isHanuka;
    isPurim = tomorrow->isPurim;
    isShushanPurim = tomorrow->isShushanPurim;
    isMoed = tomorrow->isMoed;
    isOmer = tomorrow->isOmer;
    isYomChabad = tomorrow->isYomChabad;
    yom_chabad = tomorrow->yom_chabad;
    yaaleVeyavo = tomorrow->yaaleVeyavo;
    alHanisim = tomorrow->alHanisim;
    barchenu = tomorrow->barchenu;
    barechAlenu = tomorrow->barechAlenu;
    mashivHaruah = tomorrow->mashivHaruah;
    moridHatal = tomorrow->moridHatal;
    hamelechHakadosh = tomorrow->hamelechHakadosh;
    hamelechHamishpat = tomorrow->hamelechHamishpat;
    isShabatMevarchin = tomorrow->isShabatMevarchin;
    isShabatPlus = tomorrow->shabatPlus;
    isYomKipur = tomorrow->isYomKipur;
    next_rosh_hodesh = tomorrow->next_rosh_hodesh;
    next_hodesh = tomorrow->next_hodesh;
    if(tomorrow->isPesach) { halelShalem=true; }
    parasha = tomorrow->parasha;
    parasha_baladi = tomorrow->parasha_baladi;
    parasha_hul = tomorrow->parasha_hul;
    setHaftara();
    setZom();
}
void DayInfo::setZmanMinha()
{
    zmanMinha = true;
    if(tomorrow->holiday==33) // lag baomer
    {
        isMoed=true;
        holiday = 32; // erev lag baomer
    }
    setTahanun();
    setHftaraMinha();
}
void DayInfo::setNewDay()
{
    if(userPreferences.hul) { setHul(); }
    setZom();
    if(userPreferences.eda == TEIMAN_BALADI) { setBaladi(); }
    setHaftara();
    setShanimLahurban();
    setHazot();
}
bool DayInfo::isYomHol()
{
   return !(isShabat||isShishi||isMozaeiShabat||isErevHag||isHag||isMozaeiHag||isErevZom||isZom||isMozaeiZom);
}
const char * DayInfo::getBircatHashanim()
{
    // עדות המזרח, שאמי
    if(userPreferences.eda==1 || userPreferences.eda==5 || userPreferences.eda==0)
    {        
        if(barchenu) { return strings.barechenu; }
        if(barechAlenu) { return strings.barechAlenu; }
    }
    // אשכנז, ספרד, חב"ד
    if(userPreferences.eda==2 || userPreferences.eda==6 || userPreferences.eda==3)
    {        
        if(barchenu) { return strings.vetenBracha; }
        if(barechAlenu) { return strings.vetenTalUmatar; }
    }
    // בלדי
    if(userPreferences.eda==4)
    {        
        if(barchenu) { return strings.betaleleRatzon; }
        if(barechAlenu) { return strings.vetenTalUmatar; }
    }
    return "";
}
const char * DayInfo::getGeshemTal()
{
    if(mashivHaruah) { return strings.mashivHaruah; }
    if(moridHatal) { return strings.moridHatal; }
    return "";
}
Zman DayInfo::getZriha()
{
    if (userPreferences.zriha==1) return zrihaM;
    if (userPreferences.zriha==2) return zrihaN;
    if (userPreferences.zriha==3) return zrihaA;
    if (userPreferences.zriha==4 && userPreferences.ofek > 4) return zrihaN + 2.5;
    return zrihaM;
}
Zman DayInfo::getShkiaa()
{
    if (userPreferences.shkiaa==1) return shkiaaM;
    if (userPreferences.shkiaa==2) return shkiaaN;
    if (userPreferences.shkiaa==3) return shkiaaA;
    return shkiaaM;
}
Zman DayInfo::getZeetCochavim()
{
    return tzetHacochavim;
}
Zman DayInfo::getHagIn()
{
    if(isHag) return yesterday->shabatIn;
    return shabatIn;
}
Zman DayInfo::getHagOut()
{
    if(isErevHag) return tomorrow->shabatOut;
    return shabatOut;
}
