#include "Screens.h"

ColorScreen::ColorScreen()
{
    title = new Area(0, 0, 128, 12, BLACK);
    colors = new Area(0, 13, 128, 50, BLACK);
}

void ColorScreen::printScreen()
{
    display->clearScreen();
    title->clearArea();
    colors->clearArea();
    Text txt = Text(BLUE, &font7, 0, 7, CENTER);
    title->addText(strings.colorsTitle, txt);
    title->addRect(true, 10,10,108,1, BLUE);
    uint16_t r = 7;
    uint16_t y = 40;
    uint16_t x = 13;
    for(uint8_t i = 0; i<7; i++)
    {
        colors->addCircle(true, x+17*i, y, r, color_list[i]);
    }
}
void ColorScreen::setColor(uint16_t colorIdx)
{
    this->printScreen();
    uint16_t r = 7;
    uint16_t y = 40;
    uint16_t x = 13;
    colors->addCircle(false, x+17*colorIdx, y, r-1, BLACK);
    colors->addCircle(false, x+17*colorIdx, y, r-2, BLACK);
}