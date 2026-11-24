// main.cpp — host program of tools/prefs_contract.py. Never part of the firmware.
// Compiled together with a model's src/Preferences/*.cpp (registry and layer files) against the stubs
// in stub/. Runs setupPreferences() as the firmware's setup() does, then prints the registry as
// built: simplePrefs, textPrefs, allPrefs (one line per entry, in list order), the NVS calls made
// during setupPreferences(), and anything printed to Serial meanwhile.
#include "UserPreferences.h"
#include "format.h"

#include <cstdio>
#include <string>
#include <vector>

HostSerial Serial;
extern UserPreferences userPreferences;

namespace
{
// Reads the protected Preferences pointer without changing the firmware classes: UserPrefBase::_prefs
// (trees before 2a.4) or UserPrefBase::_meta->prefs (since 2a.4: the definition is a table entry in
// flash). The overload is chosen by which member exists (SFINAE); call as PrefsPeek::of(p, 0).
template <typename B>
struct PrefsPeekT : B
{
    template <typename Q = B, typename = decltype(&PrefsPeekT<Q>::_meta)>
    static Preferences *of(const B *p, int) { return (p->*(&PrefsPeekT<Q>::_meta))->prefs; }
    template <typename Q = B, typename = decltype(&PrefsPeekT<Q>::_prefs)>
    static Preferences *of(const B *p, long) { return p->*(&PrefsPeekT<Q>::_prefs); }
};
using PrefsPeek = PrefsPeekT<UserPrefBase>;

const char *const TYPE_NAMES[] = {"PT_I8", "PT_U8", "PT_I16", "PT_U16", "PT_I32", "PT_U32",
                                  "PT_I64", "PT_U64", "PT_STR", "PT_BLOB", "PT_INVALID"};

template <typename T>
bool describeAs(const UserPrefBase *p, const char *name, std::string &cxx, std::string &def)
{
    auto q = dynamic_cast<const UserPref<T> *>(p);
    if (!q)
        return false;
    cxx = name;
    def = contract::fmt(q->getDefault());
    return true;
}

bool describe(const UserPrefBase *p, std::string &cxx, std::string &def)
{
    return describeAs<uint8_t>(p, "uint8_t", cxx, def) ||
           describeAs<int8_t>(p, "int8_t", cxx, def) ||
           describeAs<uint16_t>(p, "uint16_t", cxx, def) ||
           describeAs<int16_t>(p, "int16_t", cxx, def) ||
           describeAs<uint32_t>(p, "uint32_t", cxx, def) ||
           describeAs<int32_t>(p, "int32_t", cxx, def) ||
           describeAs<double>(p, "double", cxx, def) ||
           describeAs<String>(p, "String", cxx, def);
}

// list: std::vector<UserPrefBase *> (trees before 2a.4) or the registry's PrefList (since 2a.4).
template <typename List>
int printList(const char *name, const List &list)
{
    std::printf("[%s] %u\n", name, static_cast<unsigned>(list.size()));
    for (const UserPrefBase *p : list)
    {
        std::string cxx, def;
        if (!describe(p, cxx, def))
        {
            std::fprintf(stderr, "error: %s: unknown UserPref<T> type\n", p->getKey());
            return 1;
        }
        unsigned t = static_cast<unsigned>(p->getType());
        const char *tname = t < sizeof TYPE_NAMES / sizeof TYPE_NAMES[0] ? TYPE_NAMES[t] : "?";
        Preferences *prefs = PrefsPeek::of(p, 0);
        std::printf("%s\t%s(%u)\t%u\t%s\t%s\t%s\n", p->getKey(), tname, t,
                    static_cast<unsigned>(p->getIndex()), def.c_str(), cxx.c_str(),
                    prefs ? prefs->ns().c_str() : "(null)");
    }
    return 0;
}
} // namespace

int main()
{
    setupPreferences();

    // Copy what setup produced before anything else can add to it.
    std::vector<NvsCall> load = nvsLog();
    std::string serial = Serial.buffer();

    if (printList("simplePrefs", userPreferences.simplePrefs) ||
        printList("textPrefs", userPreferences.textPrefs) ||
        printList("allPrefs", userPreferences.allPrefs))
        return 1;

    std::printf("[nvs-load] %u\n", static_cast<unsigned>(load.size()));
    for (const NvsCall &c : load)
        std::printf("%s\t%s\t%s\t%s\n", c.ns.c_str(), c.fn.c_str(), c.key.c_str(), c.value.c_str());

    std::vector<std::string> lines;
    size_t start = 0;
    while (start < serial.size())
    {
        size_t nl = serial.find('\n', start);
        if (nl == std::string::npos)
            nl = serial.size();
        lines.push_back(serial.substr(start, nl - start));
        start = nl + 1;
    }
    std::printf("[serial] %u\n", static_cast<unsigned>(lines.size()));
    for (const std::string &l : lines)
        std::printf("%s\n", l.c_str());
    std::fflush(stdout);
    return 0;
}
