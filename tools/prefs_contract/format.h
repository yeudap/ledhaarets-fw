// format.h — value formatting shared by the host stubs and main.cpp of tools/prefs_contract.py.
// Host-only code; never part of the firmware.
#pragma once

#include <cstdint>
#include <cstdio>
#include <cstring>
#include <string>
#include <type_traits>

namespace contract
{
// Integers in decimal, double as %.15g plus the raw IEEE 754 bits, strings quoted with escapes.
template <typename T>
std::string fmt(const T &v)
{
    if constexpr (std::is_floating_point<T>::value)
    {
        double d = static_cast<double>(v);
        uint64_t bits;
        std::memcpy(&bits, &d, sizeof bits);
        char b[64];
        std::snprintf(b, sizeof b, "%.15g (0x%016llx)", d, static_cast<unsigned long long>(bits));
        return b;
    }
    else if constexpr (std::is_integral<T>::value && std::is_signed<T>::value)
    {
        return std::to_string(static_cast<long long>(v));
    }
    else if constexpr (std::is_integral<T>::value)
    {
        return std::to_string(static_cast<unsigned long long>(v));
    }
    else
    {
        std::string out = "\"";
        for (const char *p = v.c_str(); *p; ++p)
        {
            unsigned char c = static_cast<unsigned char>(*p);
            if (c == '"' || c == '\\')
            {
                out += '\\';
                out += static_cast<char>(c);
            }
            else if (c < 0x20 || c >= 0x7f)
            {
                char b[8];
                std::snprintf(b, sizeof b, "\\x%02x", c);
                out += b;
            }
            else
            {
                out += static_cast<char>(c);
            }
        }
        out += "\"";
        return out;
    }
}
} // namespace contract
