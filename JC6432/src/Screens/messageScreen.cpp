#include "Screens.h"


MessageScreen::MessageScreen()
{
    icon = new Area(85, 5, 38, 25, BLACK);
    message1 = new Area(1, 5, 80, 25, BLACK);
    message2 = new Area(1, 35, 127, 19, BLACK);
    anima = new Area(1, 54, 127, 8, BLACK);
}
void MessageScreen::printScreen()
{
    display->clearScreen();
    icon->clearArea();
    message1->clearArea();
    message2->clearArea();
    anima->clearArea();
}
void MessageScreen::printMessages(MessageType Mtype, const char* _message1, const char* _message2, uint16_t _delay)
{
    printScreen();
    Text txtIcon = Text(Mtype, &logo, 0,20, LEFT);
    icon->addText("V", txtIcon);
    Text txt = Text(Mtype, &font7, 0,15, RIGHT);
    message1->addText(_message1, txt);
    txt = Text(Mtype, &font6, 0,10, CENTER);
    message2->addText(_message2, txt);
    animateTime(Mtype, _delay);
}
void MessageScreen::printProgressBar(MessageType Mtype, const char* _message1, const char* _message2, uint8_t percent)
{
    printScreen();
    Text txtIcon = Text(Mtype, &logo, 0,20, LEFT);
    icon->addText("V", txtIcon);
    Text txt = Text(Mtype, &font7, 0,15, CENTER);
    message1->addText(_message1, txt);
    message2->addText(_message2, txt);
    animatePrercent(Mtype, percent);
}
void MessageScreen::animateTime(int color, uint16_t _delay)
{
    anima->addRoundRect(false, 1,55,127,6, 2, color);
    uint16_t point;
    uint16_t newPoint=0;
    uint16_t pointTime=_delay/125; 
    for(int i = 1; i < _delay; ++i)
    {
        point = i/pointTime;
        if(point != newPoint)
        {
            anima->addRect(true,point+1,57,1,2,WHITE);
            newPoint=point;
        }
        delay(1);
    }
}
void MessageScreen::animatePrercent(int color, uint8_t _percent)
{

    anima->addRoundRect(false, 1,55,127,6, 2, color);
    uint8_t current = _percent * 1.25; // one percent width is 125/100
    anima->addRect(true,1,57,current+1,2,WHITE);
}