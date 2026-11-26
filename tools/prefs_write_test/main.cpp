// main.cpp — host program of tools/prefs_write_test.py. Never part of the firmware.
// Compiled together with a model's src/Preferences/*.cpp against the stubs of tools/prefs_contract/stub
// (no storage: every get returns the default; every put is logged). Runs setupPreferences(), registers
// the same checks as the firmware (below), and writes through the registry entry point
// (UserPreferences::writeBytes / write) as handlePref and the editors do. Prints one line per case:
// PASS / FAIL, the result, the value after the write and the NVS puts of that key. Exit code 1 on a FAIL.
//
// The checks mirror DB/Place.cpp (ofekInRange) and DB/Shita.cpp (shitaInRange, zeetInRange), which
// cannot be compiled on the host. prefs_write_test.py verifies, in the model's source, that their bodies
// and registrations are exactly these, and passes the model's bounds (TEST_NUM_OFEKS: the number of
// entries in places[]; TEST_NUM_SHITOT, TEST_NUM_ZEET: the constants in DB/Shita.h).
#include "UserPreferences.h"
#include "DataPrefs.h"
#include "format.h"

#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

HostSerial Serial;
extern UserPreferences userPreferences;

namespace
{
int numOfOfeks = TEST_NUM_OFEKS; // set by initPlace in the firmware
const int numOfShitot = TEST_NUM_SHITOT;
const int numOfZeetCochavin = TEST_NUM_ZEET;

bool ofekInRange(const uint8_t &value)
{
    return value < numOfOfeks;
}
bool shitaInRange(const uint8_t &value)
{
    return value < numOfShitot;
}
bool zeetInRange(const uint8_t &value)
{
    return value <= numOfZeetCochavin;
}

void registerChecks()
{
    UserPreferences::registerCheck(dataPrefs.place.ofek, ofekInRange);
    UserPreferences::registerCheck(dataPrefs.zmanim.shita, shitaInRange);
    UserPreferences::registerCheck(dataPrefs.zmanim.zeetCochavim, zeetInRange);
    UserPreferences::registerCheck(dataPrefs.zmanim.rabenuTam, zeetInRange);
    UserPreferences::registerCheck(dataPrefs.zmanim.zeetShabat, zeetInRange);
    UserPreferences::registerCheck(dataPrefs.zmanim.zeetZom, zeetInRange);
}

int failures = 0;
int passes = 0;

const char *resultName(PrefWriteResult r)
{
    switch (r)
    {
    case PW_CHANGED: return "CHANGED";
    case PW_UNCHANGED: return "UNCHANGED";
    case PW_BAD_BYTES: return "BAD_BYTES";
    case PW_REJECTED: return "REJECTED";
    }
    return "?";
}

UserPrefBase *find(const char *key)
{
    for (UserPrefBase *p : userPreferences.allPrefs)
    {
        if (std::strcmp(p->getKey(), key) == 0)
            return p;
    }
    return nullptr;
}

template <typename T>
bool valueAs(const UserPrefBase *p, std::string &out)
{
    auto q = dynamic_cast<const UserPref<T> *>(p);
    if (!q)
        return false;
    out = contract::fmt(q->getValue());
    return true;
}

std::string valueOf(const UserPrefBase *p)
{
    std::string v;
    if (valueAs<uint8_t>(p, v) || valueAs<int8_t>(p, v) || valueAs<uint16_t>(p, v) ||
        valueAs<int16_t>(p, v) || valueAs<double>(p, v) || valueAs<String>(p, v))
        return v;
    return "(unknown type)";
}

// The put* calls for key after the NVS log mark; every one must carry the value now in RAM.
int putsSince(size_t mark, const char *key, const std::string &value, bool &valuesOk)
{
    int n = 0;
    valuesOk = true;
    const std::vector<NvsCall> &log = nvsLog();
    for (size_t i = mark; i < log.size(); i++)
    {
        if (log[i].fn.compare(0, 3, "put") == 0 && log[i].key == key)
        {
            n++;
            if (log[i].value != value)
                valuesOk = false;
        }
    }
    return n;
}

void report(bool ok, const char *name, const char *key, PrefWriteResult r, const std::string &v, int puts,
            PrefWriteResult expR, const std::string &expV, int expPuts)
{
    std::printf("%s\t%s\t%s\t%s\tvalue=%s\tputs=%d\n", ok ? "PASS" : "FAIL", name, key, resultName(r),
                v.c_str(), puts);
    if (!ok)
    {
        std::printf("\texpected\t%s\tvalue=%s\tputs=%d (with that value)\n", resultName(expR), expV.c_str(),
                    expPuts);
        failures++;
    }
    else
    {
        passes++;
    }
}

// One write through writeBytes (the BLE path). expPuts: 1 on a change, else 0 (save only on a change).
void bytesCase(const char *name, const char *key, const std::vector<uint8_t> &bytes, PrefWriteResult expR,
               const std::string &expV)
{
    UserPrefBase *p = find(key);
    if (!p)
    {
        std::printf("FAIL\t%s\t%s\tkey not in allPrefs\n", name, key);
        failures++;
        return;
    }
    int expPuts = expR == PW_CHANGED ? 1 : 0;
    size_t mark = nvsLog().size();
    PrefWriteResult r = UserPreferences::writeBytes(*p, bytes.data(), bytes.size());
    std::string v = valueOf(p);
    bool valuesOk;
    int puts = putsSince(mark, key, v, valuesOk);
    report(r == expR && v == expV && puts == expPuts && valuesOk, name, key, r, v, puts, expR, expV, expPuts);
}

// One write through the typed entry point (the editors' path from 2b.2).
template <typename T, typename V>
void typedCase(const char *name, UserPref<T> &pref, V value, PrefWriteResult expR, const std::string &expV)
{
    int expPuts = expR == PW_CHANGED ? 1 : 0;
    size_t mark = nvsLog().size();
    PrefWriteResult r = UserPreferences::write(pref, value);
    std::string v = contract::fmt(pref.getValue());
    bool valuesOk;
    int puts = putsSince(mark, pref.getKey(), v, valuesOk);
    report(r == expR && v == expV && puts == expPuts && valuesOk, name, pref.getKey(), r, v, puts, expR, expV,
           expPuts);
}

std::vector<uint8_t> u8(uint8_t v) { return {v}; }
std::vector<uint8_t> le16(uint16_t v) { return {static_cast<uint8_t>(v & 0xFF), static_cast<uint8_t>(v >> 8)}; }
std::vector<uint8_t> f64(double d)
{
    std::vector<uint8_t> b(sizeof d);
    std::memcpy(b.data(), &d, sizeof d);
    return b;
}
std::vector<uint8_t> str(const char *s, size_t len) { return std::vector<uint8_t>(s, s + len); }

std::string I(long long v) { return std::to_string(v); }
std::string D(double d) { return contract::fmt(d); }
std::string S(const char *s) { return contract::fmt(String(s)); }
} // namespace

