#ifndef HARDWARE_H
#define HARDWARE_H

#include "BTN/Button.h"
#include "BLE/Bluetooth.h"
#include "CLK/Clock.h"

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

enum class HardwareModel
{
    LH1022,
    LH0826
};

constexpr HardwareModel HARDWARE_MODEL = HardwareModel::LH1022;

inline void configureHardware(HardwareModel model)
{
    switch (model)
    {
        case HardwareModel::LH1022:
            selectClock(ClockType::DS3231);
            selectButtonDriver(ButtonDriverType::LH1022);
            break;

        case HardwareModel::LH0826:
            selectClock(ClockType::RV3028);
            selectButtonDriver(ButtonDriverType::LH0826);
            break;
    }
}

#endif