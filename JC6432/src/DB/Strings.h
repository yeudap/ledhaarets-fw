#ifndef STRINGS_H
#define STRINGS_H 

class Strings
{
public:
    bool rtl; // true=RTL, false=LTR

    const char* empty = "";

    // Main
    const char* yom;
    const char* leil;
    const char* parashatShavua;
    const char* zriha;
    const char* shkiaa;
    const char* tzetCoc;
    const char* talit;
    const char* aHashahar;
    const char* gra;
    const char* adhaz;
    const char* mga;
    const char* sofZman;
    const char* soz;
    const char* kriatSma;
    const char* ks;
    const char* sozksMga;
    const char* soztMga;
    const char* sozks;
    const char* sozt;
    const char* tfila;
    const char* hazot;
    const char* minhaG;
    const char* minhaGdola;
    const char* minhaKtana;
    const char* plagMinha;
    const char* plagHaminha;
    const char* hazotLayla;
    const char* shabatIn;
    const char* shabatOut;
    const char* rabenuTamRT;
    const char* hagIn;
    const char* HagOut;
    const char* ZomIn;
    const char* ZomOut;
    const char* cnisatHag;
    const char* zeetHag;
    const char* cnisatZom;
    const char* zeetZom;
    const char* barechAlenu;
    const char* barechenu;
    const char* vetenBracha;
    const char* vetenTalUmatar;
    const char* betaleleRatzon;
    const char* mashivHaruah;
    const char* mashivHaruahMimusaf;
    const char* moridHatal;
    const char* moridHatalMimusaf;
    const char* anenu;
    const char* enOmrim;
    const char* enOmrimRT;
    const char* tahanun;
    const char* zidkatcha;
    const char* eruvTavshilin;
    const char* roshHodesh;
    const char* yaaleveyavo;
    const char* halelShalem;
    const char* halelBedilug;
    const char* alHanisim;
    const char* erev;
    const char* sfiratHomer;
    const char* purimSameah;
    const char* shabatMevarchin;
    const char* aseretYemeTshuva;
    const char* hamelechHakadosh;
    const char* hamelechHamishpat;
    const char* zomTishaBeav;
    const char* LashoaVelagvura;
    const char* LehalleMaarchotIsrael;
    const char* sofZmanAchilatHametz;
    const char* sofZmanBeiurHametz;
    
    // Menu
    const char* menuTitle;
    const char* menuClock;
    const char* menuTzuga;
    const char* menuReset;
    const char* menuAbout;
    const char* menuOffek;
    const char* menuTohen;
    const char* menuEddot;
    const char* menuMinag;

    // Clock
    const char* kivunShaon;
    const char* mivneShaa;
    const char* mivneTaarich;
    const char* mivneZman;
    const char* shaonKaizHoref;
    const char* shaonKaiz;
    const char* pail;
    const char* kavuy;
    const char* otomati;
    const char* lelo;

    // Tezuga
    const char* tzugaTitle;
    const char* gufan;
    const char* tzeva;
    const char* behirut;

    // Ofek
    const char* ofekTitle;
    const char* bharOfek;
    const char* hagderMakom;
    const char* kavOrech;
    const char* kavRohav;
    const char* ezorZman;
    const char* gova;
    const char* huzLaaretz;
    
    // Shita
    const char* shitaTitle;

    // Tohen
    const char* tohenTitle;
    const char* setTohenTitle;
    const char* hakdasha;
    const char* zmaneHayom;
    const char* meidaHayom;
    const char* kezevHahlafa;
    const char* shniot;
    const char* kavua;
    const char* mithalef;
    const char* hazmanHakarov;
    const char* colZmaneiHayom;
    const char* zrihaSkiaaZeetHacochavim;
    const char* leloZmaneiHayom;
    const char* yomBashavua;
    const char* yomLezadShaon;
    const char* haftara;
    const char* taarichIvri;
    const char* taarichLoazi;
    const char* dafHayomiBavli;
    const char* dafHayomiYerushalmi;
    const char* meidaLamitpalel;
    const char* zmaneiShabat;
    const char* hagimUmoadim;
    const char* hagimIsraelim;
    const char* talitAlotZriha;
    const char* sofZmanTfila;
    const char* sofZmanKriatShma;
    const char* hazotMinhaGMinhaK;
    const char* plagShkiaaCochavim;
    const char* hazotAlotZriha;
    const char* zrihaYomShkiaa;
    const char* zrihaParashaShkiaa;
    const char* cShabatParashaZShabat;
    const char* cShabatZShabatRT;
    const char* yemeHol;
    const char* shishiShabat;
    const char* colHayom;
    const char* bazmanHamatim;
    const char* bichlalLo;

    // Minhagim
    const char* minhagTitle;
    const char* edotHamizrah;
    const char* sfarad;
    const char* ashkenaz;
    const char* teimanBaladi;
    const char* teimanShami;
    const char* chabad;
    const char* bneiHul;

    // Zmanim
    const char* mishorit;
    const char* haniret;
    const char* astronomit;
    const char* rabenuTam;
    const char* zeetShabat;
    const char* cnisatShabat;
    const char* zeetHacochavim;
    const char* zeetZomDerabanan;
    const char* zmanTalitUtfilin;
    const char* alotHashahar;
    const char* hadlakatNerotMukdemet;
    const char* dakot;
    const char* zShabatMehashkiaa;
    const char* irMukefetHoma;
    const char* leilAhreiShkiaa;
    const char* zmanLanetzHaama;
    const char* sfiraLanetz;
    const char* henetzHahama;
    const char* hazmanShenotar;
    const char* shanimLahurban;
    const char* hazegColZmaneiHayom;

    // Config
    const char* menuConfg;
    const char* bt;
    const char* on;
    const char* off;
    const char* resetPref;
    const char* resetText;
    const char* resetPass;
    const char* aboutUs;
    const char* aboutDevice;
    const char* MemoryMap;

    // About
    const char* info1;
    const char* info2;
    const char* phone;
    const char* url;
    const char* odotHamozr;
    const char* model;
    const char* swVersion;
    const char* hwVersion;
    const char* serial;
    const char* lastUpdate;

    // Reset
    const char* resetTitle;
    const char* resetPrefRequest;
    const char* resetTextRequest;
    const char* resetPassRequest;
    const char* yes;
    const char* no;

    // Texts
	const char* hazenText;
    const char* TextsTitle;
    const char* hosef;
    const char* mchak;
    const char* aroch;

    const char* brightnessTitle;
    const char* colorsTitle;

    void setStringsHeb();
    void setStringsEn();
};

#endif