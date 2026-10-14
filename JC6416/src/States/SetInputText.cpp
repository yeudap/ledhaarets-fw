#include "States.h"
#include "Logic.h"
#include "../BLE/TextCommandHandler.h"

extern UserPreferences userPreferences;
extern Logic bl;

// main
void TohenIluyState::showStateScreen()
{
    bl.mainState.showStateScreen();
    mainScreen.iluyNishmat->addRoundRectBorder();
}
void TohenIluyState::stateLoop()
{
    bl.mainState.stateLoop();
    mainScreen.iluyNishmat->addRoundRectBorder();
}

// kezev
void kezevStrIluy(char *buffer, uint16_t dakot)
{
    sprintf(buffer, "%s %4d %s", strings.shniot, dakot, strings.kezevHahlafa);
}
void SetTohenIluyStateKezev::showStateScreen()
{
    char kezev[50];
    kezevStrIluy(kezev, userPreferences.intervalIluy);
    menuScreen.printMenu(strings.hakdasha, strings.aroch, kezev, strings.empty);
};
void SetTohenIluyStateKezevSet::showStateScreen()
{
    char kezev[50];
    kezevStrIluy(kezev, this->kezevTemp);
    menuScreen.printMenu(strings.hakdasha, strings.aroch, kezev, strings.empty);
    menuScreen.kezev->addRoundRect(false, menuScreen.kezev->x, menuScreen.kezev->y + 1, menuScreen.kezev->width, menuScreen.kezev->high - 1, 3, YELLOW);
    menuScreen.kezev->addArrows();
}
void SetTohenIluyStateKezevSet::menuAction()
{
    this->kezevTemp = userPreferences.intervalIluy;
}
void SetTohenIluyStateKezevSet::upAction()
{
    this->kezevTemp += 5;
    if (this->kezevTemp == 605)
    {
        this->kezevTemp = 5;
    }
}
void SetTohenIluyStateKezevSet::downAction()
{
    this->kezevTemp -= 5;
    if (this->kezevTemp == 0)
    {
        this->kezevTemp = 600;
    }
}
void SetTohenIluyStateKezevSet::okAction()
{
    userPreferences.intervalIluy = this->kezevTemp;
    bl.configStates();
}

// SetTohenIluyStateTextList
void SetTohenIluyStateText::showStateScreen()
{
    char kezev[50];
    kezevStrIluy(kezev, userPreferences.intervalIluy);
    menuScreen.printMenu(strings.hakdasha, kezev, strings.aroch, strings.empty);
}

void updateText(String text, int index = 0, uint8_t line = 2)
{
    Serial.print("updateText: ");
    Serial.println(text);
    Text txt = Text(WHITE, &font6, 1, 10, CENTER);
    Text idx = Text(YELLOW, &font7, 1, 10, CENTER);
    if (line == 1)
    {
        textScreen.text1->clearArea();
        textScreen.text1->addText(&text, txt);
        if (index != 0)
        {
            textScreen.index1->clearArea();
            textScreen.index1->addText(index, idx);
        }
    }
    if (line == 2)
    {
        textScreen.text2->clearArea();
        textScreen.text2->addText(&text, txt);
        if (index != 0)
        {
            textScreen.index2->clearArea();
            textScreen.index2->addText(index, idx);
        }
    }
    if (line == 3)
    {
        textScreen.text3->clearArea();
        textScreen.text3->addText(&text, txt);
        if (index != 0)
        {
            textScreen.index3->clearArea();
            textScreen.index3->addText(index, idx);
        }
    }
}

void SetTohenIluyStateTextList::showStateScreen()
{
    Serial.println("TextList");
    Serial.print("indexText: ");
    Serial.println(bl.mainState.indexText);
    Serial.print("numOfValidTexts: ");
    Serial.println(bl.mainState.numOfValidTexts);

    textScreen.printScreen();
    if (bl.mainState.numOfValidTexts == 0) // אין טקסטים
    {
        updateText("!vdmvk ohbuwb iht");
        return;
    }
    else if (bl.mainState.indexText == 0) // טקסט ראשון - שורה 1 ריקה
    {
        updateText(getTextByIndex(bl.mainState.indexText + 1), bl.mainState.indexText + 2, 3);
    }
    else if (bl.mainState.indexText == bl.mainState.numOfValidTexts - 1) // טקסט אחרון - שורה 3 ריקה
    {
        updateText(getTextByIndex(bl.mainState.indexText - 1), bl.mainState.indexText, 1);
    }
    // else if (bl.mainState.indexText == numOfNameSets - 1) // טקסט מקסימלי
    // {
    //     updateText(getTextByIndex(bl.mainState.indexText - 1), bl.mainState.indexText, 1);
    // }
    else // טקסט אמצעי
    {
        updateText(getTextByIndex(bl.mainState.indexText - 1), bl.mainState.indexText, 1);
        updateText(getTextByIndex(bl.mainState.indexText + 1), bl.mainState.indexText + 2, 3);
    }
    updateText(getTextByIndex(bl.mainState.indexText), bl.mainState.indexText + 1, 2); // טקסט נוכחי

    textScreen.text2->addRoundRectBorder();
    textScreen.printDelete();
}
void SetTohenIluyStateTextList::menuAction()
{
    bl.mainState.indexText = 0;
}
void SetTohenIluyStateTextList::upAction()
{
    if (bl.mainState.indexText > 0)
    {
        bl.mainState.indexText -= 1;
    }
}
void SetTohenIluyStateTextList::downAction()
{
    if (bl.mainState.indexText < (bl.mainState.numOfValidTexts) - 1)
    {
        bl.mainState.indexText += 1;
    }
}

// SetTohenIluyStateDelete
void removeTextByIndex(int index)
{
    handleDeleteText(index);
    if (bl.mainState.indexText > 0)
    {
        bl.mainState.indexText -= 1;
    }
}

void SetTohenIluyStateDelete::showStateScreen()
{
    textScreen.text2->addRoundRectBorder(textScreen.text2->background);
    textScreen.delete1->addRoundRectBorder();
}
void SetTohenIluyStateDelete::okAction()
{
    removeTextByIndex(bl.mainState.indexText);
    bl.configStates();
}

// Global
bool SetTohenIluyStateTextList::validateTextLen(String *text)
{
    Text t = Text(RED, &font6, 1, 10, CENTER);
    Serial.println(text->c_str());
    return (mainScreen.iluyNishmat->textFits(text, t));
}
