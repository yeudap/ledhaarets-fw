#include "ClockRV3028.h"
#include <Wire.h>

bool ClockRV3028::begin()
{
    if (!Wire.begin(21, 22, 100000))
    {
        Serial.println("I2C init failed");
        return false;
    }
    return rtc.begin();
}

DateTime ClockRV3028::now()
{
    rtc.updateTime();

    return DateTime(
        2000 + rtc.getYear(),
        rtc.getMonth(),
        rtc.getDate(),
        rtc.getHours(),
        rtc.getMinutes(),
        rtc.getSeconds()
    );
}

void ClockRV3028::adjust(const DateTime& dt)
{
    rtc.setTime(
        dt.second(),
        dt.minute(),
        dt.hour(),
        dt.dayOfTheWeek(),
        dt.day(),
        dt.month(),
        dt.year() - 2000
    );
}
