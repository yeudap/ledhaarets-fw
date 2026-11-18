// ---------------------------------------------------------------------------
// PrefsViews: בדיקת קומפילציה בלבד - כל שדה בכל מבנה שכבה מפנה לשדה הנכון
// ב-userPreferences. לא מייצר קוד ולא נתונים.
// ---------------------------------------------------------------------------
#include "DataPrefs.h"
#include "LogicPrefs.h"
#include "PresentationPrefs.h"
#include "DisplayPrefs.h"
#include "SystemPrefs.h"
#include "UserContent.h"

// DataPrefs::place
static_assert(&dataPrefs.place.textPlace == &userPreferences.textPlace, "dataPrefs.place.textPlace");
static_assert(&dataPrefs.place.ofek == &userPreferences.ofek, "dataPrefs.place.ofek");
static_assert(&dataPrefs.place.nz_lat == &userPreferences.nz_lat, "dataPrefs.place.nz_lat");
static_assert(&dataPrefs.place.nz_long == &userPreferences.nz_long, "dataPrefs.place.nz_long");
static_assert(&dataPrefs.place.nz_height == &userPreferences.nz_height, "dataPrefs.place.nz_height");
static_assert(&dataPrefs.place.nz_hul == &userPreferences.nz_hul, "dataPrefs.place.nz_hul");
static_assert(&dataPrefs.place.timezone == &userPreferences.timezone, "dataPrefs.place.timezone");
static_assert(&dataPrefs.place.shaonKaiz == &userPreferences.shaonKaiz, "dataPrefs.place.shaonKaiz");
static_assert(&dataPrefs.place.homa == &userPreferences.homa, "dataPrefs.place.homa");

// DataPrefs::zmanim
static_assert(&dataPrefs.zmanim.shita == &userPreferences.shita, "dataPrefs.zmanim.shita");
static_assert(&dataPrefs.zmanim.zriha == &userPreferences.zriha, "dataPrefs.zmanim.zriha");
static_assert(&dataPrefs.zmanim.shkiaa == &userPreferences.shkiaa, "dataPrefs.zmanim.shkiaa");
static_assert(&dataPrefs.zmanim.rabenuTam == &userPreferences.rabenuTam, "dataPrefs.zmanim.rabenuTam");
static_assert(&dataPrefs.zmanim.zeetCochavim == &userPreferences.zeetCochavim, "dataPrefs.zmanim.zeetCochavim");
static_assert(&dataPrefs.zmanim.zmanTalit == &userPreferences.zmanTalit, "dataPrefs.zmanim.zmanTalit");
static_assert(&dataPrefs.zmanim.alotHashahar == &userPreferences.alotHashahar, "dataPrefs.zmanim.alotHashahar");
static_assert(&dataPrefs.zmanim.minhaG == &userPreferences.minhaG, "dataPrefs.zmanim.minhaG");
static_assert(&dataPrefs.zmanim.sozksMga == &userPreferences.sozksMga, "dataPrefs.zmanim.sozksMga");
static_assert(&dataPrefs.zmanim.soztMga == &userPreferences.soztMga, "dataPrefs.zmanim.soztMga");
static_assert(&dataPrefs.zmanim.plagMinha == &userPreferences.plagMinha, "dataPrefs.zmanim.plagMinha");
static_assert(&dataPrefs.zmanim.zeetShabat == &userPreferences.zeetShabat, "dataPrefs.zmanim.zeetShabat");
static_assert(&dataPrefs.zmanim.zeetZom == &userPreferences.zeetZom, "dataPrefs.zmanim.zeetZom");
static_assert(&dataPrefs.zmanim.nerot == &userPreferences.nerot, "dataPrefs.zmanim.nerot");

// DataPrefs::minhagim
static_assert(&dataPrefs.minhagim.eda == &userPreferences.eda, "dataPrefs.minhagim.eda");
static_assert(&dataPrefs.minhagim.hul == &userPreferences.hul, "dataPrefs.minhagim.hul");
static_assert(&dataPrefs.minhagim.shanim == &userPreferences.shanim, "dataPrefs.minhagim.shanim");

