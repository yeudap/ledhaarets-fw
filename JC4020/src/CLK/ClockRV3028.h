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
};

#endif
