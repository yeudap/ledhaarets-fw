#ifndef BUTTON_LH0826_H
#define BUTTON_LH0826_H

#include "ButtonDriver.h"
#include <Arduino.h>

class ButtonLH0826 : public IButtonDriver
{
public:
    ButtonLH0826();
    bool isPushed(ButtonCommand command) override;

private:
    static constexpr uint8_t ADC_PIN = 36;

    static constexpr uint16_t NONE_UP_THRESHOLD   = 386;
    static constexpr uint16_t UP_MENU_THRESHOLD   = 1212;
    static constexpr uint16_t MENU_DOWN_THRESHOLD = 2070;
    static constexpr uint16_t DOWN_OK_THRESHOLD   = 2746;

    ButtonCommand readAdcButton();
};

#endif
