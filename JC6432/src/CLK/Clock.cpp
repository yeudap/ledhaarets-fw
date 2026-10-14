#include "Clock.h"

#include "ClockDriver.h"
#include "ClockDS3231.h"
#include "ClockRV3028.h"

static ClockDS3231 ds3231Clock;
static ClockRV3028 rv3028Clock;
static ClockDriver* activeClock = nullptr;

DateTime now;

int currentHour = -1;
int currentMinute = -1;
int currentSecond = -1;
int currentSecond1 = -1;
int currentSecond2 = -1;
int currentDay = -1;
int currentMonth = -1;
int currentYear = -1;


// --------------------------------------------------
// Select physical RTC
// --------------------------------------------------

void selectClock(ClockType type)
{
    switch (type)
    {
        case ClockType::DS3231:
            activeClock = &ds3231Clock;
            break;

        case ClockType::RV3028:
            activeClock = &rv3028Clock;
            break;
    }
}


// --------------------------------------------------
// Read active RTC and update global DateTime "now"
// --------------------------------------------------

void updateClockTime()
{
    if (activeClock == nullptr)
    {
        return;
    }

    now = activeClock->now();
}


// --------------------------------------------------
// Set active RTC and refresh global DateTime "now"
// --------------------------------------------------

void setClockTime(const DateTime& dt)
{
    if (activeClock == nullptr)
    {
        return;
    }

    activeClock->adjust(dt);
    updateClockTime();
}


// --------------------------------------------------
// RTC validation
// --------------------------------------------------

void validate_rtc()
{
    if (activeClock == nullptr)
    {
        Serial.println("RTC driver was not selected");
        while (1);
    }

    if (!activeClock->begin())
    {
        Serial.println("Couldn't find RTC");
        while (1);
    }
}


// --------------------------------------------------
// Time validation
// --------------------------------------------------

void validate_time()
{
    updateClockTime();

    if (now < MIN_DATE || now > MAX_DATE)
    {
        setClockTime(DateTime(2026, 9, 1, 0, 0, 1));
    }
}


// --------------------------------------------------
// Synchronize RTC to compiler build time
// --------------------------------------------------

void syncClock()
{
    setClockTime(DateTime(__DATE__, __TIME__));
}


// --------------------------------------------------
// Current day index
// --------------------------------------------------

int getCurrentDayIndex()
{
    DateTime idx0 = DateTime(
        idx0Year,
        idx0mounth,
        idx0day,
        now.hour(),
        now.minute(),
        now.second()
    );

    TimeSpan diff = now - idx0;

    return diff.days();
}


// --------------------------------------------------
// Zman <-> DateTime
// --------------------------------------------------

DateTime zmanToDateTime(Zman zman)
{
    return DateTime(
        now.year(),
        now.month(),
        now.day(),
        zman.shaot,
        zman.dakot,
        zman.shniot
    );
}


Zman dateTimeToZman(DateTime dateTime)
{
    return Zman(
        dateTime.hour(),
        dateTime.minute()
    );
}


DateTime zmanPlusDakot(Zman zman, uint16_t plusDakot)
{
    return DateTime(
        now.year(),
        now.month(),
        now.day(),
        zman.shaot,
        zman.dakot,
        now.second()
    ) + TimeSpan(plusDakot * 60);
}


DateTime zmanMinusDakot(Zman zman, uint16_t minusDakot)
{
    return DateTime(
        now.year(),
        now.month(),
        now.day(),
        zman.shaot,
        zman.dakot,
        now.second()
    ) - TimeSpan(minusDakot * 60);
}


// --------------------------------------------------
// Time ranges
// --------------------------------------------------

bool isTimeInRange(Zman begin, Zman end)
{
    return zmanToDateTime(begin) <= now &&
           now <= zmanToDateTime(end);
}


bool isTimeInRange(DateTime begin, Zman end)
{
    return begin <= now &&
           now <= zmanToDateTime(end);
}


bool isTimeInRange(Zman begin, DateTime end)
{
    return zmanToDateTime(begin) <= now &&
           now <= end;
}


bool isTimeInRange(DateTime begin, DateTime end)
{
    return begin <= now &&
           now <= end;
}


// --------------------------------------------------
// DST manual adjustment
// --------------------------------------------------

void setShaonKaitz()
{
    setClockTime(now + TimeSpan(3600));
}


void setShaonHoref()
{
    setClockTime(now - TimeSpan(3600));
}


// --------------------------------------------------
// Change detection
// --------------------------------------------------

bool hourChanged()
{
    return now.hour() != currentHour;
}


bool minuteChanged()
{
    return now.minute() != currentMinute;
}


bool secondChanged()
{
    return now.second() != currentSecond;
}


// --------------------------------------------------
// Exact time comparison
// --------------------------------------------------

bool timeIs(uint8_t shaa, uint8_t daka, uint8_t shniot)
{
    return now.hour() == shaa &&
           now.minute() == daka &&
           now.second() == shniot;
}
