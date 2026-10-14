#ifndef CLOCK_DRIVER_H
#define CLOCK_DRIVER_H

#include <RTClib.h>

class ClockDriver
{
public:
    virtual ~ClockDriver() = default;

    virtual bool begin() = 0;
    virtual DateTime now() = 0;
    virtual void adjust(const DateTime& dt) = 0;
};

#endif
