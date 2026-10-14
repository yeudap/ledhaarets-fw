#ifndef LoaziDate_H
#define LoaziDate_H

#include <RTClib.h>


class LoaziDate
{
public:
    uint8_t day;
    uint8_t month;
    uint16_t year;
    LoaziDate() : day(0), month(0), year(0) {}
    LoaziDate(uint8_t _day, uint8_t _month, uint16_t _year) : day(_day), month(_month), year(_year) {}
    LoaziDate(const DateTime& d) : day(d.day()), month(d.month()), year(d.year()) {}
    bool isLeap() { return (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)); }
    void toString(char* buffer, uint8_t format) 
    { 
        if(format == 1)      {sprintf(buffer, "%02d/%02d/%d", day, month, year-2000); } // 14/03/2023
        else if(format == 2) {sprintf(buffer, "%d/%d/%d", day, month, year-2000); }     // 14/3/2023
        else if(format == 3) {sprintf(buffer, "%d/%d/%d", month, day, year-2000); }// 3/14/23
        else if(format == 4) {sprintf(buffer, "%d/%d", day, month); }              // 14/3
        else if(format == 5) {sprintf(buffer, "%02d/%02d/%d", month, day, year-2000); } // 03/14/2023
        else if(format == 6) {sprintf(buffer, "%d=%02d=%02d", year-2000, month, day); } // 2023-03-14
        else if(format == 7) {sprintf(buffer, "%02d.%02d.%d", day, month, year-2000); } // 14.03.2023
        else if(format == 8) {sprintf(buffer, ""); } // empty
    }

    bool operator==(const LoaziDate& other) const {
        return (day == other.day && month == other.month && year == other.year);
    }
    bool operator<(const LoaziDate& other) const {
        if (year != other.year) return year < other.year;
        if (month != other.month) return month < other.month;
        return day < other.day;
    }
    bool operator>(const LoaziDate& other) const {
        return other < *this;
    }
    bool operator<=(const LoaziDate& other) const {
        return !(*this > other);
    }

    bool operator>=(const LoaziDate& other) const {
        return !(*this < other);
    }
};

#endif