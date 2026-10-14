#ifndef SUN_H
#define SUN_H

#include "LoaziDate.h"
#include "Zman.h"
#include "Place.h"

#define sin_d(x) (sin((x)*DEG_TO_RAD))
#define cos_d(x) (cos((x)*DEG_TO_RAD))
#define tan_d(x) (tan((x)*DEG_TO_RAD))

class Sun
{
public:
    const double refrection = 0.833;
    // const double refrection = 50/60;
    double declination = 0;
    double solarNoon = 0;
    Place* place;
    LoaziDate date;

    Sun(){}
    Sun(Place* _p, LoaziDate _d);
    void init();
    double hourangle(double altitude);
    Zman getTimeByElev(double deg);
    double calculateJulianCentury(Zman, LoaziDate, double);
    double days_from_2000(LoaziDate d);
    Zman calcZrihaMishorit();
    Zman calcShkiaaMishorit();
    Zman calcZrihaAstronomit();
    Zman calcShkiaaAstronomit();
    Zman calcAlot90();
    Zman calcZeet90();
    Zman calcAlot72();
    Zman calcZeet72();
};

#endif
