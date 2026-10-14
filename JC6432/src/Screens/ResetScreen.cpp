#include "Screens.h"

ResetScreen::ResetScreen()
{
    title = new Area(0, 0, 128, 10, BLACK);
    question = new Area(20, 15, 90, 20, BLACK);
    yes = new Area(75, 40, 18, 14, BLACK);
    no = new Area(30, 40, 18, 14, BLACK);
}

void ResetScreen::printScreen()
{
    display->clearScreen();
    question->clearArea();
    yes->clearArea();
    no->clearArea();


}
void ResetScreen::SetScreen(const char * text)
{
    printScreen();
    Text txt = Text(BLUE, &font7, 0, 7, CENTER);
    title->addText(strings.resetTitle, txt);
    title->addRect(true, 10,10,108,1, BLUE);

    txt = Text(GREEN, &font7, 2, 10);
    question->addText(text, txt);
    no->addText(strings.no, txt);
    txt = Text(GREEN, &font7, 5, 9);
    yes->addText(strings.yes, txt);
}
void ResetScreen::focusArea(bool reset)
{
    if (reset)
    {
        yes->addRoundRect(false, yes->x-1, yes->y-1, yes->width+2, yes->high+2, 3, RED);
    }
    else
    {
        no->addRoundRect(false, no->x-1, no->y-1, no->width+2, no->high+2, 3, RED);
    }
}