// LogicPrefs
static_assert(&logicPrefs.netzClk == &userPreferences.netzClk, "logicPrefs.netzClk");
static_assert(&logicPrefs.interval == &userPreferences.interval, "logicPrefs.interval");
static_assert(&logicPrefs.intervalIluy == &userPreferences.intervalIluy, "logicPrefs.intervalIluy");
static_assert(&logicPrefs.intervalTime == &userPreferences.intervalTime, "logicPrefs.intervalTime");
static_assert(&logicPrefs.intervalInfo == &userPreferences.intervalInfo, "logicPrefs.intervalInfo");
static_assert(&logicPrefs.dafHayomi == &userPreferences.dafHayomi, "logicPrefs.dafHayomi");
static_assert(&logicPrefs.yerushalmi == &userPreferences.yerushalmi, "logicPrefs.yerushalmi");
static_assert(&logicPrefs.yom == &userPreferences.yom, "logicPrefs.yom");
static_assert(&logicPrefs.parasha == &userPreferences.parasha, "logicPrefs.parasha");
static_assert(&logicPrefs.haftara == &userPreferences.haftara, "logicPrefs.haftara");
static_assert(&logicPrefs.ivri == &userPreferences.ivri, "logicPrefs.ivri");
static_assert(&logicPrefs.loazi == &userPreferences.loazi, "logicPrefs.loazi");
static_assert(&logicPrefs.tfila == &userPreferences.tfila, "logicPrefs.tfila");
static_assert(&logicPrefs.moadim == &userPreferences.moadim, "logicPrefs.moadim");
static_assert(&logicPrefs.showIsraeli == &userPreferences.showIsraeli, "logicPrefs.showIsraeli");
static_assert(&logicPrefs.TAZ == &userPreferences.TAZ, "logicPrefs.TAZ");
static_assert(&logicPrefs.SOZT == &userPreferences.SOZT, "logicPrefs.SOZT");
static_assert(&logicPrefs.SOZKS == &userPreferences.SOZKS, "logicPrefs.SOZKS");
static_assert(&logicPrefs.HMM == &userPreferences.HMM, "logicPrefs.HMM");
static_assert(&logicPrefs.PSZ == &userPreferences.PSZ, "logicPrefs.PSZ");
static_assert(&logicPrefs.HAZ == &userPreferences.HAZ, "logicPrefs.HAZ");
static_assert(&logicPrefs.ZSZ1 == &userPreferences.ZSZ1, "logicPrefs.ZSZ1");
static_assert(&logicPrefs.ZSZ == &userPreferences.ZSZ, "logicPrefs.ZSZ");
static_assert(&logicPrefs.ZYS == &userPreferences.ZYS, "logicPrefs.ZYS");
static_assert(&logicPrefs.ZPS == &userPreferences.ZPS, "logicPrefs.ZPS");
static_assert(&logicPrefs.IPO == &userPreferences.IPO, "logicPrefs.IPO");
static_assert(&logicPrefs.IORT == &userPreferences.IORT, "logicPrefs.IORT");

// PresentationPrefs
static_assert(&presentationPrefs.safa == &userPreferences.safa, "presentationPrefs.safa");
static_assert(&presentationPrefs.dateFormat == &userPreferences.dateFormat, "presentationPrefs.dateFormat");
static_assert(&presentationPrefs.clockFormat == &userPreferences.clockFormat, "presentationPrefs.clockFormat");
static_assert(&presentationPrefs.timeFormat == &userPreferences.timeFormat, "presentationPrefs.timeFormat");
static_assert(&presentationPrefs.leil == &userPreferences.leil, "presentationPrefs.leil");

// PresentationPrefs::layout
static_assert(&presentationPrefs.layout.middleLine == &userPreferences.middleLine, "presentationPrefs.layout.middleLine");
static_assert(&presentationPrefs.layout.upLine == &userPreferences.upLine, "presentationPrefs.layout.upLine");
static_assert(&presentationPrefs.layout.screen == &userPreferences.screen, "presentationPrefs.layout.screen");

// DisplayPrefs
static_assert(&displayPrefs.brightness == &userPreferences.brightness, "displayPrefs.brightness");
static_assert(&displayPrefs.colorUp == &userPreferences.colorUp, "displayPrefs.colorUp");
static_assert(&displayPrefs.colorMiddle == &userPreferences.colorMiddle, "displayPrefs.colorMiddle");
static_assert(&displayPrefs.colorBottomT == &userPreferences.colorBottomT, "displayPrefs.colorBottomT");
static_assert(&displayPrefs.colorBottomV == &userPreferences.colorBottomV, "displayPrefs.colorBottomV");
static_assert(&displayPrefs.colorYom == &userPreferences.colorYom, "displayPrefs.colorYom");
static_assert(&displayPrefs.colorParasha == &userPreferences.colorParasha, "displayPrefs.colorParasha");
static_assert(&displayPrefs.colorIluy == &userPreferences.colorIluy, "displayPrefs.colorIluy");
static_assert(&displayPrefs.clockFont == &userPreferences.clockFont, "displayPrefs.clockFont");
static_assert(&displayPrefs.colorIvri == &userPreferences.colorIvri, "displayPrefs.colorIvri");
static_assert(&displayPrefs.colorLoazi == &userPreferences.colorLoazi, "displayPrefs.colorLoazi");
static_assert(&displayPrefs.colorParashaV == &userPreferences.colorParashaV, "displayPrefs.colorParashaV");

// SystemPrefs
static_assert(&systemPrefs.bt == &userPreferences.bt, "systemPrefs.bt");

// UserContent
static_assert(&userContent.textPrefs == &userPreferences.textPrefs, "userContent.textPrefs");
