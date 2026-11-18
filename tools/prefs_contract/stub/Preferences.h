// Preferences.h — host stub for tools/prefs_contract.py. Not the ESP32 library, never part of the
// firmware. No storage: every get returns the default it was given, as on a clock with empty NVS.
// Every call is appended to the NVS log, which main.cpp reports in the [nvs-load] section.
#pragma once

#include "Arduino.h"
#include "../format.h"
#include <string>
#include <vector>

// Same enumerators, same order (= same values) as
// framework-arduinoespressif32/libraries/Preferences/src/Preferences.h:19-21 (Arduino-ESP32 2.0.17).
typedef enum
{
    PT_I8, PT_U8, PT_I16, PT_U16, PT_I32, PT_U32, PT_I64, PT_U64, PT_STR, PT_BLOB, PT_INVALID
} PreferenceType;

struct NvsCall
{
    std::string ns, fn, key, value;
};

// Never freed, so calls from static destructors stay safe.
inline std::vector<NvsCall> &nvsLog()
{
    static std::vector<NvsCall> *log = new std::vector<NvsCall>();
    return *log;
}

class Preferences
{
    std::string _ns;

    template <typename T>
    T get(const char *fn, const char *key, T def)
    {
        nvsLog().push_back({_ns, fn, key, contract::fmt(def)});
        return def;
    }
    template <typename T>
    size_t put(const char *fn, const char *key, T value)
    {
        nvsLog().push_back({_ns, fn, key, contract::fmt(value)});
        return 1;
    }

public:
    const std::string &ns() const { return _ns; }

    bool begin(const char *name, bool readOnly = false, const char *partition_label = NULL)
    {
        _ns = name;
        nvsLog().push_back({_ns, "begin", "", readOnly ? "ro" : "rw"});
        return true;
    }
    void end() { nvsLog().push_back({_ns, "end", "", ""}); }

    int8_t getChar(const char *key, int8_t d = 0) { return get("getChar", key, d); }
    uint8_t getUChar(const char *key, uint8_t d = 0) { return get("getUChar", key, d); }
    int16_t getShort(const char *key, int16_t d = 0) { return get("getShort", key, d); }
    uint16_t getUShort(const char *key, uint16_t d = 0) { return get("getUShort", key, d); }
    double getDouble(const char *key, double d = NAN) { return get("getDouble", key, d); }
    String getString(const char *key, String d = String()) { return get("getString", key, d); }

    size_t putChar(const char *key, int8_t v) { return put("putChar", key, v); }
    size_t putUChar(const char *key, uint8_t v) { return put("putUChar", key, v); }
    size_t putShort(const char *key, int16_t v) { return put("putShort", key, v); }
    size_t putUShort(const char *key, uint16_t v) { return put("putUShort", key, v); }
    size_t putDouble(const char *key, double v) { return put("putDouble", key, v); }
    size_t putString(const char *key, String v) { return put("putString", key, v); }
};
