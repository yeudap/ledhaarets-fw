#ifndef BUTTON_DRIVER_H
#define BUTTON_DRIVER_H

#include <BLE/Commands.h>

class IButtonDriver
{
public:
    virtual ~IButtonDriver() = default;
    virtual bool isPushed(ButtonCommand command) = 0;
};

#endif
