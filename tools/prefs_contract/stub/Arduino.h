// Arduino.h — host stub for tools/prefs_contract.py. Not the Arduino core, never part of the firmware.
// Provides only what src/Preferences needs: String, Serial and the fixed-width integer types.
#pragma once

#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <math.h>
#include <stdint.h>
#include <string>
#include <type_traits>

class String
{
    std::string _s;

public:
    String() {}
    String(const char *c) : _s(c ? c : "") {}
    String(const char *cstr, unsigned int length) : _s(cstr ? std::string(cstr, length) : std::string()) {}
    const char *c_str() const { return _s.c_str(); }
    size_t length() const { return _s.size(); }
    char operator[](size_t i) const { return _s[i]; }
    bool operator==(const String &o) const { return _s == o._s; }
    bool operator!=(const String &o) const { return _s != o._s; }
};

// Serial: everything printed is kept in a buffer; main.cpp reports it in the [serial] section.
// The buffer is never freed, so printing from static destructors stays safe.
class HostSerial
{
    std::string *_out = new std::string();

    template <typename T>
    void put(const T &v)
    {
        if constexpr (std::is_same<T, String>::value)
            *_out += v.c_str();
        else if constexpr (std::is_floating_point<T>::value)
        {
            char b[64];
            std::snprintf(b, sizeof b, "%.2f", static_cast<double>(v));
            *_out += b;
        }
        else if constexpr (std::is_integral<T>::value && std::is_signed<T>::value)
            *_out += std::to_string(static_cast<long long>(v));
        else if constexpr (std::is_integral<T>::value)
            *_out += std::to_string(static_cast<unsigned long long>(v));
        else
            *_out += v;
    }

public:
    template <typename T>
    void print(const T &v) { put(v); }
    template <typename T>
    void println(const T &v)
    {
        put(v);
        *_out += "\n";
    }
    void println() { *_out += "\n"; }
    std::string &buffer() { return *_out; }
};

extern HostSerial Serial;
