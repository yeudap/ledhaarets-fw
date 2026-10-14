#include "Screens.h"
#include "Metadata.h"

void getDate(char* buffer)
{
    char month[4];
    sprintf(month, "%c%c%c", __DATE__[0], __DATE__[1], __DATE__[2]);
    int monthInt = 0;
         if (month[0] == 'J' && month[1] == 'a' && month[2] == 'n') monthInt = 1;
    else if (month[0] == 'F' && month[1] == 'e' && month[2] == 'b') monthInt = 2;
    else if (month[0] == 'M' && month[1] == 'a' && month[2] == 'r') monthInt = 3;
    else if (month[0] == 'A' && month[1] == 'p' && month[2] == 'r') monthInt = 4;
    else if (month[0] == 'M' && month[1] == 'a' && month[2] == 'y') monthInt = 5;
    else if (month[0] == 'J' && month[1] == 'u' && month[2] == 'n') monthInt = 6;
    else if (month[0] == 'J' && month[1] == 'u' && month[2] == 'l') monthInt = 7;
    else if (month[0] == 'A' && month[1] == 'u' && month[2] == 'g') monthInt = 8;
    else if (month[0] == 'S' && month[1] == 'e' && month[2] == 'p') monthInt = 9;
    else if (month[0] == 'O' && month[1] == 'c' && month[2] == 't') monthInt = 10;
    else if (month[0] == 'N' && month[1] == 'o' && month[2] == 'v') monthInt = 11;
    else if (month[0] == 'D' && month[1] == 'e' && month[2] == 'c') monthInt = 12;
    char monthStr[3];
    sprintf(monthStr, "%02d", monthInt);

    char yearStr[5];
    sprintf(yearStr, "%c%c%c%c", __DATE__[7], __DATE__[8], __DATE__[9], __DATE__[10]);

    char day0 = __DATE__[4];
    if (day0 == ' ') day0 = '0';
    char dayStr[3];
    sprintf(dayStr, "%c%c", day0, __DATE__[5]);
    
    char timeStr[6];
    sprintf(timeStr, "%c%c:%c%c", __TIME__[0], __TIME__[1], __TIME__[3], __TIME__[4]);

    sprintf(buffer, "%s/%s/%s %s", dayStr, monthStr, yearStr, timeStr);
}

VersionScreen::VersionScreen()
{
    title = new Area(0, 0, 128, 10, BLACK);

    modelTitle =  new Area(74, 14, 53, 10, BLACK);
    swTitle =     new Area(74, 24, 53, 10, BLACK);
    hwTitle =     new Area(74, 34, 53, 10, BLACK);
    serialTitle = new Area(74, 44, 53, 10, BLACK);
    updateTitle = new Area(74, 54, 53, 10, BLACK);

    modelValue =  new Area(1, 14, 73, 10, BLACK);
    swValue =     new Area(1, 24, 73, 10, BLACK);
    hwValue =     new Area(1, 34, 73, 10, BLACK);
    serialValue = new Area(1, 44, 73, 10, BLACK);
    updateValue = new Area(1, 54, 73, 10, BLACK);
}

void VersionScreen::printScreen()
{
    display->clearScreen();
    title->clearArea();
    modelTitle->clearArea();
    swTitle->clearArea();
    hwTitle->clearArea();
    serialTitle->clearArea();
    updateTitle->clearArea();
    modelValue->clearArea();
    swValue->clearArea();
    hwValue->clearArea();
    serialValue->clearArea();
    updateValue->clearArea();

    printValues();
}
void VersionScreen::printValues()
{
    Text txt = Text(BLUE, &font7, 0, 7, CENTER);
    title->addText(strings.odotHamozr, txt);
    title->addRect(true, 10,10,108,1, BLUE);

    txt = Text(GREEN, &font6, 0, 7, RIGHT);
    modelTitle->addText(strings.model, txt);
    swTitle->addText(strings.swVersion, txt);
    hwTitle->addText(strings.hwVersion, txt);
    serialTitle->addText(strings.serial, txt);
    updateTitle->addText(strings.lastUpdate, txt);

    txt = Text(RED, nullptr, 1, 1, LEFT);
    modelValue->addText(Metadata::model, txt);
    swValue->addText(Metadata::SWversion, txt);
    hwValue->addText(Metadata::HWversion, txt);
    uint8_t mac_base[6] = {0};
    esp_efuse_mac_get_default(mac_base);
    char mac[12];
    sprintf(mac, "%02X%02X%02X", mac_base[3],mac_base[4],mac_base[5]);
    serialValue->addText(mac, txt);
    char date[20];
    getDate(date);
    txt = Text(RED, &font7, 1, 7, LEFT);
    updateValue->addText(date, txt);
}
