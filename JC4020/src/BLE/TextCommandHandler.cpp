#include "TextCommandHandler.h"
#include "../Preferences/UserPreferences.h"
#include "States/States.h"
#include <Logic.h>

extern UserPreferences userPreferences;
extern Logic bl;

// פונקציה פרטית לבדיקת תקינות אינדקס
bool isValidIndex(uint8_t index)
{
    if (index >= numOfNameSets)
    {
        Serial.print("Invalid text index: ");
        Serial.println(index);
        return false;
    }
    return true;
}

// פונקציה פרטית לקבלת טקסט לפי אינדקס
String getTextByIndex(uint8_t index)
{
    if (!isValidIndex(index))
    {
        return "";
    }

    UserPref<String> *textPref = (UserPref<String> *)userPreferences.textPrefs[index];
    return textPref->getValue();
}

// פונקציה פרטית לבדיקה אם הטקסט ריק
bool isTextEmpty(uint8_t index)
{
    return getTextByIndex(index).length() == 0;
}

// קבלת טקסט לפי אינדקס
CommandResult handleReadText(uint8_t index)
{
    // בדיקת תקינות האינדקס
    if (!isValidIndex(index))
    {
        return CommandResult::error(ERR_INVALID_INDEX);
    }

    String text = getTextByIndex(index);

    // בדיקה אם הטקסט ריק
    if (text.length() == 0)
    {
        // אפשר להחזיר שגיאה או טקסט ריק
        // במקרה זה נחזיר הצלחה עם טקסט ריק
        return CommandResult::okWithText("");
    }

    CommandResult result = CommandResult::okWithText(text);
    return result;
}

// כתיבת טקסט לפי אינדקס
CommandResult handleWriteText(uint8_t index, const String &text)
{
//     if (!isValidIndex(index))
//     {
        return CommandResult::error(ERR_INVALID_INDEX);
    // }

    // בדיקת תקינות הטקסט
    // String textCopy = text;
    // if (!(bl.setTohenIluyStateTextList.validateTextLen(&textCopy)))
    // {
    //     return CommandResult::error(ERR_INVALID_FORMAT);
    // }

    // try
    // {
    //     UserPref<String> *textPref = (UserPref<String> *)userPreferences.textPrefs[index];
    //     *textPref = text;
    //     bl.mainState.numOfValidTexts = handleGetTextCount().data1;
    //     // הפעלת הפונקציה להצגת טקסט
    //     bl.mainState.stateIndexNameSet = index;
    //     bl.mainState.flagNameSetChanged = false;
    //     return CommandResult::ok(index);
    // }
    // catch (std::exception &e)
    // {
    //     Serial.print("Error writing text: ");
    //     Serial.println(e.what());
    //     return CommandResult::error(ERR_WRITE_FAILED);
    // }
}

// מחיקת טקסט לפי אינדקס וסגירת "חורים"
CommandResult handleDeleteText(uint8_t index)
{
    if (!isValidIndex(index))
    {
        return CommandResult::error(ERR_INVALID_INDEX);
    }

    try
    {
        // עוברים על כל האינדקסים החל מזה שנמחק
        for (int i = index; i < numOfNameSets - 1; i++)
        {
            UserPref<String> *currentPref = (UserPref<String> *)userPreferences.textPrefs[i];
            UserPref<String> *nextPref = (UserPref<String> *)userPreferences.textPrefs[i + 1];

            String nextText = nextPref->getValue();

            // אם אין טקסט באינדקס הבא, יוצאים מהלולאה
            if (nextText.isEmpty())
            {
                *currentPref = ""; // מוודאים שהטקסט הנוכחי נמחק
                break;
            }

            // העברת הטקסט הבא לאינדקס הנוכחי
            *currentPref = nextText;

            // מחיקת הטקסט באינדקס הבא (יכתב בסיבוב הבא או יישאר ריק אם זה האחרון)
            *nextPref = "";
        }
        // bl.mainState.numOfValidTexts = handleGetTextCount().data1;
        return CommandResult::ok(index);
    }
    catch (std::exception &e)
    {
        Serial.print("Error deleting text: ");
        Serial.println(e.what());
        return CommandResult::error(ERR_DELETE_FAILED);
    }
}

// הצגת טקסט על המסך
CommandResult handleDisplayText(uint8_t index)
{
    // if (!isValidIndex(index))
    {
        return CommandResult::error(ERR_INVALID_INDEX);
    }

    // String text = getTextByIndex(index);
    // if (text.length() == 0)
    // {
    //     return CommandResult::error(ERR_EMPTY_TEXT);
    // }

    try
    {
        // הפעלת הפונקציה להצגת טקסט
        // bl.mainState.stateIndexNameSet = index;
        // bl.mainState.flagNameSetChanged = false;
        // return CommandResult::ok(index);
    }
    catch (std::exception &e)
    {
        Serial.print("Error displaying text: ");
        Serial.println(e.what());
        return CommandResult::error(ERR_UNSUPPORTED);
    }
}

// החלפת מיקום בין שני טקסטים
CommandResult handleSwapTexts(uint8_t index1, uint8_t index2)
{
    if (!isValidIndex(index1) || !isValidIndex(index2))
    {
        return CommandResult::error(ERR_INVALID_INDEX);
    }

    try
    {
        UserPref<String> *textPref1 = (UserPref<String> *)userPreferences.textPrefs[index1];
        UserPref<String> *textPref2 = (UserPref<String> *)userPreferences.textPrefs[index2];

        String temp = textPref1->getValue();
        *textPref1 = textPref2->getValue();
        *textPref2 = temp;
        return CommandResult::okWithData(index1, index2);
    }
    catch (std::exception &e)
    {
        Serial.print("Error swapping texts: ");
        Serial.println(e.what());
        return CommandResult::error(ERR_MEMORY);
    }
}

// בקשת מספר הטקסטים הלא ריקים
CommandResult handleGetTextCount()
{
    uint8_t count = 0;

    try
    {
        for (uint8_t i = 0; i < numOfNameSets; i++)
        {
            UserPref<String> *textPref = (UserPref<String> *)userPreferences.textPrefs[i];
            if (textPref->getValue().length() > 0)
            {
                count++;
            }
        }

        return CommandResult::ok(count);
    }
    catch (std::exception &e)
    {
        Serial.print("Error counting texts: ");
        Serial.println(e.what());
        return CommandResult::error(ERR_READ_FAILED);
    }
}