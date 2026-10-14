#ifndef METADATA_H
#define METADATA_H

#include <cstddef>
#include <cstdint>

#ifndef PROJECT_VER
#define PROJECT_VER "0.00"
#endif

#ifndef PRODUCT_MANUFACTURER
#define PRODUCT_MANUFACTURER "LEDHAARETS"
#endif

#ifndef HARDWARE_VERSION
#define HARDWARE_VERSION "LH1022"
#endif

#ifndef DEVICE_MODEL
#define DEVICE_MODEL "JC6416"
#endif

namespace Metadata {

inline constexpr char manufacturer[] = PRODUCT_MANUFACTURER;
inline constexpr char SWversion[]    = PROJECT_VER;
inline constexpr char HWversion[]    = HARDWARE_VERSION;
inline constexpr char model[]        = DEVICE_MODEL;

inline constexpr std::size_t manufacturer_len = 10;
inline constexpr std::size_t SWversion_len    = 4;
inline constexpr std::size_t HWversion_len    = 6;
inline constexpr std::size_t model_len        = 6;

inline constexpr std::size_t combinedData_len =
    manufacturer_len +
    SWversion_len +
    HWversion_len +
    model_len +
    1; // null terminator

static_assert(
    sizeof(manufacturer) - 1 == manufacturer_len,
    "Manufacturer must be exactly 10 characters"
);

static_assert(
    sizeof(SWversion) - 1 == SWversion_len,
    "SW version must be exactly 4 characters"
);

static_assert(
    sizeof(HWversion) - 1 == HWversion_len,
    "HW version must be exactly 6 characters"
);

static_assert(
    sizeof(model) - 1 == model_len,
    "Model must be exactly 6 characters"
);

static_assert(
    combinedData_len == 27,
    "Combined manufacturer data must be exactly 27 bytes"
);

void combineManufacturerData(
    char (&combinedData)[combinedData_len]
);

} // namespace Metadata

#endif