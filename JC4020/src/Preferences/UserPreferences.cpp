
#include "UserPreferences.h"
#include <States/States.h>
#include "DataPrefs.h"
#include "LogicPrefs.h"
#include "PresentationPrefs.h"
#include "DisplayPrefs.h"
#include "SystemPrefs.h"
#include "UserContent.h"

UserPreferences::UserPreferences() : forceWrite("forceWrite", 0, PT_U8, 1, &prefs)
{
        // הוספת ההעדפות לווקטור לגישה גנרית
        simplePrefs.push_back(&forceWrite);
        simplePrefs.push_back(&dataPrefs.place.textPlace);
        simplePrefs.push_back(&dataPrefs.zmanim.shita);
        simplePrefs.push_back(&dataPrefs.minhagim.eda);
        simplePrefs.push_back(&dataPrefs.place.ofek);
        simplePrefs.push_back(&dataPrefs.place.nz_lat);
        simplePrefs.push_back(&dataPrefs.place.nz_long);
        simplePrefs.push_back(&dataPrefs.place.nz_height);
        simplePrefs.push_back(&dataPrefs.place.nz_hul);
        simplePrefs.push_back(&dataPrefs.place.timezone);
        simplePrefs.push_back(&displayPrefs.brightness);
        simplePrefs.push_back(&presentationPrefs.safa);
        simplePrefs.push_back(&displayPrefs.colorUp);
        simplePrefs.push_back(&displayPrefs.colorMiddle);
        simplePrefs.push_back(&displayPrefs.colorBottomT);
        simplePrefs.push_back(&displayPrefs.colorBottomV);
        simplePrefs.push_back(&displayPrefs.colorYom);
        simplePrefs.push_back(&displayPrefs.colorParasha);
        simplePrefs.push_back(&displayPrefs.colorIluy);
        simplePrefs.push_back(&displayPrefs.clockFont);
        simplePrefs.push_back(&presentationPrefs.dateFormat);
        simplePrefs.push_back(&presentationPrefs.clockFormat);
        simplePrefs.push_back(&presentationPrefs.timeFormat);
        simplePrefs.push_back(&dataPrefs.place.shaonKaiz);
        simplePrefs.push_back(&presentationPrefs.layout.middleLine);
        simplePrefs.push_back(&presentationPrefs.layout.upLine);
        simplePrefs.push_back(&dataPrefs.zmanim.zriha);
        simplePrefs.push_back(&dataPrefs.zmanim.shkiaa);
        simplePrefs.push_back(&dataPrefs.zmanim.rabenuTam);
        simplePrefs.push_back(&dataPrefs.zmanim.zeetCochavim);
        simplePrefs.push_back(&dataPrefs.zmanim.zmanTalit);
        simplePrefs.push_back(&dataPrefs.zmanim.alotHashahar);
        simplePrefs.push_back(&dataPrefs.zmanim.minhaG);
        simplePrefs.push_back(&dataPrefs.zmanim.sozksMga);
        simplePrefs.push_back(&dataPrefs.zmanim.soztMga);
        simplePrefs.push_back(&dataPrefs.zmanim.plagMinha);
        simplePrefs.push_back(&dataPrefs.zmanim.zeetShabat);
        simplePrefs.push_back(&dataPrefs.zmanim.zeetZom);
        simplePrefs.push_back(&dataPrefs.zmanim.nerot);
        simplePrefs.push_back(&dataPrefs.place.homa);
        simplePrefs.push_back(&dataPrefs.minhagim.hul);
        simplePrefs.push_back(&presentationPrefs.leil);
        simplePrefs.push_back(&logicPrefs.netzClk);
        simplePrefs.push_back(&dataPrefs.minhagim.shanim);
        simplePrefs.push_back(&logicPrefs.interval);
        simplePrefs.push_back(&logicPrefs.intervalIluy);
        simplePrefs.push_back(&logicPrefs.intervalTime);
        simplePrefs.push_back(&logicPrefs.intervalInfo);
        simplePrefs.push_back(&logicPrefs.dafHayomi);
        simplePrefs.push_back(&logicPrefs.yerushalmi);
        simplePrefs.push_back(&logicPrefs.yom);
        simplePrefs.push_back(&logicPrefs.parasha);
        simplePrefs.push_back(&logicPrefs.haftara);
        simplePrefs.push_back(&logicPrefs.ivri);
        simplePrefs.push_back(&logicPrefs.loazi);
        simplePrefs.push_back(&logicPrefs.tfila);
        simplePrefs.push_back(&logicPrefs.moadim);
        simplePrefs.push_back(&logicPrefs.showIsraeli);
        simplePrefs.push_back(&logicPrefs.TAZ);
        simplePrefs.push_back(&logicPrefs.SOZT);
        simplePrefs.push_back(&logicPrefs.SOZKS);
        simplePrefs.push_back(&logicPrefs.HMM);
        simplePrefs.push_back(&logicPrefs.PSZ);
        simplePrefs.push_back(&logicPrefs.HAZ);
        simplePrefs.push_back(&logicPrefs.ZSZ1);
        simplePrefs.push_back(&logicPrefs.ZSZ);
        simplePrefs.push_back(&logicPrefs.ZYS);
        simplePrefs.push_back(&logicPrefs.ZPS);
        simplePrefs.push_back(&logicPrefs.IPO);
        simplePrefs.push_back(&logicPrefs.IORT);
        simplePrefs.push_back(&displayPrefs.colorIvri);
        simplePrefs.push_back(&displayPrefs.colorLoazi);
        simplePrefs.push_back(&displayPrefs.colorParashaV);
        simplePrefs.push_back(&presentationPrefs.layout.screen);
        simplePrefs.push_back(&systemPrefs.bt);

        textPrefs.push_back(&userContent.textInput1);
        textPrefs.push_back(&userContent.textInput2);
        textPrefs.push_back(&userContent.textInput3);
        textPrefs.push_back(&userContent.textInput4);
        textPrefs.push_back(&userContent.textInput5);
        textPrefs.push_back(&userContent.textInput6);
        textPrefs.push_back(&userContent.textInput7);
        textPrefs.push_back(&userContent.textInput8);
        textPrefs.push_back(&userContent.textInput9);
        textPrefs.push_back(&userContent.textInput10);
        textPrefs.push_back(&userContent.textInput11);
        textPrefs.push_back(&userContent.textInput12);
        textPrefs.push_back(&userContent.textInput13);
        textPrefs.push_back(&userContent.textInput14);
        textPrefs.push_back(&userContent.textInput15);
        textPrefs.push_back(&userContent.textInput16);
        textPrefs.push_back(&userContent.textInput17);
        textPrefs.push_back(&userContent.textInput18);
        textPrefs.push_back(&userContent.textInput19);
        textPrefs.push_back(&userContent.textInput20);
        textPrefs.push_back(&userContent.textInput21);
        textPrefs.push_back(&userContent.textInput22);
        textPrefs.push_back(&userContent.textInput23);
        textPrefs.push_back(&userContent.textInput24);
        textPrefs.push_back(&userContent.textInput25);
        textPrefs.push_back(&userContent.textInput26);
        textPrefs.push_back(&userContent.textInput27);
        textPrefs.push_back(&userContent.textInput28);
        textPrefs.push_back(&userContent.textInput29);
        textPrefs.push_back(&userContent.textInput30);
        textPrefs.push_back(&userContent.textInput31);
        textPrefs.push_back(&userContent.textInput32);
        textPrefs.push_back(&userContent.textInput33);
        textPrefs.push_back(&userContent.textInput34);
        textPrefs.push_back(&userContent.textInput35);
        textPrefs.push_back(&userContent.textInput36);
        textPrefs.push_back(&userContent.textInput37);
        textPrefs.push_back(&userContent.textInput38);
        textPrefs.push_back(&userContent.textInput39);
        textPrefs.push_back(&userContent.textInput40);
        textPrefs.push_back(&userContent.textInput41);
        textPrefs.push_back(&userContent.textInput42);
        textPrefs.push_back(&userContent.textInput43);
        textPrefs.push_back(&userContent.textInput44);
        textPrefs.push_back(&userContent.textInput45);
        textPrefs.push_back(&userContent.textInput46);
        textPrefs.push_back(&userContent.textInput47);
        textPrefs.push_back(&userContent.textInput48);
        textPrefs.push_back(&userContent.textInput49);
        textPrefs.push_back(&userContent.textInput50);

        allPrefs.insert(allPrefs.end(), textPrefs.begin(), textPrefs.end());
        allPrefs.insert(allPrefs.end(), simplePrefs.begin(), simplePrefs.end());
}

UserPreferences userPreferences;

void setupPreferences()
{
    userPreferences.init("clk");
    userPreferences.loadAll();
}