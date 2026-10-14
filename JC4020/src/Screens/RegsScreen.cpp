#include "Screens.h"
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
    printValue(a1, (int)userPreferences.shita, (int)userPreferences.shita.getDefault());
    printValue(a2, (int)userPreferences.eda, (int)userPreferences.eda.getDefault());
    printValue(a3, (int)userPreferences.ofek, (int)userPreferences.ofek.getDefault());
    printValue(a4, (int)userPreferences.timezone, (int)userPreferences.timezone.getDefault());
    printValue(a5, (int)userPreferences.clockFont, (int)userPreferences.clockFont.getDefault());
    printValue(a6, (int)userPreferences.dateFormat, (int)userPreferences.dateFormat.getDefault());
    printValue(a7, (int)userPreferences.clockFormat, (int)userPreferences.clockFormat.getDefault());
    printValue(b1, (int)userPreferences.shaonKaiz, (int)userPreferences.shaonKaiz.getDefault());
    printValue(b2, (int)userPreferences.middleLine, (int)userPreferences.middleLine.getDefault());
    printValue(b3, (int)userPreferences.upLine, (int)userPreferences.upLine.getDefault());
    printValue(b4, (int)userPreferences.zriha, (int)userPreferences.zriha.getDefault());
    printValue(b5, (int)userPreferences.shkiaa, (int)userPreferences.shkiaa.getDefault());
    printValue(b6, (int)userPreferences.rabenuTam, (int)userPreferences.rabenuTam.getDefault());
    printValue(b7, (int)userPreferences.zeetCochavim, (int)userPreferences.zeetCochavim.getDefault());
    printValue(c1, (int)userPreferences.zmanTalit, (int)userPreferences.zmanTalit.getDefault());
    printValue(c2, (int)userPreferences.alotHashahar, (int)userPreferences.alotHashahar.getDefault());
    printValue(c3, (int)userPreferences.minhaG, (int)userPreferences.minhaG.getDefault());
    printValue(c4, (int)userPreferences.zeetShabat, (int)userPreferences.zeetShabat.getDefault());
    printValue(c5, (int)userPreferences.zeetZom, (int)userPreferences.zeetZom.getDefault());
    printValue(c6, (int)userPreferences.nerot, (int)userPreferences.nerot.getDefault());
    printValue(c7, (int)userPreferences.hul, (int)userPreferences.hul.getDefault());
    printValue(d1, (int)userPreferences.leil, (int)userPreferences.leil.getDefault());
    printValue(d2, (int)userPreferences.leil, (int)userPreferences.leil.getDefault());
    printValue(d3, (int)userPreferences.dafHayomi, (int)userPreferences.dafHayomi.getDefault());
    printValue(d4, (int)userPreferences.yerushalmi, (int)userPreferences.yerushalmi.getDefault());
    printValue(d5, (int)userPreferences.yom, (int)userPreferences.yom.getDefault());
    printValue(d6, (int)userPreferences.parasha, (int)userPreferences.parasha.getDefault());
    printValue(d7, (int)userPreferences.tfila, (int)userPreferences.tfila.getDefault());
    printValue(e1, (int)userPreferences.moadim, (int)userPreferences.moadim.getDefault());
    printValue(e2, (int)userPreferences.showIsraeli, (int)userPreferences.showIsraeli.getDefault());
    printValue(e3, (int)userPreferences.TAZ, (int)userPreferences.TAZ.getDefault());
    printValue(e4, (int)userPreferences.SOZT, (int)userPreferences.SOZT.getDefault());
    printValue(e5, (int)userPreferences.SOZKS, (int)userPreferences.SOZKS.getDefault());
    printValue(e6, (int)userPreferences.HMM, (int)userPreferences.HMM.getDefault());
    printValue(e7, (int)userPreferences.PSZ, (int)userPreferences.PSZ.getDefault());
    printValue(f1, (int)userPreferences.HAZ, (int)userPreferences.HAZ.getDefault());
    printValue(f2, (int)userPreferences.ZSZ1, (int)userPreferences.ZSZ1.getDefault());
    printValue(f3, (int)userPreferences.ZSZ, (int)userPreferences.ZSZ.getDefault());
    printValue(f4, (int)userPreferences.ZYS, (int)userPreferences.ZYS.getDefault());
    printValue(f5, (int)userPreferences.ZPS, (int)userPreferences.ZPS.getDefault());
    printValue(f6, (int)userPreferences.IPO, (int)userPreferences.IPO.getDefault());
    printValue(f7, (int)userPreferences.IORT, (int)userPreferences.IORT.getDefault());
    printValue(g1, (int)userPreferences.colorUp, (int)userPreferences.colorUp.getDefault());
    printValue(g2, (int)userPreferences.colorMiddle, (int)userPreferences.colorMiddle.getDefault());
    printValue(g3, (int)userPreferences.colorBottomT, (int)userPreferences.colorBottomT.getDefault());
    printValue(g4, (int)userPreferences.colorBottomV, (int)userPreferences.colorBottomV.getDefault());
    printValue(g5, (int)userPreferences.colorYom, (int)userPreferences.colorYom.getDefault());
    printValue(g6, (int)userPreferences.colorParasha, (int)userPreferences.colorParasha.getDefault());
    printValue(g7, (int)userPreferences.colorIluy, (int)userPreferences.colorIluy.getDefault());
    printValue(h1, (int)userPreferences.homa, (int)userPreferences.homa.getDefault());
    printValue(h2, (int)userPreferences.netzClk, (int)userPreferences.netzClk.getDefault());
    printValue(h3, (int)userPreferences.shanim, (int)userPreferences.shanim.getDefault());
    printValue(h4, (int)userPreferences.interval, (int)userPreferences.interval.getDefault());
    printValue(h5, (int)userPreferences.intervalIluy, (int)userPreferences.intervalIluy.getDefault());
    printValue(h6, (int)userPreferences.intervalTime, (int)userPreferences.intervalTime.getDefault());
    printValue(h7, (int)userPreferences.intervalInfo, (int)userPreferences.intervalInfo.getDefault());
        
    double_t val;
    val = userPreferences.nz_long;
    int lonPre = 1;
    if (val < 0) { lonPre = -1; }
    val *= lonPre; // abs
    int lonInt = int(val);
    int lonFrac = (val-lonInt)*100;
    val = userPreferences.nz_lat;
    int latPre = 1;
    if (val < 0) { latPre = -1; }
    val *= latPre; // abs
    int latInt = int(val);
    int latFrac = (val-latInt)*100;
    val = userPreferences.nz_long.getDefault();
    int lonPreDef = 1;
    if (val < 0) { lonPreDef = -1; }
    val *= lonPreDef; // abs
    int lonIntDef = int(val);
    int lonFracDef = (val-lonIntDef)*100;
    val = userPreferences.nz_lat.getDefault();
    int latPreDef = 1;
    if (val < 0) { latPreDef = -1; }
    val *= latPreDef; // abs
    int latIntDef = int(val);
    int latFracDef = (val-latIntDef)*100;
    printValue(i1, (int)userPreferences.nz_height, (int)userPreferences.nz_height.getDefault());
    printValue(i2, (int)lonPre, (int)lonPreDef);
    printValue(i3, (int)lonInt, (int)lonIntDef);
    printValue(i4, (int)lonFrac, (int)lonFracDef);
    printValue(i5, (int)latPre, (int)latPreDef);
    printValue(i6, (int)latInt, (int)latIntDef);
    printValue(i7, (int)latFrac, (int)latFracDef);
}