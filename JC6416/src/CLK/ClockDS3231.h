#ifndef CLOCK_DS3231_H
#define CLOCK_DS3231_H

#include "ClockDriver.h"
#include <RTClib.h>

class ClockDS3231 : public ClockDriver
{
public:
    bool begin() override;
    DateTime now() override;
    void adjust(const DateTime& dt) override;

private:
    RTC_DS3231 rtc;
};

#endif
