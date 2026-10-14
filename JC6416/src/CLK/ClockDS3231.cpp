#include "ClockDS3231.h"

bool ClockDS3231::begin()
{
    return rtc.begin();
}

DateTime ClockDS3231::now()
{
    return rtc.now();
}

void ClockDS3231::adjust(const DateTime& dt)
{
    rtc.adjust(dt);
}