int main()
{
    setupPreferences();
    registerChecks();

    // Decision 2: writing the value already in RAM (here: every default) returns UNCHANGED and saves nothing.
    {
        int n = 0, bad = 0;
        for (UserPrefBase *p : userPreferences.allPrefs)
        {
            std::vector<uint8_t> bytes = p->toBytes();
            std::string before = valueOf(p);
            size_t mark = nvsLog().size();
            PrefWriteResult r = UserPreferences::writeBytes(*p, bytes.data(), bytes.size());
            bool valuesOk;
            int puts = putsSince(mark, p->getKey(), before, valuesOk);
            n++;
            if (r != PW_UNCHANGED || puts != 0 || valueOf(p) != before)
            {
                std::printf("FAIL\tdefault round trip\t%s\t%s\tputs=%d\n", p->getKey(), resultName(r), puts);
                bad++;
            }
        }
        std::printf("%s\tdefault round trip (toBytes -> writeBytes), all %d preferences\tUNCHANGED, puts=0\n",
                    bad ? "FAIL" : "PASS", n);
        bad ? failures++ : passes++;
    }

    // shita: < numOfShitot
    bytesCase("shita last", "shita", u8(numOfShitot - 1), PW_CHANGED, I(numOfShitot - 1));
    bytesCase("shita = numOfShitot", "shita", u8(numOfShitot), PW_REJECTED, I(numOfShitot - 1));
    bytesCase("shita 255", "shita", u8(255), PW_REJECTED, I(numOfShitot - 1));
    bytesCase("shita same again", "shita", u8(numOfShitot - 1), PW_UNCHANGED, I(numOfShitot - 1));
    bytesCase("shita no bytes", "shita", {}, PW_BAD_BYTES, I(numOfShitot - 1));
    bytesCase("shita 0", "shita", u8(0), PW_CHANGED, I(0));
    bytesCase("shita extra byte ignored", "shita", {3, 0x55}, PW_CHANGED, I(3));
    typedCase("shita typed = numOfShitot", dataPrefs.zmanim.shita, numOfShitot, PW_REJECTED, I(3));
    typedCase("shita typed 4", dataPrefs.zmanim.shita, 4, PW_CHANGED, I(4));

    // ofek: < numOfOfeks
    bytesCase("ofek last", "ofek", u8(numOfOfeks - 1), PW_CHANGED, I(numOfOfeks - 1));
    bytesCase("ofek = numOfOfeks", "ofek", u8(numOfOfeks), PW_REJECTED, I(numOfOfeks - 1));
    bytesCase("ofek 0 (custom place)", "ofek", u8(0), PW_CHANGED, I(0));
    bytesCase("ofek 0 again", "ofek", u8(0), PW_UNCHANGED, I(0));
    bytesCase("ofek no bytes", "ofek", {}, PW_BAD_BYTES, I(0));

    // the four zeet keys: <= numOfZeetCochavin, 0 accepted (P8)
    for (const char *k : {"zeetCochavim", "rabenuTam", "zeetShabat", "zeetZom"})
    {
        bytesCase("zeet 0", k, u8(0), PW_CHANGED, I(0));
        bytesCase("zeet max", k, u8(numOfZeetCochavin), PW_CHANGED, I(numOfZeetCochavin));
        bytesCase("zeet max+1", k, u8(numOfZeetCochavin + 1), PW_REJECTED, I(numOfZeetCochavin));
        bytesCase("zeet no bytes", k, {}, PW_BAD_BYTES, I(numOfZeetCochavin));
    }

    // a preference with no registered check accepts any value of its type (as before 2b.1; P1)
    bytesCase("u8 without check", "dateFormat", u8(200), PW_CHANGED, I(200));

    // short lengths and signed values per type
    bytesCase("i8 negative", "timezone", u8(0xF4), PW_CHANGED, I(-12));
    bytesCase("i8 no bytes", "timezone", {}, PW_BAD_BYTES, I(-12));
    bytesCase("i16 negative", "nz_height", le16(static_cast<uint16_t>(-200)), PW_CHANGED, I(-200));
    bytesCase("i16 one byte", "nz_height", u8(0x10), PW_BAD_BYTES, I(-200));
    bytesCase("u16 two bytes LE", "interval", le16(0x1234), PW_CHANGED, I(0x1234));
    bytesCase("u16 one byte", "interval", u8(0x10), PW_BAD_BYTES, I(0x1234));
    bytesCase("double negative", "nz_lat", f64(-31.5), PW_CHANGED, D(-31.5));
    bytesCase("double 7 bytes", "nz_lat", str("\x01\x02\x03\x04\x05\x06\x07", 7), PW_BAD_BYTES, D(-31.5));
    bytesCase("double", "nz_long", f64(35.25), PW_CHANGED, D(35.25));
    bytesCase("double same again", "nz_long", f64(35.25), PW_UNCHANGED, D(35.25));

    // String: up to the first \0, or the whole value without one
    bytesCase("String with \\0", "textPlace", str("abc\0", 4), PW_CHANGED, S("abc"));
    bytesCase("String without \\0, same", "textPlace", str("abc", 3), PW_UNCHANGED, S("abc"));
    bytesCase("String cut at \\0", "textPlace", str("ab\0cd", 5), PW_CHANGED, S("ab"));
    {
        std::string longName(70, 'x');
        bytesCase("String 70 bytes", "textPlace", str(longName.c_str(), longName.size()), PW_CHANGED,
                  S(longName.c_str()));
    }
    bytesCase("String only \\0", "textPlace", str("\0", 1), PW_CHANGED, S(""));
    bytesCase("String no bytes", "textPlace", {}, PW_BAD_BYTES, S(""));
    bytesCase("text slot", "textInput1", str("vkkv\0", 5), PW_CHANGED, S("vkkv"));

    std::printf("[summary] %d passed, %d failed\n", passes, failures);
    std::fflush(stdout);
    return failures ? 1 : 0;
}
