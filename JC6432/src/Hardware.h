#ifndef HARDWARE_H
#define HARDWARE_H

#include "Metadata.h"
#include "BTN/Button.h"
#include "BLE/Bluetooth.h"
#include "CLK/Clock.h"

enum class HardwareModel
{
    LH1022,
    LH0826
};
inline HardwareModel hardwareModel;

static void dumpMem(const char* tag)
{
    Serial.printf(
        "[%s] FreeDMA=%u LargestDMA=%u Free=%u Largest=%u\n",
        tag,
        heap_caps_get_free_size(MALLOC_CAP_DMA),
        heap_caps_get_largest_free_block(MALLOC_CAP_DMA),
        ESP.getFreeHeap(),
        heap_caps_get_largest_free_block(MALLOC_CAP_8BIT)
    );
}



inline HardwareModel detectHardwareModel()
{
    constexpr int HW_ID_PIN = 35;

    analogSetPinAttenuation(HW_ID_PIN, ADC_11db);

    uint32_t sum = 0;

    for (int i = 0; i < 16; i++)
    {
        sum += analogReadMilliVolts(HW_ID_PIN);
        delay(2);
    }

    uint32_t mv = sum / 16;

    Serial.printf("HW_ID: %lu mV\n", mv);

    // LH0826: voltage divider gives ~1.7V
    if (mv > 1200 && mv < 2200)
        return HardwareModel::LH0826;

    // LH1022: GPIO35 is button input:
    // ~0V normally, ~3.3V when pressed
    return HardwareModel::LH1022;
}

inline void configureHardware(HardwareModel model)
{
    switch (model)
    {
        case HardwareModel::LH1022:
            Metadata::setHardwareVersion(Metadata::hwLH1022);
            selectClock(ClockType::DS3231);
            selectButtonDriver(ButtonDriverType::LH1022);
            break;

        case HardwareModel::LH0826:
            Metadata::setHardwareVersion(Metadata::hwLH0826);
            selectClock(ClockType::RV3028);
            selectButtonDriver(ButtonDriverType::LH0826);
            break;
    }
}

#endif