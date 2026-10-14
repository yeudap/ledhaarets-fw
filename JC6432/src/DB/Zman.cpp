#include "Zman.h"
#include "Preferences/UserPreferences.h"

extern UserPreferences userPreferences;

Zman::Zman(uint8_t _shaot, uint8_t _dakot, uint8_t _shniot)
{
    this->shaot = _shaot;
    this->dakot = _dakot;
    this->shniot = _shniot;
    this->totalSec = shaot * 3600 + dakot * 60 + shniot;

}
uint8_t Zman::getShaot() 
{ 
    if (userPreferences.clockFormat == 12) 
    {
        if (shaot == 0 || shaot == 12) { return 12; }
        return shaot % 12; 
    }
    return shaot; 
}
uint8_t Zman::getDakot() { return dakot; }
uint8_t Zman::getShniot() { return shniot; }
void Zman::toString(char* buffer) 
{
    if (userPreferences.timeFormat == 0) 
    {
        sprintf(buffer, "%d:%02d", getShaot(), dakot);
    }
    else{
        sprintf(buffer, "%02d:%02d", getShaot(), dakot);
    }
}
void Zman::toStringSec(char* buffer) 
{ 
    if (userPreferences.timeFormat == 0) 
    {
        sprintf(buffer, "%d:%02d:%02d", getShaot(), dakot, shniot);
    }
    else{
        sprintf(buffer, "%02d:%02d:%02d", getShaot(), dakot, shniot);
    }
}
Zman::Zman(int totalSec)
{
    this->secondToZman(totalSec);
}
void Zman::secondToZman(int totalSecond)
{
    this->totalSec = totalSecond;
    this->shaot = totalSecond / 3600;
    totalSecond %= 3600;
    this->dakot = totalSecond / 60;
    this->shniot = totalSecond % 60;
}

bool Zman::operator>(const Zman& rhs) {
	return totalSec > rhs.totalSec;
}
bool Zman::operator>=(const Zman& rhs) {
	return totalSec >= rhs.totalSec;
}
bool Zman::operator<(const Zman& rhs) {
	return totalSec < rhs.totalSec;
}
bool Zman::operator<=(const Zman& rhs) {
	return totalSec <= rhs.totalSec;
}
bool Zman::operator==(const Zman& rhs) {
	return totalSec == rhs.totalSec;
}
bool Zman::operator!=(const Zman& rhs) {
	return totalSec != rhs.totalSec;
}
Zman Zman::operator+(const Zman& rhs) {
	return Zman(totalSec + rhs.totalSec);
}
Zman Zman::operator+(const double dakot) {
	return Zman(totalSec + (int)dakot*60);
}
Zman Zman::operator-(const Zman& rhs) {
	return Zman(totalSec - rhs.totalSec);
}
Zman Zman::operator-(const double dakot) {
	return Zman(totalSec - (int)dakot * 60);
}
Zman Zman::operator*(const double mul) {
	return Zman((int)(totalSec * mul));
}
Zman Zman::operator/(const double div) {
	return Zman((int)(totalSec / div));
}

Zman Zman::ceil()
{
    if (this->shniot == 0)
    {
        return floor();
    }
    return Zman(totalSec + (60 - this->shniot));
}
Zman Zman::floor()
{
    return Zman(totalSec - this->shniot);
}
Zman Zman::round()
{
    if (this->shniot >= 30)
    {
        return ceil();
    }
    else {
        return floor();
    }
}
Zman max(const Zman& lhs, const Zman& rhs)
{
    if (lhs.totalSec >= rhs.totalSec)
        return lhs;
    return rhs;
}
Zman min(const Zman& lhs, const Zman& rhs)
{
    if (lhs.totalSec < rhs.totalSec)
        return lhs;
    return rhs;
}
Zman decimalDayToTime(double day)
{
    double hours = 24.0 * day;
    int hour = static_cast<int>(hours);
    double minutes = (hours - hour) * 60;
    int minute = static_cast<int>(minutes);
    double seconds = (minutes - minute) * 60;
    int second = static_cast<int>(seconds);
    return Zman(hour, minute, second);
}
double TimeToDecimalDay(Zman& t)
{
    return (t.shaot + t.dakot / 60.0 + t.shniot / 3600.0) / 24.0;
}