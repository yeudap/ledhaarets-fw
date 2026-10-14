#include "Screens.h"

LogoScreen::LogoScreen()
{
    logoArea = new Area(30, 1, 63, 50, BLACK);
}

void LogoScreen::printScreen()
{
    display->clearScreen();
    logoArea->clearArea();

    Text txt = Text(BLUE, &logo, 0, 44);
    logoArea->addText("A", txt);
}
