#include "ButtonLH1022.h"

ButtonLH1022::ButtonLH1022()
{
    pinMode(MENU_PIN, INPUT);
    pinMode(UP_PIN, INPUT);
    pinMode(DOWN_PIN, INPUT);
    pinMode(OK_PIN, INPUT);
}

uint8_t ButtonLH1022::pinForCommand(ButtonCommand command) const
{
    switch (command)
    {
        case ButtonCommand::MENU: return MENU_PIN;
        case ButtonCommand::UP:   return UP_PIN;
        case ButtonCommand::DOWN: return DOWN_PIN;
        case ButtonCommand::OK:   return OK_PIN;
        default:                  return 0xFF;
    }
}

bool ButtonLH1022::isPushed(ButtonCommand command)
{
    uint8_t pin = pinForCommand(command);

    if (pin == 0xFF)
        return false;

    return digitalRead(pin) != LOW;
}
