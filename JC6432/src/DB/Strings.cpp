#include "Strings.h"

void Strings::setStringsHeb()
{
    rtl=true;

    // Main
    yom = "ouh";                   // יום
    leil = "khk";                  // ליל
    parashatShavua = "gucav .p";   // פ. השבוע
    zriha = "vjhrz";               // זריחה
    shkiaa = "vghea";              // שקיעה
    tzetCoc = "f\"vtm";            // צאה"כ
    talit = "w\"uy .z";            // זמן טו"ת
    aHashahar = "rja.g";            // עלות
    gra = "t\"rdv";                // הגר"א
    adhaz = "z\"vst";              // אדה"ז
    mga = "t\"dn";                 // מג"א
    sofZman = "inz qux";           // סוף זמן
    soz = "z\"ux";                 // סו"ז
    kriatSma = "gna wthre";        // קריאת שמע
    ks = "gna .e";             // ק"ש
    sozks = "a\"ezux";             // סוזק"ש
    sozt = "w\"zux";               // סוז"ת
    sozksMga = "t\"dn a\"ezux";    // סוזק"ש מג"א
    soztMga = "t\"dn w\"zux";      // סוז"ת מג"א
    tfila = "vkhpw";               // תפילה
    hazot = "wumj";                // חצות
    minhaG = ".d vjbn";            // .מנחה ג
    minhaGdola = "vkusd vjbn";     // מנחה גדולה
    minhaKtana = ".e vjbn";        // .מנחה ק
    plagMinha = ".n dkp";          // .פלג מ
    plagHaminha = "vjbnv dkp";     // פלג המנחה
    hazotLayla = "wumj";        // .חצות ל
    shabatIn = "wca .f";            // כ. שבת
    shabatOut = "wca .m";           // צ. שבת
    rabenuTamRT = "w\"r";          // ר"ת
    hagIn = "djv .f";               // כ.החג
    HagOut = "djv .m";              // צ.החג
    ZomIn = "oumv .f";              // כ.הצום
    ZomOut = "oumv .m";             // צ.הצום
    rabenuTamRT = "w\"r";          // ר"ת
    cnisatHag = "djv wxhbf";               // כ.החג
    zeetHag = "djv wtm";              // צ.החג
    cnisatZom = "oumv wxhbf";              // כ.הצום
    zeetZom = "oumv wtm";             // צ.הצום
    barechAlenu = "ubhkg lrc";     // ברך עלינו
    barechenu = "ubfrc";           // ברכנו
    vetenBracha = "vfrc iwu";      // ותן ברכה
    vetenTalUmatar = "rynu ky iwu";// ותן טל ומטר
    betaleleRatzon = "iumr hkkyc"; // בטללי רצון
    mashivHaruah = "jurv chan";    // משיב הרוח
    mashivHaruahMimusaf = "qxunn - jurv chan";    // משיב הרוח - ממוסף
    moridHatal = "kyv shrun";      // מוריד הטל
    moridHatalMimusaf = "qxunn - kyv shrun";      // מוריד הטל - ממוסף
    anenu = "ubbg";                // עננו
    enOmrim = "ohrnut iht";              // א"א
    enOmrimRT = "t\"t";            // א"א
    tahanun = "iubjw";             // תחנון
    zidkatcha = "lwesm";           // צדקתך
    roshHodesh = "asuj atr";       // ראש חודש
    yaaleveyavo = "tuchu vkgh";    // יעלה ויבוא
    halelShalem = "oka kkv";       // הלל שלם
    halelBedilug = "dukhsc kkv";   // הלל בדילוג
    alHanisim = "ohxhbv kg";       // על הניסים
    erev = "crg";                  // ערב
    sfiratHomer = "rnugv wrhpx";   // ספירת העומר
    purimSameah = "jna ohrup";     // פורים שמח
    shabatMevarchin = "ohfrcn wca";      // שבת מברכים
    aseretYemeTshuva = "vcuaw hnh wrag"; // עשרת ימי תשובה
    hamelechHakadosh = "ausev lknv";     // המלך הקדוש
    hamelechHamishpat = "ypanv lknv";    // המלך המשפט
    eruvTavshilin = "ihkhacw curhg";     // עירוב תבשילין
    zomTishaBeav = "ctc vgaw oum";       // צום תשעה באב
    LashoaVelagvura = "vrucdku vtuak";   // לשואה ולגבורה
    LehalleMaarchotIsrael = "ktrah wufrgn hkkjk"; // לחללי מערכות ישראל
    sofZmanAchilatHametz = "_nj wkhft inz qux";     // סוף זמן אכילת חמץ
    sofZmanBeiurHametz = "_nj rughc inz qux";       // סוף זמן ביעור חמץ

    // Menu
    menuTitle = "yhrpw";      // תפריט
    menuClock = "lhrtwu vga"; // שעה ותאריך
    menuTzuga = "vdumw";      // תצוגה
    menuReset = "xupht";      // איפוס
    menuAbout = "wusut";      // אודות
    menuOffek = "ouen";       // מקום
    menuTohen = "ifuw";       // תוכן 
    menuEddot = "ohdvbn";     // מנהגים
    menuMinag = "ohbnz";      // זמנים 

    // Tezuga
    tzugaTitle = "vdumw wursdv"; // הגדרות תצוגה
    gufan = "ipud";              // גופן
    tzeva = "gcm";               // צבע
    behirut = "wurhvc";          // בהירות

    // Clock
    kivunShaon = "lhrtwu vga iuuhf";  // כיוון שעה ותאריך
    mivneTaarich = "lhrtw vbcn";      // מבנה תאריך
    mivneShaa = "vga vbcn";           // מבנה שעה
    mivneZman = "inz vbcn";           // מבנה זמן
    shaonKaizHoref = "hynuyut qruj/_he iuga"; // שעון קיץ/חורף אוטומטי
    shaonKaiz = "_he iuga";           // שעון קיץ
    pail = "khgp";                    // פעיל
    kavuy = "hucf";                   // כבוי
    otomati = "hynuyut";              // אוטומטי
    lelo = "tkk";                     // ללא

    // Ofek
    ofekTitle = "eput";          // אופק
    bharOfek = "eput rjc";       // בחר אופק
    hagderMakom = "ouen rsdv";   // הגדר מקום
    kavOrech = "lrut ue";        // קו אורך
    kavRohav = "cjur ue";        // קו רוחב      
    ezorZman = "inz ruzt";       // אזור זמן
    gova = "vcud";               // גובה
    huzLaaretz = "_rtk _uj";     // חוץ לארץ

    // shita
    shitaTitle = "vyha rjc";      // בחר שיטה

    // Tohen
    tohenTitle = "ifuw";              // תוכן
    setTohenTitle = "ifuw rjc";       // בחר תוכן
    hakdasha = "vasev";               // הקדשה
    zmaneHayom = "ouhv hbnz";         // זמני היום
    meidaHayom = "ouhv gshn";         // מידע היום
    kezevHahlafa = "vpkjv cme";       // קצב החלפה
    shniot = "wuhba";                 // שניות
    hagimIsraelim = "ohktrah ohsgun"; // מועדים ישראלים
    kavua = "guce";                   // קבוע
    mithalef = "qkjwn";               // מתחלף
    hazmanHakarov = "curev inzv";     // הזמן הקרוב
    colZmaneiHayom = "ouhv hbnz kf";  // כל זמני היום
    yomBashavua = "gucac ouh";        // יום בשבוע
    yomLezadShaon = "iuga smk ouh";   // יום לצד שעון
    haftara = "vrypv";                // הפטרה
    taarichIvri = "hrcg lhrtw";       // תאריך עברי
    taarichLoazi = "hzguk lhrtw";     // תאריך לועזי
    dafHayomiBavli = "hkcc hnuhv qs";                   // דף היומי בבלי
    dafHayomiYerushalmi = "hnkaurh hnuhv qs";           // דף היומי ירושלמי
    zrihaSkiaaZeetHacochavim = "f\"vm = vghea = vjhrz"; // זריחה שקיעה צה"כ
    leloZmaneiHayom = "ouhv hbnz tkk";                  // ללא זמני היום
    meidaLamitpalel = "kkpwnk gshn";                    // מידע למתפלל
    zmaneiShabat = "wca hbnz";                          // זמני שבת
    hagimUmoadim = "ohsgunu ohdj";                      // חגים ומועדים
    talitAlotZriha = "vjhrz = rjav.g = w\"uy inz";      // זמן טו"ת - ע.השחר - זריחה
    sofZmanTfila = "vkhpw inz qux";                     // סוף זמן תפילה
    sofZmanKriatShma = "gna wthre inz qux";             // סוף זמן קריאת שמע
    hazotMinhaGMinhaK = ".e vjbn = .d vjbn = wumj";     // .חצות - מנחה ג. - מנחה ק
    plagShkiaaCochavim = "f\"vm = vghea = .n dkp";      // פלג מ. - שקיעה - צה"כ
    hazotAlotZriha = "vjhrz = rjav.g = wumj";           // חצות - ע.השחר - זריחה
    zrihaYomShkiaa = "vghea = ouh = vjhrz";             // זריחה - יום - שקיעה
    zrihaParashaShkiaa = "vghea = varp = vjhrz";        // זריחה - פרשה - שקיעה
    cShabatParashaZShabat = "wca .m = varp = wca .f";   // כ. שבת - פרשה - צ. שבת
    cShabatZShabatRT = "w\"r = wca .m = wca .f";        // כ. שבת - צ. שבת - ר"ת
    yemeHol = "kuj hnh";             // ימי חול
    shishiShabat = "wcau haha";      // שישי ושבת
    colHayom = "ouhv kf";            // כל היום
    bazmanHamatim = "ohtwnv inzc";   // בזמן המתאים
    bichlalLo = "tk kkfc";           // בכלל לא

    // Minhagim
    minhagTitle = "dvbn rjc";     // בחר מנהג
    edotHamizrah = "jrznv wusg";  // עדות המזרח
    sfarad = "srpx";              // ספרד
    ashkenaz = "zbfat";           // אשכנז
    teimanBaladi = "hskc inhw";   // תימן בלדי
    teimanShami = "hnta inhw";    // תימן שאמי
    chabad = "s\"cj";             // חב"ד
    bneiHul = "k\"uj hbc";        // בני חו"ל
    
    // Zmanim
    mishorit = "whruahn";                    // מישורית
    haniret = "whtrbv";                      // הנראית
    astronomit = "whnuburyxt";               // אסטרונומית
    rabenuTam = "ow ubcr";                   // רבנו תם
    zeetShabat = "wca wtm";                  // צאת שבת
    cnisatShabat = "wca wxhbf";              // כניסת שבת
    zeetHacochavim = "ohcfufv wtm";          // צאת הכוכבים
    zeetZomDerabanan = "ibcrs wunum wtm";    // צאת צומות דרבנן
    zmanTalitUtfilin = "ihkhpwu whky inz";   // זמן טלית ותפילין
    alotHashahar = "rjav wukg";              // עלות השחר
    hadlakatNerotMukdemet = "wnseun b\"vsv"; // הדה"נ מוקדמת
    dakot = "wues";                          // דקות
    zShabatMehashkiaa = "vgheavn wca .m";    // צ. שבת מהשקיעה
    irMukefetHoma = "vnuj wpeun rhg";        // עיר מוקפת חומה
    leilAhreiShkiaa = "vghea rjtk rjn khk";  // ליל מחר לאחר שקיעה
    zmanLanetzHaama = ":vnjv _bk inz";       // :זמן לנץ החמה
    sfiraLanetz = "_bk rujtk vrhpx";         // ספירה לאחור לנץ
    henetzHahama = "vnjv _bv";               // הנץ החמה
    hazmanShenotar = "rwuba inzv";           // הזמן שנותר
    shanimLahurban = "icrujk ohba";          // שנים לחורבן
    hazegColZmaneiHayom = "ouhv hbnz kf dmv";// הצג כל זמני היום

    // Config
    menuConfg = "wursdv";
    bt = "'wuyukc";
    on = "khgp";
    off = "hucf";
    resetPref = "wupsgv xupht";
    resetText = "ohyxey xupht";
    resetPass = "vnxhx xupht";
    aboutUs = "ubhwusut";
    aboutDevice = "rmunv hyrp";
    MemoryMap = "iurfz wpn";
    
    // About
    info1 = "sk hfxn";            // מסכי לד
    info2 = "hkyhdhs yukhau";     // ושילוט דיגיטלי
    phone = "0507933800";
    url = "www.ledhaarets.co.il";
    odotHamozr = ":rmunv wusut";
    model = ":ods";
    swVersion = ":vbfuw wxrd";
    hwVersion = ":vrnuj wxrd";
    serial = ":hrushx rpxn";
    lastUpdate = ":iurjt iufsg";

    // Reset
    resetTitle = "xupht";           // איפוס
    resetPrefRequest = "?wupsgv xptk";  // ?לאפס העדפות
    resetTextRequest = "?ohyxey xptk";  // ?לאפס טקסטים
    resetPassRequest = "?vnxhx xptk";  // ?לאפס סיסמה
    yes = "if";                     // כן
    no = "tk";                      // לא

    // Texts
	hazenText = "yxey izv";         // הזן טקסט
    TextsTitle = "ohyxey wfhrg";    // עריכת טקסטים
    hosef = "qxuv";                 // הוסף
    mchak = "ejn";                  // מחק
    aroch = "lurg";                 // ערוך

    brightnessTitle = "wurhvc";    // בהירות
    colorsTitle = "gcm";           // צבע
}

void Strings::setStringsEn()
{
    rtl=false;

    // Menu
    menuTitle = "Menu";         // תפריט
    menuClock = "Set clock";    // כיוון שעון 
    menuReset = "Reset";        // איפוס
    menuAbout = "About";        // אודות
    menuOffek = "Set ofek";     // בחר אופק
    menuTohen = "Layout";       // תוכן 

    // Ofek
    ofekTitle = "Ofek";        // אופק

    zriha = "sunrise";              // זריחה
    shkiaa = "sunset";              // שקיעה
    brightnessTitle = "Brightness"; // בהירות
}