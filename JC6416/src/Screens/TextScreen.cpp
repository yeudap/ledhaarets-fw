#include "Screens.h"


TextScreen::TextScreen()
{
    title = new Area(0, 0, 256, 10, BLACK);
    index1 = new Area(192, 15, 15, 15, BLACK);
    index2 = new Area(192, 30, 15, 15, BLACK);
    index3 = new Area(192, 45, 15, 15, BLACK);
    text1 = new Area(54, 15, 135, 15, BLACK);
    text2 = new Area(54, 30, 135, 15, BLACK);
    text3 = new Area(54, 45, 135, 15, BLACK);
    delete1 = new Area(31, 30, 21, 15, BLACK);
}
void TextScreen::printScreen()
{
    display->clearScreen();
    title->clearArea();
    index1->clearArea();
    index2->clearArea();
    index3->clearArea();
    text1->clearArea();
    text2->clearArea();
    text3->clearArea();
    delete1->clearArea();
    printTitle();
}
void TextScreen::printDelete()
{
    Text txt = Text(WHITE, &font6, 2, 9, CENTER);
    delete1->addText(strings.mchak, txt); 
}

void TextScreen::printTitle()
{
    Text txt = Text(BLUE, &font7, 0, 7, CENTER);
    title->addText(strings.TextsTitle, txt);
    title->addRect(true, 10,10,236,1, BLUE);
}
