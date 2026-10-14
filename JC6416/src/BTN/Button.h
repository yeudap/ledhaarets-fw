#ifndef BUTTON_H
#define BUTTON_H

#include <Arduino.h>
#include <BLE/Commands.h>
#include "ButtonDriver.h"

const int long_push_button = 8;
const int delay_button_time = 250;

enum class ButtonDriverType
{
    LH1022,
    LH0826
};

class Button
{
public:
    explicit Button(ButtonCommand command);
    bool isPushed();

private:
    ButtonCommand bt_command;
};

void selectButtonDriver(ButtonDriverType type);

extern Button menuButton;
extern Button upButton;
extern Button downButton;
extern Button okButton;

#endif
