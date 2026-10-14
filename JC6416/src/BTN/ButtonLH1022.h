#ifndef BUTTON_LH1022_H
#define BUTTON_LH1022_H

#include "ButtonDriver.h"
#include <Arduino.h>

class ButtonLH1022 : public IButtonDriver
{
public:
    ButtonLH1022();
    bool isPushed(ButtonCommand command) override;

private:
    static constexpr uint8_t MENU_PIN = 32;
    static constexpr uint8_t UP_PIN   = 33;
    static constexpr uint8_t DOWN_PIN = 35;
    static constexpr uint8_t OK_PIN   = 34;

    uint8_t pinForCommand(ButtonCommand command) const;
};

#endif
