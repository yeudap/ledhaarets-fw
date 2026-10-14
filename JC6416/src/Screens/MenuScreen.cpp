#include "Screens.h"


MenuScreen::MenuScreen()
{
    title = new Area(0, 0, 128, 10, BLACK);
    option1 = new Area(0, 14, 128, 13, BLACK);
    option2 = new Area(1, 30, 127, 13, RED);
    option3 = new Area(0, 46, 128, 13, BLACK);
    kezev = new Area(53, 29, 19, 14, RED);
    nerot = new Area(43, 29, 16, 14, RED);
    timeZone = new Area(70, 29, 16, 14, RED);
    clkFormat = new Area(28, 29, 47, 14, RED);
    bt = new Area(62, 29, 28, 14, RED);
    format = new Area(7, 29, 60, 14, RED);
    zmanFormat = new Area(48, 29, 33, 14, RED);
    netz = new Area(34, 29, 15, 14, RED);
    shanim = new Area(43, 29, 24, 14, RED);
    kaiz = new Area(17, 29, 70, 14, RED);
}
void MenuScreen::printScreen()
{
    display->clearScreen();
    title->clearArea();
    option1->clearArea();
    option2->clearArea();
    option3->clearArea();
}
void MenuScreen::printMenu(const char* _title, const char* a, const char* b, const char* c)
{
    printScreen();
    printTitle(_title);
    Text txt = Text(WHITE, &font7, 2, 9, RIGHT);
    option1->addText(a, txt);
    option2->addText(b, txt);
    option3->addText(c, txt);
}
void MenuScreen::printTitle(const char* _title)
{
    Text txt = Text(BLUE, &font7, 0, 7, CENTER);
    title->addText(_title, txt);
    title->addRect(true, 10,10,108,1, BLUE);
}
void MenuScreen::addZman(Zman _zman, bool sec)
{
    Text txt = Text(WHITE, &font7, 2, 9, LEFT);
    char zmanStr[10];
    if(sec) {_zman.toStringSec(zmanStr);}
    else {_zman.toString(zmanStr); }
    option2->addText(zmanStr, txt);
}
