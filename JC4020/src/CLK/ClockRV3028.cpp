#include "ClockRV3028.h"
#include <Wire.h>

// ------------------------------------------------------------
// RV-3028 registers used by this driver
// ------------------------------------------------------------

static constexpr uint8_t CLK_RV_REG_STATUS = 0x0E;
static constexpr uint8_t CLK_RV_REG_CTRL1  = 0x0F;

static constexpr uint8_t CLK_RV_REG_EEADDR = 0x25;
static constexpr uint8_t CLK_RV_REG_EEDATA = 0x26;
static constexpr uint8_t CLK_RV_REG_EECMD  = 0x27;

static constexpr uint8_t CLK_RV_REG_BACKUP = 0x37;

static constexpr uint8_t CLK_RV_EEBUSY_BIT = 7;
static constexpr uint8_t CLK_RV_EERD_BIT   = 3;


// ------------------------------------------------------------
// Low-level I2C register access
// ------------------------------------------------------------

bool ClockRV3028::writeRegister(uint8_t reg, uint8_t value)
{
    Wire.beginTransmission(RV3028_ADDR);
    Wire.write(reg);
    Wire.write(value);

    return Wire.endTransmission() == 0;
}


bool ClockRV3028::readRegister(uint8_t reg, uint8_t &value)
{
    Wire.beginTransmission(RV3028_ADDR);
    Wire.write(reg);

    if (Wire.endTransmission(false) != 0)
        return false;

    if (Wire.requestFrom(RV3028_ADDR, (uint8_t)1) != 1)
        return false;

    value = Wire.read();

    return true;
}


// ------------------------------------------------------------
// Wait until EEPROM controller is ready
// ------------------------------------------------------------

bool ClockRV3028::waitEEPROM(uint32_t timeoutMs)
{
    uint32_t start = millis();

    while ((millis() - start) < timeoutMs)
    {
        uint8_t status;

        if (!readRegister(CLK_RV_REG_STATUS, status))
            return false;

        if ((status & (1 << CLK_RV_EEBUSY_BIT)) == 0)
            return true;

        delay(1);
    }

    return false;
}


// ------------------------------------------------------------
// Write one EEPROM byte
// RV-3028 EEPROM command 0x21
// ------------------------------------------------------------

bool ClockRV3028::writeEEPROMByte(uint8_t address, uint8_t value)
{
    uint8_t ctrl1;

    if (!readRegister(CLK_RV_REG_CTRL1, ctrl1))
        return false;

    // Set EERD = 1
    ctrl1 |= (1 << CLK_RV_EERD_BIT);

    if (!writeRegister(CLK_RV_REG_CTRL1, ctrl1))
        return false;

    if (!waitEEPROM(500))
        return false;

    // EEPROM address
    if (!writeRegister(CLK_RV_REG_EEADDR, address))
        return false;

    // EEPROM data
    if (!writeRegister(CLK_RV_REG_EEDATA, value))
        return false;

    // Required EEPROM command sequence: 00h -> 21h
    if (!writeRegister(CLK_RV_REG_EECMD, 0x00))
        return false;

    if (!writeRegister(CLK_RV_REG_EECMD, 0x21))
        return false;

    if (!waitEEPROM(500))
        return false;

    // Clear EERD
    if (!readRegister(CLK_RV_REG_CTRL1, ctrl1))
        return false;

    ctrl1 &= ~(1 << CLK_RV_EERD_BIT);

    if (!writeRegister(CLK_RV_REG_CTRL1, ctrl1))
        return false;

    return true;
}


// ------------------------------------------------------------
// Ensure correct backup configuration
// ------------------------------------------------------------

