#include "Screens.h"
#include "Preferences/PresentationPrefs.h"
#include "Preferences/DisplayPrefs.h"
#include "Preferences/LogicPrefs.h"
#include "Preferences/DataPrefs.h"
extern UserPreferences userPreferences;

RegsScreen::RegsScreen()
{
    a1 = new Area(1,  0, 15, 10, BLACK);
    a2 = new Area(1,  9, 15, 10, BLACK);
    a3 = new Area(1, 18, 15, 10, BLACK);
    a4 = new Area(1, 27, 15, 10, BLACK);
    a5 = new Area(1, 36, 15, 10, BLACK);
    a6 = new Area(1, 45, 15, 10, BLACK);
    a7 = new Area(1, 54, 15, 10, BLACK);

    b1 = new Area(15,  0, 15, 10, BLACK);
    b2 = new Area(15,  9, 15, 10, BLACK);
    b3 = new Area(15, 18, 15, 10, BLACK);
    b4 = new Area(15, 27, 15, 10, BLACK);
    b5 = new Area(15, 36, 15, 10, BLACK);
    b6 = new Area(15, 45, 15, 10, BLACK);
    b7 = new Area(15, 54, 15, 10, BLACK);

    c1 = new Area(29,  0, 15, 10, BLACK);
    c2 = new Area(29,  9, 15, 10, BLACK);
    c3 = new Area(29, 18, 15, 10, BLACK);
    c4 = new Area(29, 27, 15, 10, BLACK);
    c5 = new Area(29, 36, 15, 10, BLACK);
    c6 = new Area(29, 45, 15, 10, BLACK);
    c7 = new Area(29, 54, 15, 10, BLACK);

    d1 = new Area(43,  0, 15, 10, BLACK);
    d2 = new Area(43,  9, 15, 10, BLACK);
    d3 = new Area(43, 18, 15, 10, BLACK);
    d4 = new Area(43, 27, 15, 10, BLACK);
    d5 = new Area(43, 36, 15, 10, BLACK);
    d6 = new Area(43, 45, 15, 10, BLACK);
    d7 = new Area(43, 54, 15, 10, BLACK);

    e1 = new Area(57,  0, 15, 10, BLACK);
    e2 = new Area(57,  9, 15, 10, BLACK);
    e3 = new Area(57, 18, 15, 10, BLACK);
    e4 = new Area(57, 27, 15, 10, BLACK);
    e5 = new Area(57, 36, 15, 10, BLACK);
    e6 = new Area(57, 45, 15, 10, BLACK);
    e7 = new Area(57, 54, 15, 10, BLACK);

    f1 = new Area(71,  0, 15, 10, BLACK);
    f2 = new Area(71,  9, 15, 10, BLACK);
    f3 = new Area(71, 18, 15, 10, BLACK);
    f4 = new Area(71, 27, 15, 10, BLACK);
    f5 = new Area(71, 36, 15, 10, BLACK);
    f6 = new Area(71, 45, 15, 10, BLACK);
    f7 = new Area(71, 54, 15, 10, BLACK);

    g1 = new Area(85,  0, 15, 10, BLACK);
    g2 = new Area(85,  9, 15, 10, BLACK);
    g3 = new Area(85, 18, 15, 10, BLACK);
    g4 = new Area(85, 27, 15, 10, BLACK);
    g5 = new Area(85, 36, 15, 10, BLACK);
    g6 = new Area(85, 45, 15, 10, BLACK);
    g7 = new Area(85, 54, 15, 10, BLACK);

    h1 = new Area(99,  0, 15, 10, BLACK);
    h2 = new Area(99,  9, 15, 10, BLACK);
    h3 = new Area(99, 18, 15, 10, BLACK);
    h4 = new Area(99, 27, 15, 10, BLACK);
    h5 = new Area(99, 36, 15, 10, BLACK);
    h6 = new Area(99, 45, 15, 10, BLACK);
    h7 = new Area(99, 54, 15, 10, BLACK);

    i1 = new Area(113,  0, 15, 10, BLACK);
    i2 = new Area(113,  9, 15, 10, BLACK);
    i3 = new Area(113, 18, 15, 10, BLACK);
    i4 = new Area(113, 27, 15, 10, BLACK);
    i5 = new Area(113, 36, 15, 10, BLACK);
    i6 = new Area(113, 45, 15, 10, BLACK);
    i7 = new Area(113, 54, 15, 10, BLACK);

}

