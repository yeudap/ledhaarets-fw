#include "ButtonLH0826.h"

ButtonLH0826::ButtonLH0826()
{
    analogReadResolution(12);
    analogSetPinAttenuation(ADC_PIN, ADC_11db);
}

ButtonCommand ButtonLH0826::readAdcButton()
{
    uint32_t sum = 0;
    constexpr uint8_t samples = 4;

    for (uint8_t i = 0; i < samples; ++i)
        sum += analogReadMilliVolts(ADC_PIN);

    uint16_t mv = sum / samples;

    if (mv < NONE_UP_THRESHOLD)
        return ButtonCommand::NONE;

    if (mv < UP_MENU_THRESHOLD)
        return ButtonCommand::UP;

    if (mv < MENU_DOWN_THRESHOLD)
        return ButtonCommand::MENU;

    if (mv < DOWN_OK_THRESHOLD)
        return ButtonCommand::DOWN;

    return ButtonCommand::OK;
}

bool ButtonLH0826::isPushed(ButtonCommand command)
{
    return readAdcButton() == command;
}
