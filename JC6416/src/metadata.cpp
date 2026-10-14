#include "metadata.h"

#include <cstring>

namespace Metadata {

void combineManufacturerData(
    char (&combinedData)[combinedData_len])
{
    memcpy(&combinedData[0],  manufacturer, manufacturer_len);
    memcpy(&combinedData[10], SWversion,    SWversion_len);
    memcpy(&combinedData[14], HWversion,    HWversion_len);
    memcpy(&combinedData[20], model,        model_len);

    combinedData[combinedData_len - 1] = '\0';
}

} // namespace Metadata
