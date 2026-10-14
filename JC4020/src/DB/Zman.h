#ifndef ZMAN_H
#define ZMAN_H


#include <Arduino.h>


class Zman
{
public:
    uint8_t shaot;
    uint8_t dakot;
    uint8_t shniot;

    int totalSec;

    Zman(uint8_t _shaot = 0, uint8_t _dakot = 0, uint8_t _shniot = 0);
    Zman(int totalSec);

    uint8_t getShaot();
    uint8_t getDakot();
    uint8_t getShniot();

    void secondToZman(int);
    void toString(char*);
    void toStringSec(char*);
    bool operator>(const Zman&);
    bool operator>=(const Zman&);
    bool operator<(const Zman&);
    bool operator<=(const Zman&);
    bool operator==(const Zman&);
    bool operator!=(const Zman&);
    Zman operator+(const Zman&);
    Zman operator+(const double);
    Zman operator-(const Zman&);
    Zman operator-(const double);
    Zman operator*(const double);
    Zman operator/(const double);

    Zman ceil();
    Zman round();
    Zman floor();
};
Zman max(const Zman&, const Zman&);
Zman min(const Zman&, const Zman&);
Zman decimalDayToTime(double day);
double TimeToDecimalDay(Zman&);

#endif