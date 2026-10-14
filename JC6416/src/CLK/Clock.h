#ifndef CLOCK_H
#define CLOCK_H

#include <Arduino.h>
#include <RTClib.h>
#include "DB/Zman.h"

enum class ClockType : uint8_t
{
    DS3231,
    RV3028
};
const int idx0Year = 2022;
const int idx0mounth = 9;
const int idx0day = 26;

const DateTime MIN_DATE = DateTime(2022, 9, 27, 0, 0, 0);
const DateTime MAX_DATE = DateTime(2033, 9, 22, 0, 0, 0);

// Global current time
extern DateTime now;

extern int currentHour;
extern int currentMinute;
extern int currentSecond;
extern int currentSecond1;
extern int currentSecond2;
extern int currentDay;
extern int currentMonth;
extern int currentYear;

// Select the physical RTC before validate_rtc() is called.
void selectClock(ClockType type);

void validate_rtc();
void validate_time();

void updateClockTime();
void setClockTime(const DateTime& dt);

int getCurrentDayIndex();
void syncClock();

DateTime zmanToDateTime(Zman zman);
Zman dateTimeToZman(DateTime dateTime);

DateTime zmanPlusDakot(Zman zman, uint16_t plusDakot);
DateTime zmanMinusDakot(Zman zman, uint16_t minusDakot);

bool isTimeInRange(Zman begin, Zman end);
bool isTimeInRange(DateTime begin, Zman end);
bool isTimeInRange(Zman begin, DateTime end);
bool isTimeInRange(DateTime begin, DateTime end);

void setShaonKaitz();
void setShaonHoref();

bool hourChanged();
bool minuteChanged();
bool secondChanged();

bool timeIs(uint8_t shaa, uint8_t daka, uint8_t shniot);

#endif
