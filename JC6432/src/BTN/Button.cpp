#include "Button.h"
#include "ButtonLH1022.h"
#include "ButtonLH0826.h"

extern ButtonCommand currentButtonCommand;

int long_push_temp = 0;

static ButtonLH1022 lh1022Driver;
static ButtonLH0826 lh0826Driver;
static IButtonDriver* activeButtonDriver = nullptr;

Button::Button(ButtonCommand command)
    : bt_command(command)
{
}

bool Button::isPushed()
{
    // BLE button commands are hardware-independent.
    if (currentButtonCommand == bt_command)
        return true;

    if (activeButtonDriver == nullptr)
        return false;

    return activeButtonDriver->isPushed(bt_command);
}

void selectButtonDriver(ButtonDriverType type)
{
    switch (type)
    {
        case ButtonDriverType::LH1022:
            activeButtonDriver = &lh1022Driver;
            break;

        case ButtonDriverType::LH0826:
            activeButtonDriver = &lh0826Driver;
            break;
    }
}

Button menuButton(ButtonCommand::MENU);
Button upButton(ButtonCommand::UP);
Button downButton(ButtonCommand::DOWN);
Button okButton(ButtonCommand::OK);