void RegsScreen::printScreen()
{
    display->clearScreen();
    a1->clearArea();
    a2->clearArea();
    a3->clearArea();
    a4->clearArea();
    a5->clearArea();
    a6->clearArea();
    a7->clearArea();
    b1->clearArea();
    b2->clearArea();
    b3->clearArea();
    b4->clearArea();
    b5->clearArea();
    b6->clearArea();
    b7->clearArea();
    c1->clearArea();
    c2->clearArea();
    c3->clearArea();
    c4->clearArea();
    c5->clearArea();
    c6->clearArea();
    c7->clearArea();
    d1->clearArea();
    d2->clearArea();
    d3->clearArea();
    d4->clearArea();
    d5->clearArea();
    d6->clearArea();
    d7->clearArea();
    e1->clearArea();
    e2->clearArea();
    e3->clearArea();
    e4->clearArea();
    e5->clearArea();
    e6->clearArea();
    e7->clearArea();
    f1->clearArea();
    f2->clearArea();
    f3->clearArea();
    f4->clearArea();
    f5->clearArea();
    f6->clearArea();
    f7->clearArea();
    g1->clearArea();
    g2->clearArea();
    g3->clearArea();
    g4->clearArea();
    g5->clearArea();
    g6->clearArea();
    g7->clearArea();
    h1->clearArea();
    h2->clearArea();
    h3->clearArea();
    h4->clearArea();
    h5->clearArea();
    h6->clearArea();
    h7->clearArea();
    i1->clearArea();
    i2->clearArea();
    i3->clearArea();
    i4->clearArea();
    i5->clearArea();
    i6->clearArea();
    i7->clearArea();

    printValues();

    a1->addRectBorder();
    a2->addRectBorder();
    a3->addRectBorder();
    a4->addRectBorder();
    a5->addRectBorder();
    a6->addRectBorder();
    a7->addRectBorder();
    b1->addRectBorder();
    b2->addRectBorder();
    b3->addRectBorder();
    b4->addRectBorder();
    b5->addRectBorder();
    b6->addRectBorder();
    b7->addRectBorder();
    c1->addRectBorder();
    c2->addRectBorder();
    c3->addRectBorder();
    c4->addRectBorder();
    c5->addRectBorder();
    c6->addRectBorder();
    c7->addRectBorder();
    d1->addRectBorder();
    d2->addRectBorder();
    d3->addRectBorder();
    d4->addRectBorder();
    d5->addRectBorder();
    d6->addRectBorder();
    d7->addRectBorder();
    e1->addRectBorder();
    e2->addRectBorder();
    e3->addRectBorder();
    e4->addRectBorder();
    e5->addRectBorder();
    e6->addRectBorder();
    e7->addRectBorder();
    f1->addRectBorder();
    f2->addRectBorder();
    f3->addRectBorder();
    f4->addRectBorder();
    f5->addRectBorder();
    f6->addRectBorder();
    f7->addRectBorder();
    g1->addRectBorder();
    g2->addRectBorder();
    g3->addRectBorder();
    g4->addRectBorder();
    g5->addRectBorder();
    g6->addRectBorder();
    g7->addRectBorder();
    h1->addRectBorder();
    h2->addRectBorder();
    h3->addRectBorder();
    h4->addRectBorder();
    h5->addRectBorder();
    h6->addRectBorder();
    h7->addRectBorder();
    i1->addRectBorder();
    i2->addRectBorder();
    i3->addRectBorder();
    i4->addRectBorder();
    i5->addRectBorder();
    i6->addRectBorder();
    i7->addRectBorder();
}
void printValue(Area* area, int value, int defaultValue)
{
    Text txt = Text(GREEN, &font6, 0, 7, CENTER);
    if(value!=defaultValue) { txt.color=RED; }
    area->addText(value, txt);
}
void RegsScreen::printValues()
{
    printValue(a1, (int)dataPrefs.zmanim.shita, (int)dataPrefs.zmanim.shita.getDefault());
    printValue(a2, (int)dataPrefs.minhagim.eda, (int)dataPrefs.minhagim.eda.getDefault());
    printValue(a3, (int)dataPrefs.place.ofek, (int)dataPrefs.place.ofek.getDefault());
    printValue(a4, (int)dataPrefs.place.timezone, (int)dataPrefs.place.timezone.getDefault());
    printValue(a5, (int)displayPrefs.clockFont, (int)displayPrefs.clockFont.getDefault());
    printValue(a6, (int)presentationPrefs.dateFormat, (int)presentationPrefs.dateFormat.getDefault());
    printValue(a7, (int)presentationPrefs.clockFormat, (int)presentationPrefs.clockFormat.getDefault());
    printValue(b1, (int)dataPrefs.place.shaonKaiz, (int)dataPrefs.place.shaonKaiz.getDefault());
    printValue(b2, (int)presentationPrefs.layout.middleLine, (int)presentationPrefs.layout.middleLine.getDefault());
    printValue(b3, (int)presentationPrefs.layout.upLine, (int)presentationPrefs.layout.upLine.getDefault());
    printValue(b4, (int)dataPrefs.zmanim.zriha, (int)dataPrefs.zmanim.zriha.getDefault());
    printValue(b5, (int)dataPrefs.zmanim.shkiaa, (int)dataPrefs.zmanim.shkiaa.getDefault());
    printValue(b6, (int)dataPrefs.zmanim.rabenuTam, (int)dataPrefs.zmanim.rabenuTam.getDefault());
    printValue(b7, (int)dataPrefs.zmanim.zeetCochavim, (int)dataPrefs.zmanim.zeetCochavim.getDefault());
    printValue(c1, (int)dataPrefs.zmanim.zmanTalit, (int)dataPrefs.zmanim.zmanTalit.getDefault());
    printValue(c2, (int)dataPrefs.zmanim.alotHashahar, (int)dataPrefs.zmanim.alotHashahar.getDefault());
    printValue(c3, (int)dataPrefs.zmanim.minhaG, (int)dataPrefs.zmanim.minhaG.getDefault());
    printValue(c4, (int)dataPrefs.zmanim.zeetShabat, (int)dataPrefs.zmanim.zeetShabat.getDefault());
    printValue(c5, (int)dataPrefs.zmanim.zeetZom, (int)dataPrefs.zmanim.zeetZom.getDefault());
    printValue(c6, (int)dataPrefs.zmanim.nerot, (int)dataPrefs.zmanim.nerot.getDefault());
    printValue(c7, (int)dataPrefs.minhagim.hul, (int)dataPrefs.minhagim.hul.getDefault());
    printValue(d1, (int)presentationPrefs.leil, (int)presentationPrefs.leil.getDefault());
    printValue(d2, (int)presentationPrefs.leil, (int)presentationPrefs.leil.getDefault());
    printValue(d3, (int)logicPrefs.dafHayomi, (int)logicPrefs.dafHayomi.getDefault());
    printValue(d4, (int)logicPrefs.yerushalmi, (int)logicPrefs.yerushalmi.getDefault());
    printValue(d5, (int)logicPrefs.yom, (int)logicPrefs.yom.getDefault());
    printValue(d6, (int)logicPrefs.parasha, (int)logicPrefs.parasha.getDefault());
    printValue(d7, (int)logicPrefs.tfila, (int)logicPrefs.tfila.getDefault());
    printValue(e1, (int)logicPrefs.moadim, (int)logicPrefs.moadim.getDefault());
    printValue(e2, (int)logicPrefs.showIsraeli, (int)logicPrefs.showIsraeli.getDefault());
    printValue(e3, (int)logicPrefs.TAZ, (int)logicPrefs.TAZ.getDefault());
    printValue(e4, (int)logicPrefs.SOZT, (int)logicPrefs.SOZT.getDefault());
    printValue(e5, (int)logicPrefs.SOZKS, (int)logicPrefs.SOZKS.getDefault());
    printValue(e6, (int)logicPrefs.HMM, (int)logicPrefs.HMM.getDefault());
    printValue(e7, (int)logicPrefs.PSZ, (int)logicPrefs.PSZ.getDefault());
    printValue(f1, (int)logicPrefs.HAZ, (int)logicPrefs.HAZ.getDefault());
    printValue(f2, (int)logicPrefs.ZSZ1, (int)logicPrefs.ZSZ1.getDefault());
    printValue(f3, (int)logicPrefs.ZSZ, (int)logicPrefs.ZSZ.getDefault());
    printValue(f4, (int)logicPrefs.ZYS, (int)logicPrefs.ZYS.getDefault());
    printValue(f5, (int)logicPrefs.ZPS, (int)logicPrefs.ZPS.getDefault());
    printValue(f6, (int)logicPrefs.IPO, (int)logicPrefs.IPO.getDefault());
    printValue(f7, (int)logicPrefs.IORT, (int)logicPrefs.IORT.getDefault());
    printValue(g1, (int)displayPrefs.colorUp, (int)displayPrefs.colorUp.getDefault());
    printValue(g2, (int)displayPrefs.colorMiddle, (int)displayPrefs.colorMiddle.getDefault());
    printValue(g3, (int)displayPrefs.colorBottomT, (int)displayPrefs.colorBottomT.getDefault());
    printValue(g4, (int)displayPrefs.colorBottomV, (int)displayPrefs.colorBottomV.getDefault());
    printValue(g5, (int)displayPrefs.colorYom, (int)displayPrefs.colorYom.getDefault());
    printValue(g6, (int)displayPrefs.colorParasha, (int)displayPrefs.colorParasha.getDefault());
    printValue(g7, (int)displayPrefs.colorIluy, (int)displayPrefs.colorIluy.getDefault());
    printValue(h1, (int)dataPrefs.place.homa, (int)dataPrefs.place.homa.getDefault());
    printValue(h2, (int)logicPrefs.netzClk, (int)logicPrefs.netzClk.getDefault());
    printValue(h3, (int)dataPrefs.minhagim.shanim, (int)dataPrefs.minhagim.shanim.getDefault());
    printValue(h4, (int)logicPrefs.interval, (int)logicPrefs.interval.getDefault());
    printValue(h5, (int)logicPrefs.intervalIluy, (int)logicPrefs.intervalIluy.getDefault());
    printValue(h6, (int)logicPrefs.intervalTime, (int)logicPrefs.intervalTime.getDefault());
    printValue(h7, (int)logicPrefs.intervalInfo, (int)logicPrefs.intervalInfo.getDefault());
        
    double_t val;
    val = dataPrefs.place.nz_long;
    int lonPre = 1;
    if (val < 0) { lonPre = -1; }
    val *= lonPre; // abs
    int lonInt = int(val);
    int lonFrac = (val-lonInt)*100;
    val = dataPrefs.place.nz_lat;
    int latPre = 1;
    if (val < 0) { latPre = -1; }
    val *= latPre; // abs
    int latInt = int(val);
    int latFrac = (val-latInt)*100;
    val = dataPrefs.place.nz_long.getDefault();
    int lonPreDef = 1;
    if (val < 0) { lonPreDef = -1; }
    val *= lonPreDef; // abs
    int lonIntDef = int(val);
    int lonFracDef = (val-lonIntDef)*100;
    val = dataPrefs.place.nz_lat.getDefault();
    int latPreDef = 1;
    if (val < 0) { latPreDef = -1; }
    val *= latPreDef; // abs
    int latIntDef = int(val);
    int latFracDef = (val-latIntDef)*100;
    printValue(i1, (int)dataPrefs.place.nz_height, (int)dataPrefs.place.nz_height.getDefault());
    printValue(i2, (int)lonPre, (int)lonPreDef);
    printValue(i3, (int)lonInt, (int)lonIntDef);
    printValue(i4, (int)lonFrac, (int)lonFracDef);
    printValue(i5, (int)latPre, (int)latPreDef);
    printValue(i6, (int)latInt, (int)latIntDef);
    printValue(i7, (int)latFrac, (int)latFracDef);
}