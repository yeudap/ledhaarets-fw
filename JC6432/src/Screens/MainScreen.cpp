#include "Screens.h"
#include <pgmspace.h>

MainScreen::MainScreen() 
{
    numOfMainScreens = 2;
}
void MainScreen::setScreen(uint8_t screen)
{
    display->clearScreen();
    if(screen == 1)
    {
        Big();
    }
    if(screen == 2)
    {
        Small();
    }
    h0 = Digit(x0,y);
    h1 = Digit(x0+dw,y);
    m0 = Digit(x0+dw+nw,y);
    m1 = Digit(x0+dw+nw+dw,y);
    s0 = Digit(x0+dw+nw+dw+nw,y);
    s1 = Digit(x0+dw+nw+dw+nw+dw,y);
}
void MainScreen::Small()
{
    // iluy
    uint8_t h_iluy = 18;
    iluyNishmat = new Area(0, 0, 256, 19, BLACK);

    // clock
    // x0 = 11;
    // dw = 24;
    // nw = 35;

    uint8_t digitWidth = pgm_read_byte(&clockFont->glyph['8' - clockFont->first].xAdvance);
    uint8_t nekudotaimWidth = pgm_read_byte(&clockFont->glyph[':' - clockFont->first].xAdvance);
    // Serial.print("nekudotaimWidth = ");
    // Serial.println(nekudotaimWidth);
    uint8_t digitPlace = digitWidth*2;
    uint8_t nekudotaimPlace = nekudotaimWidth;
    // Serial.print("nekudotaimPlace = ");
    // Serial.println(nekudotaimPlace);
    uint8_t clkPlace = 3*digitPlace+2*nekudotaimPlace;
    uint8_t clkOffset = 0;
    if (180-clkPlace > 0) clkOffset = (180-clkPlace)/2;
    // Serial.print("clkOffset: ");
    // Serial.println(clkOffset);
    uint8_t h_clock = 38;
    uint8_t clk_y = 18; 
    y = clk_y+4;
    x0 = clkOffset+4;
    dw = digitWidth;
    nw = digitWidth+nekudotaimPlace;
    // Serial.print("y: ");
    // Serial.println(y);
    // Serial.print("x0: ");
    // Serial.println(x0);
    // Serial.print("dw: ");
    // Serial.println(dw);
    // Serial.print("nw: ");
    // Serial.println(nw);
    hour = new Area(clkOffset, clk_y, digitPlace, h_clock, BLACK);
    nekudotaim1 = new Area(clkOffset+digitPlace, clk_y, nekudotaimPlace, h_clock, BLACK);
    minute = new Area(clkOffset+digitPlace+nekudotaimPlace, clk_y, digitPlace, h_clock, BLACK);
    nekudotaim2 = new Area(clkOffset+2*digitPlace+nekudotaimPlace, clk_y, nekudotaimPlace, h_clock, BLACK);
    second1 = new Area(clkOffset+digitPlace+nekudotaimPlace+digitPlace+nekudotaimPlace, clk_y, digitPlace/2, h_clock, BLACK);
    second2 = new Area(clkOffset+2*digitPlace+2*nekudotaimPlace+digitPlace/2, clk_y, digitPlace/2, h_clock, BLACK);
    clockAll = new Area(clkOffset, y-1, clkPlace, h_clock+3, BLACK);

    // date
    uint8_t x_date_yom = clkPlace;
    if (clkPlace<180) x_date_yom = 180;
    loaziDate = new Area(x_date_yom, 19, 256-x_date_yom, 19, BLACK);
    ivriDate = new Area(x_date_yom, 20+19, 256-x_date_yom, 19, BLACK);

    // info
    uint8_t h_info = 18;
    uint8_t y_zmanUp = 56;
    uint8_t y_zmanDown = 74;
    uint8_t w_zman = 180;
    uint8_t w_parasha = 256-w_zman;

    middleTitleAll = new Area(0, y_zmanUp-1, 255, h_info+1, BLACK);
    middleValueAll = new Area(0, y_zmanDown-1, 255, h_info+1, BLACK);
    parashaTitle = new Area(w_zman, y_zmanUp, w_parasha, h_info, BLACK);
    parashaValue = new Area(w_zman, y_zmanDown, w_parasha, h_info, BLACK);
    parashaAll = new Area(w_zman, y_zmanUp, w_parasha, h_info*2, BLACK);

    if(userPreferences.dateFormat != 8) yom = parashaTitle; // יש לועזי
    else yom = loaziDate;

    zmanAll = new Area(0, y_zmanUp, w_zman, h_info*2, BLACK);
    zmanUp = new Area(0, y_zmanUp, w_zman, h_info, BLACK);
    zmanDown = new Area(0, y_zmanDown, w_zman, h_info, BLACK);
    
    // infoAll = new Area(0, y_info-1, 256, h_info+1, BLACK);
    // infoRight = new Area(128, y_info, 128, h_info, BLACK);
    // infoLeft = new Area(0, y_info, 128, h_info, BLACK);
    // info2 = new Area(0, 110, 256, 19, BLACK);

    uint8_t w_time = 60;
    zmanRightUp =    new Area(w_time*2, y_zmanUp, w_time, h_info, BLACK);
    zmanRightDown =  new Area(w_time*2, y_zmanDown, w_time, h_info, BLACK);
    zmanMiddle =     new Area(w_time, y_zmanUp+h_info/2, w_time, h_info, BLACK);
    zmanMiddleUp =   new Area(w_time, y_zmanUp, w_time, h_info, BLACK);
    zmanMiddleDown = new Area(w_time, y_zmanDown, w_time, h_info, BLACK);
    zmanLeftUp =     new Area(0, y_zmanUp, w_time, h_info, BLACK);
    zmanLeftDown =   new Area(0, y_zmanDown, w_time, h_info, BLACK);

    uint8_t y_infoUp = 92;
    uint8_t y_infoDown = y_infoUp+h_info;

    uint8_t xBegin = 0;
    uint16_t widthAll = 256;
    uint8_t widthLeft = 85;
    uint8_t widthMiddle = 86;
    uint8_t widthRight = 85;

    zmanInfoRightUp = new Area(xBegin+widthLeft+widthMiddle, y_infoUp, widthRight, h_info, BLACK);
    zmanInfoRightDown = new Area(xBegin+widthLeft+widthMiddle, y_infoDown, widthRight, h_info, BLACK);
    zmanInfoMiddleUp = new Area(xBegin+widthLeft, y_infoUp, widthMiddle, h_info, BLACK);
    zmanInfoMiddleDown = new Area(xBegin+widthLeft, y_infoDown, widthMiddle, h_info, BLACK);
    zmanInfoLeftUp = new Area(xBegin, y_infoUp, widthLeft, h_info, BLACK);
    zmanInfoLeftDown = new Area(xBegin, y_infoDown, widthLeft, h_info, BLACK);

    uint8_t widthLeftS = 85;
    uint8_t widthMiddleS = 86;
    uint8_t widthRightS = 85;
    zmanRightUpS = new Area(xBegin+widthLeftS+widthMiddleS, y_infoUp, widthRightS, h_info, BLACK);
    zmanRightDownS = new Area(xBegin+widthLeftS+widthMiddleS, y_infoDown, widthRightS, h_info, BLACK);
    zmanMiddleUpS = new Area(xBegin+widthLeftS, y_infoUp, widthMiddleS, h_info, BLACK);
    zmanMiddleDownS = new Area(xBegin+widthLeftS, y_infoDown, widthMiddleS, h_info, BLACK);
    zmanLeftUpS = new Area(xBegin, y_infoUp, widthLeftS, h_info, BLACK);
    zmanLeftDownS = new Area(xBegin, y_infoDown, widthLeftS, h_info, BLACK);

    infoAll = new Area(xBegin, y_infoUp, widthAll, h_info*2, BLACK);
    infoUp = new Area(xBegin, y_infoUp, widthAll, h_info, BLACK);
    infoDown = new Area(xBegin, y_infoDown, widthAll, h_info, BLACK);
    infoCenter = new Area(xBegin, y_infoUp+h_info/2, widthAll, h_info, BLACK);

    uint8_t widthHalf = widthAll/2;
    infoUpHalfRight = new Area(xBegin+widthHalf, y_infoUp, widthHalf, h_info, BLACK);
    infoUpHalfLeft = new Area(xBegin, y_infoUp, widthHalf, h_info, BLACK);
    infoDownHalfRight = new Area(xBegin+widthHalf, y_infoDown, widthHalf, h_info, BLACK);
    infoDownHalfLeft = new Area(xBegin, y_infoDown, widthHalf, h_info, BLACK);

    uint8_t widthShort = 50;
    uint8_t widthHalfS = (widthAll-widthShort)/2;
    infoAllS = new Area(xBegin+widthShort, y_infoUp, widthAll-widthShort, h_info*2, BLACK);
    infoUpS = new Area(xBegin+widthShort, y_infoUp, widthAll-widthShort, h_info, BLACK);
    infoDownS = new Area(xBegin+widthShort, y_infoDown, widthAll-widthShort, h_info, BLACK);
    infoUpHalfRightS = new Area(xBegin+widthShort+widthHalfS, y_infoUp, widthHalfS, h_info, BLACK);
    infoUpHalfLeftS = new Area(xBegin+widthShort, y_infoUp, widthHalfS, h_info, BLACK);
    infoShortUpLeft = new Area(xBegin, y_infoUp, widthShort, h_info, BLACK);
    infoShortDownLeft = new Area(xBegin, y_infoDown, widthShort, h_info, BLACK);

}
void MainScreen::Big()
{
    // y = 22;
    // x0 = 25;
    // dw = 24;
    // nw = 59;
    // iluy
    uint8_t h_iluy = 18;
    iluyNishmat = new Area(0, 110, 255, h_iluy, BLACK);

    // date
    uint8_t h_date = 18;
    uint8_t w_yom = 70;
    uint8_t w_loazi = 66;
    uint8_t w_ivri = 256-w_loazi-w_yom;
    upAll = new Area(0, 0, 255, h_date, BLACK);
    yom = new Area(w_loazi+w_ivri, 0, w_yom, h_date, BLACK);
    ivriDate = new Area(w_loazi, 0, w_ivri, h_date, BLACK);
    loaziDate = new Area(0, 0, w_loazi, h_date, BLACK);

    // clock
    uint8_t digitWidth = pgm_read_byte(&clockFont->glyph['8' - clockFont->first].xAdvance);
    uint8_t nekudotaimWidth = pgm_read_byte(&clockFont->glyph[':' - clockFont->first].xAdvance);
    // Serial.print("nekudotaimWidth = ");
    // Serial.println(nekudotaimWidth);
    uint8_t digitPlace = digitWidth*2;
    uint8_t nekudotaimPlace = nekudotaimWidth + 24;
    // Serial.print("nekudotaimPlace = ");
    // Serial.println(nekudotaimPlace);
    uint8_t clkOffset = (256-(3*digitPlace+2*nekudotaimPlace))/2;
    uint8_t h_clock = 38;
    uint8_t clk_y = 18; 
    y = clk_y+4;
    // Serial.print("clkOffset: ");
    // Serial.println(clkOffset);
    x0 = clkOffset+4;
    dw = digitWidth;
    nw = digitWidth+nekudotaimPlace;
    // Serial.print("y: ");
    // Serial.println(y);
    // Serial.print("x0: ");
    // Serial.println(x0);
    // Serial.print("dw: ");
    // Serial.println(dw);
    // Serial.print("nw: ");
    // Serial.println(nw);
    clockAll = new Area(0, y-1, 255, h_clock+3, BLACK);
    hour = new Area(clkOffset, clk_y, digitPlace, h_clock, BLACK);
    nekudotaim1 = new Area(clkOffset+digitPlace, clk_y, nekudotaimPlace, h_clock, BLACK);
    minute = new Area(clkOffset+digitPlace+nekudotaimPlace, clk_y, digitPlace, h_clock, BLACK);
    nekudotaim2 = new Area(clkOffset+2*digitPlace+nekudotaimPlace, clk_y, nekudotaimPlace, h_clock, BLACK);
    second1 = new Area(clkOffset+digitPlace+nekudotaimPlace+digitPlace+nekudotaimPlace, clk_y, digitPlace/2, h_clock, BLACK);
    second2 = new Area(clkOffset+2*digitPlace+2*nekudotaimPlace+digitPlace/2, clk_y, digitPlace/2, h_clock, BLACK);

    // info
    uint8_t h_info = 18;
    uint8_t y_zmanUp = 56;
    uint8_t y_zmanDown = 74;
    uint8_t w_zman = 180;
    uint8_t w_parasha = 256-w_zman;
    uint8_t y_info = 92;

    middleTitleAll = new Area(0, y_zmanUp-1, 255, h_info+1, BLACK);
    middleValueAll = new Area(0, y_zmanDown-1, 255, h_info+1, BLACK);
    parashaTitle = new Area(w_zman, y_zmanUp, w_parasha, h_info, BLACK);
    parashaValue = new Area(w_zman, y_zmanDown, w_parasha, h_info, BLACK);
    parashaAll = new Area(w_zman, y_zmanUp, w_parasha, h_info*2, BLACK);
    
    zmanAll = new Area(0, y_zmanUp, w_zman, h_info*2, BLACK);
    zmanUp = new Area(0, y_zmanUp, w_zman, h_info, BLACK);
    zmanDown = new Area(0, y_zmanDown, w_zman, h_info, BLACK);
    
    infoAll = new Area(0, y_info-1, 256, h_info+1, BLACK);
    infoRight = new Area(128, y_info, 128, h_info, BLACK);
    infoLeft = new Area(0, y_info, 128, h_info, BLACK);

    uint8_t w_time = 60;
    zmanRightUp =    new Area(w_time*2, y_zmanUp, w_time, h_info, BLACK);
    zmanRightDown =  new Area(w_time*2, y_zmanDown, w_time, h_info, BLACK);
    zmanMiddle =     new Area(w_time, y_zmanUp+h_info/2, w_time, h_info, BLACK);
    zmanMiddleUp =   new Area(w_time, y_zmanUp, w_time, h_info, BLACK);
    zmanMiddleDown = new Area(w_time, y_zmanDown, w_time, h_info, BLACK);
    zmanLeftUp =     new Area(0, y_zmanUp, w_time, h_info, BLACK);
    zmanLeftDown =   new Area(0, y_zmanDown, w_time, h_info, BLACK);
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
uint8_t y_offset = 36;
uint8_t x_offset = 0;
bool dbg = false;
void MainScreen::updateHour(char* _hour)
{
    Text txt = Text(colorMiddle, clockFont, x_offset, y_offset);
    hour->clearArea();
    if(!dbg) hour->addText(_hour, txt);
    if(dbg) hour->addText("88", txt);
}
void MainScreen::updateMinute(char* _minute)
{
    Text txt = Text(colorMiddle, clockFont, x_offset, y_offset);
    minute->clearArea();
    if(!dbg) minute->addText(_minute, txt);
    if(dbg) minute->addText("88", txt);
}
void MainScreen::updateSecond1(char* _second1)
{   
    Text txt = Text(colorMiddle, clockFont, x_offset, y_offset);
    second1->clearArea();
    if(!dbg) second1->addText(_second1, txt);
    if(dbg) second1->addText("8", txt);
}
void MainScreen::updateSecond2(char* _second2)
{   
    Text txt = Text(colorMiddle, clockFont, 0, y_offset);
    second2->clearArea();
    if(!dbg) second2->addText(_second2, txt);
    if(dbg) second2->addText("8", txt);
}
void MainScreen::updateNekudotaim(char* _nekudotaim)
{   
    uint8_t x = (nekudotaim1->width-11)/2;
    // Serial.print("nekudotaim1->width: ");
    // Serial.println(nekudotaim1->width);
    // Serial.print("x: ");
    // Serial.println(x);
    Text txt = Text(colorMiddle, clockFont, x, y_offset, LEFT);
    nekudotaim1->clearArea();
    // Serial.print("calculateTextLength(_nekudotaim, txt) = ");
    // Serial.println(nekudotaim1->calculateTextLength(_nekudotaim, txt));
    nekudotaim1->addText(_nekudotaim, txt);
    nekudotaim2->clearArea();
    nekudotaim2->addText(_nekudotaim, txt);
}
void MainScreen::updateHebDate(const char* _ivriDateL, const char* _ivriDateS)
{
    Text txt = Text(colorIvri, &ariel10, 0, 13, CENTER);
    ivriDate->clearArea();
    if(!ivriDate->textFits(_ivriDateL, txt))
    {
        ivriDate->addText(_ivriDateS, txt);
    }
    else
    {
        ivriDate->addText(_ivriDateL, txt);
    }
}
void MainScreen::updateLoaziDate(const char* _loaziDate)
{
    Text txt = Text(colorLoazi, &ariel10, 0, 13, CENTER);
    loaziDate->clearArea();
    loaziDate->addText(_loaziDate, txt);
}
void MainScreen::updateIluyNishmat(String* _iluyNishmat)
{
    Text txt = Text(colorIluy, &ariel10, 0, 13, CENTER);
    if (!iluyNishmat->textFits(_iluyNishmat, txt)) { txt = Text(colorIluy, &font9, 0, 12, CENTER); }
    if (!iluyNishmat->textFits(_iluyNishmat, txt)) { txt = Text(colorIluy, &font7, 0, 12, CENTER); }
    if (!iluyNishmat->textFits(_iluyNishmat, txt)) { txt = Text(colorIluy, &font6, 0, 9, CENTER); }
    if (!iluyNishmat->textFits(_iluyNishmat, txt)) { txt = Text(colorIluy, &font5, 0, 9, CENTER); }
    iluyNishmat->clearArea();
    iluyNishmat->addText(_iluyNishmat, txt);
}
void MainScreen::updateInfo(const char* _info)
{
    Text txt = Text(colorUp, &ariel10, 0, 13, CENTER);
    if (!infoAll->textFits(_info, txt)) { txt = Text(colorUp, &font9, 0, 12, CENTER); }
    if (!infoAll->textFits(_info, txt)) { txt = Text(colorUp, &font7, 0, 12, CENTER); }
    if (!infoAll->textFits(_info, txt)) { txt = Text(colorUp, &font6, 0, 9, CENTER); }
    if (!infoAll->textFits(_info, txt)) { txt = Text(colorUp, &font5, 0, 9, CENTER); }
    infoAll->clearArea();
    infoAll->addText(_info, txt);
}
void MainScreen::updateInfo(const char* _info1, const char* _info2)
{
    char strAll[150];
    sprintf(strAll, "%s %s",_info1, _info2);
    updateInfo(strAll);
}

void MainScreen::updateParashaTitle(const char* _parashaTitle)
{
    Text txt = Text(colorParasha, &ariel10, 1, 13, CENTER);
    parashaTitle->clearArea();
    parashaTitle->addText(_parashaTitle, txt);
}
void MainScreen::updateParashaValue(const char* _parashaValue)
{
    Text txt = Text(colorParashaV, &ariel10, 1, 13, CENTER);
    if (!parashaValue->textFits(_parashaValue, txt))
    {
        txt.font = &font7;
    }
    parashaValue->clearArea();
    parashaValue->addText(_parashaValue, txt);
}
