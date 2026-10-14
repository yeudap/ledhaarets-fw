#include "Screens.h"


SetClockScreen::SetClockScreen()
{
    title = new Area(0, 0, 128, 10, BLACK);
    hour = new Area(4, 16, 35, 23, BLACK);
    minute = new Area(39, 16, 51, 23, BLACK);
    second = new Area(90, 16, 35, 23, BLACK);
    day = new Area(35, 50, 13, 9, BLACK);
    month = new Area(53, 50, 13, 9, BLACK);
    year = new Area(71, 50, 23, 9, BLACK);
    point = new Area(85, 16, 6, 23, BLACK);
}
void SetClockScreen::printScreen()
{
    display->clearScreen();
    Text txt = Text(BLUE, &font7, 0, 7, CENTER);
    title->addText(strings.kivunShaon, txt);
    title->addRect(true, 10,10,108,1, BLUE);
}
void SetClockScreen::updateHour(char* _hour)
{
    Text txt = Text(WHITE, &number19, 1, 20, CENTER);
    hour->clearArea();
    hour->addText(_hour, txt);
}
void SetClockScreen::updateMinute(char* _minute)
{
    Text txt = Text(WHITE, &number19, 1, 20, CENTER);
    minute->clearArea();
    minute->addText(_minute, txt);
}
void SetClockScreen::updateSecond(char* _second)
{   
    Text txt = Text(WHITE, &number19, 1, 20);
    second->clearArea();
    second->addText(_second, txt);
}
void SetClockScreen::updateDay(char* _day)
{
    Text txt = Text(WHITE, &font7, 1, 7);
    day->clearArea();
    day->addText(_day, txt);
}
void SetClockScreen::updateMonth(char* _month)
{
    Text txt = Text(WHITE, &font7, 1, 7);
    month->clearArea();
    month->addText(_month, txt);
}
void SetClockScreen::updateYear(char* _year)
{   
    Text txt = Text(WHITE, &font7, 1, 7);
    year->clearArea();
    year->addText(_year, txt);
}
void SetClockScreen::focusArea(Area* area)
{
    uint16_t center = area->x + area->width/2;
    area->addTriangle(true, center-2, area->y-1, center, area->y-3, center+2, area->y-1 , YELLOW);
    area->addTriangle(true, center-2, area->high+area->y+1, center, area->high+area->y+3, center+2, area->high+area->y+1 , YELLOW);
}
void SetClockScreen::printPoint()
{
    Text txt = Text(WHITE, &number19, 1, 20, CENTER);
    point->addText(".", txt);
}