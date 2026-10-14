#include "Sun.h"

Sun::Sun(Place* _p, LoaziDate _d)
{
    place = _p;
    date = _d;
}
void Sun::init()
{
    // Serial.println("Sun: start init.");
    // Serial.print("Sun. timezone: ");
    // Serial.println(place->getTimezone());
    // Serial.print("Sun. longitude: ");
    // Serial.println(place->longitude);
    // Serial.print("Sun. latitude: ");
    // Serial.println(place->latitude);
    double Jcent = calculateJulianCentury(Zman(12+ + place->getTimezone(), 0, 0), date, place->getTimezone());
    double Manom = 357.52911 + Jcent * (35999.05029 - 0.0001537 * Jcent);  // Suns mean anomaly
    double Mlong = fmod((280.46646 + Jcent * (36000.76983 + Jcent * 0.0003032)), 360);  // Suns mean longitude
    double Mobliq = 23 + (26 + (21.448 - Jcent * (46.815 + Jcent * (0.00059 - Jcent * 0.001813))) / 60) / 60;  // Obliquity of the ecliptic
    double Seqcent =
        sin_d(Manom) * (1.914602 - Jcent * (0.004817 + 0.000014 * Jcent)) +
        sin_d(2 * Manom) * (0.019993 - 0.000101 * Jcent) +
        sin_d(3 * Manom) * 0.000289;  // Sun's equation of center
    double Struelong = Mlong + Seqcent;  // Suns true longitude
    double Sapplong = Struelong - 0.00569 - 0.00478 * sin_d(125.04 - 1934.136 * Jcent);  // Apparent longitude L(lambda) of the Sun
    double obliq = Mobliq + 0.00256 * cos_d(125.04 - 1934.136 * Jcent);
    declination = degrees(asin(sin_d(obliq) * sin_d(Sapplong)));
    double Eccent = 0.016708634 - Jcent * (0.000042037 + 0.0000001267 * Jcent);  // eccentricity of Earth's orbit
    double vary = tan_d(obliq / 2) * tan_d(obliq / 2);
    double eqtime = 4 * degrees(
        vary * sin_d(2 * Mlong)
        - 2 * Eccent * sin_d(Manom)
        + 4 * Eccent * vary * sin_d(Manom) * cos_d(2 * Mlong)
        - 0.5 * vary * vary * sin_d(4 * Mlong)
        - 1.25 * Eccent * Eccent * sin_d(2 * Manom)
    );
    solarNoon = (720 - 4 * place->longitude - eqtime + place->getTimezone() * 60) / 1440;
    // Serial.println("Sun: end init.");
}

// Calculations
double Sun::hourangle(double altitude)
{
    double temp = cos_d(altitude) / (cos_d(place->latitude) * cos_d(declination)) - tan_d(place->latitude) * tan_d(declination);
    if (temp < -1) { temp = -1; }
    if (temp > 1) { temp = 1; }
    return degrees(acos(temp));
}
Zman Sun::getTimeByElev(double deg)
{
    if (deg < 0) {
        return decimalDayToTime(solarNoon - hourangle(-deg) * 4 / 1440);
    }
    else {
        return decimalDayToTime(solarNoon + hourangle(deg) * 4 / 1440);
    }
}
Zman Sun::calcZrihaMishorit()
{
    return getTimeByElev(-(90 + refrection));
}
Zman Sun::calcShkiaaMishorit()
{
    return getTimeByElev(90 + refrection);
}
Zman Sun::calcZrihaAstronomit()
{
    double h = 2.076 * sqrt(abs(place->height))/60;
    if(place->height<0) { h = h * -1; }
    return getTimeByElev(-(90 + refrection + h));
}
Zman Sun::calcShkiaaAstronomit()
{
    double h = 2.076 * sqrt(abs(place->height))/60;
    if(place->height<0) { h = h * -1; }
    return getTimeByElev(90 + refrection + h);
}
Zman Sun::calcAlot90(){ return getTimeByElev(-(90 + 19.75)); }
Zman Sun::calcZeet90(){ return getTimeByElev(90 + 19.75); }
Zman Sun::calcAlot72(){ return getTimeByElev(-(90 + 16)); }
Zman Sun::calcZeet72(){ return getTimeByElev(90 + 16); }
double Sun::calculateJulianCentury(Zman time, LoaziDate date, double timezone)
{
    double decimalDay = TimeToDecimalDay(time);
    double Jday = days_from_2000(date) + decimalDay - timezone / 24;
    double Jcent = Jday / 36525;
    return Jcent;
}
double Sun::days_from_2000(LoaziDate d)
{
    // days from 1/1/2000 12:00:00 to d/m/y 00:00:00
    int leap = 0;
    if (d.year % 4 == 0 && (d.year % 100 != 0 || d.year % 400 == 0)) { leap = 1; }
    int month_lengths[13] = { 0, 31, 28 + leap, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    double days = d.day - 1.5;  // not include today and half day of the first day
    for (int m = 1; m < d.month; m++) {
        days += month_lengths[m];
    }
    // if (d.month > 2) { days += leap; }  // Add an extra day for leap years if the date is after February
    for (int y = 2000; y < d.year; y++) {
        days += 365;
        if (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) { days++; }
    }
    return days;
}
