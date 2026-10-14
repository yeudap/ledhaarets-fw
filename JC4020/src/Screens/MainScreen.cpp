#include "Screens.h"


MainScreen::MainScreen()
{
    fullScreen = new Area(0, 0, 128, 64, BLACK);
    fullScreenArrow = new Area(0, 3, 128, 57, BLACK);
    // up
    upAll = new Area(0, 0, 128, 11, BLACK);
    ivriDate = new Area(53, 0, 74, 11, BLACK);
    loaziDate = new Area(1, 0, 53, 11, BLACK);
    // middle
    middleAll = new Area(0, 11, 128, 30, BLACK);
    uint8_t clkOffset = 1;
    uint8_t digitPlace = 38;
    uint8_t nekudotaimPlace = 6;
    hour = new Area(clkOffset, 11, digitPlace, 30, BLACK);
    nekudotaim1 = new Area(clkOffset+digitPlace, 11, nekudotaimPlace, 30, BLACK);
    minute = new Area(clkOffset+digitPlace+nekudotaimPlace, 11, digitPlace, 30, BLACK);
    nekudotaim2 = new Area(clkOffset+2*digitPlace+nekudotaimPlace, 11, nekudotaimPlace, 30, BLACK);
    second1 = new Area(clkOffset+digitPlace+nekudotaimPlace+digitPlace+nekudotaimPlace, 11, digitPlace/2, 30, BLACK);
    second2 = new Area(clkOffset+2*digitPlace+2*nekudotaimPlace+digitPlace/2, 11, digitPlace/2, 30, BLACK);
    yom = new Area(82, 11, 46, 30, BLACK);

    //down
    uint8_t highUp = 13;
    uint8_t highDown = 13;
    uint8_t highAll = highUp + highDown;
    uint8_t yUp = 38;
    uint8_t yDown = yUp + highUp;

    uint8_t xBegin = 0;
    uint8_t widthAll = 128;
    
    uint8_t widthLeft = 43;
    uint8_t widthMiddle = 42;
    uint8_t widthRight = 43;
    zmanRightUp = new Area(xBegin+widthLeft+widthMiddle, yUp, widthRight, highUp, BLACK);
    zmanRightDown = new Area(xBegin+widthLeft+widthMiddle, yDown, widthRight, highDown, BLACK);
    zmanMiddleUp = new Area(xBegin+widthLeft, yUp, widthMiddle, highUp, BLACK);
    zmanMiddleDown = new Area(xBegin+widthLeft, yDown, widthMiddle, highDown, BLACK);
    zmanLeftUp = new Area(xBegin, yUp, widthLeft, highUp, BLACK);
    zmanLeftDown = new Area(xBegin, yDown, widthLeft, highDown, BLACK);

    uint8_t widthLeftS = 32;
    uint8_t widthMiddleS = 64;
    uint8_t widthRightS = 32;
    zmanRightUpS = new Area(xBegin+widthLeftS+widthMiddleS, yUp, widthRightS, highUp, BLACK);
    zmanRightDownS = new Area(xBegin+widthLeftS+widthMiddleS, yDown, widthRightS, highDown, BLACK);
    zmanMiddleUpS = new Area(xBegin+widthLeftS, yUp, widthMiddleS, highUp, BLACK);
    zmanMiddleDownS = new Area(xBegin+widthLeftS, yDown, widthMiddleS, highDown, BLACK);
    zmanLeftUpS = new Area(xBegin, yUp, widthLeftS, highUp, BLACK);
    zmanLeftDownS = new Area(xBegin, yDown, widthLeftS, highDown, BLACK);

    infoAll = new Area(xBegin, yUp, widthAll, highAll, BLACK);
    infoUp = new Area(xBegin, yUp, widthAll, highUp, BLACK);
    infoDown = new Area(xBegin, yDown, widthAll, highDown, BLACK);

    uint8_t widthHalf = widthAll/2;
    infoUpHalfRight = new Area(xBegin+widthHalf, yUp, widthHalf, highUp, BLACK);
    infoUpHalfLeft = new Area(xBegin, yUp, widthHalf, highUp, BLACK);
    infoDownHalfRight = new Area(xBegin+widthHalf, yDown, widthHalf, highDown, BLACK);
    infoDownHalfLeft = new Area(xBegin, yDown, widthHalf, highDown, BLACK);

    uint8_t widthShort = 22;
    uint8_t widthHalfS = (widthAll-widthShort)/2;
    infoAllS = new Area(xBegin+widthShort, yUp, widthAll-widthShort, highAll, BLACK);
    infoUpS = new Area(xBegin+widthShort, yUp, widthAll-widthShort, highUp, BLACK);
    infoDownS = new Area(xBegin+widthShort, yDown, widthAll-widthShort, highDown, BLACK);
    infoUpHalfRightS = new Area(xBegin+widthShort+widthHalfS, yUp, widthHalfS, highUp, BLACK);
    infoUpHalfLeftS = new Area(xBegin+widthShort, yUp, widthHalfS, highUp, BLACK);
    infoShortUpLeft = new Area(xBegin, yUp, widthShort, highUp, BLACK);
    infoShortDownLeft = new Area(xBegin, yDown, widthShort, highDown, BLACK);
}
void MainScreen::update(Area* area, const char* str, Text &txt)
{
    area->clearArea();
    area->addText(str, txt);
}
void MainScreen::update(Area* area, Zman zman, Text &txt)
{
    char zmanStr[6];
    zman.toString(zmanStr);
    area->clearArea();
    area->addText(zmanStr, txt);
}
void MainScreen::printScreen()
{
    display->clearScreen();
}
void MainScreen::updateHour(char* _hour)
{
    Text txt = Text(colorMiddle, clockFont, 1, 26);
    hour->clearArea();
    hour->addText(_hour, txt);
}
void MainScreen::updateMinute(char* _minute)
{
    Text txt = Text(colorMiddle, clockFont, 1, 26);
    minute->clearArea();
    minute->addText(_minute, txt);
}
void MainScreen::updateSecond1(char* _second1)
{   
    Text txt = Text(colorMiddle, clockFont, 1, 26);
    second1->clearArea();
    second1->addText(_second1, txt);
}
void MainScreen::updateSecond2(char* _second2)
{   
    Text txt = Text(colorMiddle, clockFont, 0, 26);
    second2->clearArea();
    second2->addText(_second2, txt);
}
void MainScreen::updateNekudotaim1(char* _nekudotaim)
{   
    Text txt = Text(colorMiddle, clockFont, 0, 26);
    nekudotaim1->clearArea();
    nekudotaim1->addText(_nekudotaim, txt);
}
void MainScreen::updateNekudotaim2(char* _nekudotaim)
{   
    Text txt = Text(colorMiddle, clockFont, 0, 26);
    nekudotaim2->clearArea();
    nekudotaim2->addText(_nekudotaim, txt);
}
void MainScreen::updateHebDate(const char* _ivriDate1, const char* _ivriDate2)
{
    Text txt = Text(colorUp, &font6, 1, 8, RIGHT);
    ivriDate->clearArea();
    if(!ivriDate->textFits(_ivriDate2, txt))
    {
        ivriDate->addText(_ivriDate1, txt);
    }
    else{
        ivriDate->addText(_ivriDate2, txt);
    }
}
void MainScreen::updateHebDateFull(const char* _ivriDate)
{
    Text txt = Text(colorUp, &font6, 1, 8, CENTER);
    upAll->clearArea();
    upAll->addText(_ivriDate, txt);
}
void MainScreen::updateLoaziDate(const char* _loaziDate)
{
    Text txt = Text(colorUp, &font7, 0, 8, LEFT);
    loaziDate->clearArea();
    loaziDate->addText(_loaziDate, txt);
}
void MainScreen::updateYom(const char* _yomTitle, const char* _yom)
{
    yom->clearArea();
    Text txt = Text(colorMiddle, &font6, 1, 9, CENTER);
    yom->addText(_yomTitle, txt);
    txt = Text(colorMiddle, &ariel10, 0, 25, CENTER);
    yom->addText(_yom, txt);
}
