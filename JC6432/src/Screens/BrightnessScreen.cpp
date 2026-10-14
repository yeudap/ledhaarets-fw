#include "Screens.h"


BrightnessScreen::BrightnessScreen()
{
    title = new Area(0, 0, 128, 12, BLACK);
    scale = new Area(0, 13, 128, 50, BLACK);
}
void BrightnessScreen::printScreen()
{
    display->clearScreen();
    title->clearArea();
    scale->clearArea();
    Text txt = Text(BLUE, &font7, 0, 7, CENTER);
    title->addText(strings.brightnessTitle, txt);
    title->addRect(true, 10,10,108,1, BLUE);
    scale->addRect(true, 9,40,108,2, WHITE);
}
void BrightnessScreen::setBrightness(int Brightness)
{
    uint16_t point = ((Brightness / 20) + 1) * 9;
    printScreen();
    scale->addRect(true, 9,40,point-9,2, 0x2834);
    scale->addCircle(true, point, 40, 6, WHITE);
    scale->addCircle(false, point, 40, 6, 0x2834);
    scale->addCircle(false, point, 40, 5, 0x2834);
}