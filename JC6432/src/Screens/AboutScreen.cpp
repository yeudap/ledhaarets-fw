#include "Screens.h"
#include "Metadata.h"

AboutScreen::AboutScreen()
{
    logoArea = new Area(63, 1, 63, 50, BLACK);
    info = new Area(1, 1, 62, 28, BLACK);
    swVer = new Area(1, 32, 62, 10, BLACK);
    phone = new Area(1, 42, 62, 10, BLACK);
    url = new Area(1, 53, 126, 10, BLACK);

}

void AboutScreen::printScreen()
{
    display->clearScreen();
    logoArea->clearArea();
    swVer->clearArea();
    info->clearArea();
    phone->clearArea();
    url->clearArea();

    Text txt = Text(BLUE, &logo, 0, 44);
    logoArea->addText("A", txt);
    txt = Text(GREEN, &font6, 10, 7);
    info->addText(strings.info1, txt);
    txt = Text(GREEN, &font6, 0, 17);
    info->addText(strings.info2, txt);
    txt = Text(RED, nullptr, 0, 0);
    swVer->addText(Metadata::SWversion, txt);
    phone->addText(strings.phone, txt);
    url->addText(strings.url, txt);

}