bool ClockRV3028::ensureBackupConfiguration()
{
    /*
        Backup EEPROM register 0x37

        Desired configuration:

        TCE  = 0
            Disable trickle charger.
            Required because CR1220 is non-rechargeable.

        FEDE = 1
            Fast edge detection enabled.

        BSM  = 11
            Level Switching Mode.

        All unrelated bits are preserved.
    */

    uint8_t current =
        rtc.readConfigEEPROM_RAMmirror(CLK_RV_REG_BACKUP);

    Serial.print("RV3028 EEPROM 0x37: 0x");

    if (current < 0x10)
        Serial.print('0');

    Serial.println(current, HEX);


    uint8_t wanted = current;

    // TCE = 0
    wanted &= ~(1 << 5);

    // FEDE = 1
    wanted |= (1 << 4);

    // BSM = 11
    wanted &= ~(0b11 << 2);
    wanted |=  (0b11 << 2);


    // Already configured correctly
    if (wanted == current)
    {
        Serial.println("RV3028 backup config already correct");
        return true;
    }


    Serial.print("RV3028 new EEPROM 0x37: 0x");

    if (wanted < 0x10)
        Serial.print('0');

    Serial.println(wanted, HEX);


    /*
        EEPROM access must be performed while backup
        switchover is disabled.

        Change only the active RAM mirror temporarily.
        Do not modify EEPROM yet.
    */

    uint8_t temporary = current;

    // BSM = 00
    temporary &= ~(0b11 << 2);

    if (!writeRegister(CLK_RV_REG_BACKUP, temporary))
    {
        Serial.println("RV3028: failed to disable BSM in RAM");
        return false;
    }


    // Write only EEPROM byte 0x37
    if (!writeEEPROMByte(CLK_RV_REG_BACKUP, wanted))
    {
        Serial.println("RV3028: EEPROM write failed");

        // Best effort: restore previous active RAM config
        writeRegister(CLK_RV_REG_BACKUP, current);

        return false;
    }


    /*
        EEPROM now contains the wanted configuration.
        Apply the same value immediately to the RAM mirror.
    */

    if (!writeRegister(CLK_RV_REG_BACKUP, wanted))
    {
        Serial.println("RV3028: failed to restore backup config to RAM");
        return false;
    }


    // Verify EEPROM
    uint8_t verify =
        rtc.readConfigEEPROM_RAMmirror(CLK_RV_REG_BACKUP);

    if (verify != wanted)
    {
        Serial.print("RV3028: EEPROM verify failed, read 0x");

        if (verify < 0x10)
            Serial.print('0');

        Serial.println(verify, HEX);

        return false;
    }


    Serial.println("RV3028 backup config updated successfully");

    return true;
}


// ------------------------------------------------------------
// Driver initialization
// ------------------------------------------------------------

bool ClockRV3028::begin()
{
    if (!Wire.begin(21, 22, 100000))
    {
        Serial.println("I2C init failed");
        return false;
    }

    /*
        Do NOT use rtc.begin() with default arguments.

        true  -> set 24-hour mode
        false -> do not run disableTrickleCharge()
        false -> do not run setBackupSwitchoverMode()

        Backup configuration is handled safely by this driver.
    */

    if (!rtc.begin(Wire, true, false, false))
    {
        Serial.println("RV3028 init failed");
        return false;
    }

    if (!ensureBackupConfiguration())
    {
        Serial.println("RV3028 backup configuration failed");
        return false;
    }

    return true;
}


// ------------------------------------------------------------
// Read current time
// ------------------------------------------------------------

DateTime ClockRV3028::now()
{
    rtc.updateTime();

    return DateTime(
        2000 + rtc.getYear(),
        rtc.getMonth(),
        rtc.getDate(),
        rtc.getHours(),
        rtc.getMinutes(),
        rtc.getSeconds()
    );
}


// ------------------------------------------------------------
// Set current time
// ------------------------------------------------------------

void ClockRV3028::adjust(const DateTime& dt)
{
    rtc.setTime(
        dt.second(),
        dt.minute(),
        dt.hour(),
        dt.dayOfTheWeek(),
        dt.day(),
        dt.month(),
        dt.year() - 2000
    );
}