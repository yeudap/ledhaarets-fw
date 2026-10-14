#ifndef CLOCK_RV3028_H
#define CLOCK_RV3028_H

#include "ClockDriver.h"
#include <RV-3028-C7.h>

class ClockRV3028 : public ClockDriver
{
public:
    bool begin() override;
    DateTime now() override;
    void adjust(const DateTime& dt) override;

private:
    RV3028 rtc;

    bool ensureBackupConfiguration();

    bool writeRegister(uint8_t reg, uint8_t value);
    bool readRegister(uint8_t reg, uint8_t &value);

    bool waitEEPROM(uint32_t timeoutMs = 500);

    bool writeEEPROMByte(uint8_t address, uint8_t value);
};

#endif
