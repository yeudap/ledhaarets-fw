#include "Bluetooth.h"
#include "CoreService.h"
#include "FirmwareService.h"
#include "AuthManager.h"
#include <Display.h>
#include "Preferences/UserPreferences.h"
#include "Logic.h"

extern Logic           bl;
extern State*          currentState;
extern UserPreferences userPreferences;

bool deviceConnected = false;

class MyServerCallbacks : public NimBLEServerCallbacks
{
    void onConnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo) override
    {
        deviceConnected = true;
        Serial.println("Device connected");

        // AUTH: כל חיבור מתחיל לא-מאומת — חגורת ביטחון בנוסף לאיפוס בניתוק
        authManager.resetSession();

        Serial.printf("MTU on connect: %u\n", connInfo.getMTU());
        onOtaMtuChanged(connInfo.getMTU());

        onOtaConnect();
    }

    void onDisconnect(NimBLEServer* pServer, NimBLEConnInfo& connInfo, int reason) override
    {
        deviceConnected = false;
        Serial.printf("Device disconnected, reason=%d\n", reason);

        // AUTH: דגל האימות הוא per-connection — מתאפס בכל ניתוק
        authManager.resetSession();

        onOtaDisconnect();

        NimBLEDevice::startAdvertising();
        State* lastState = currentState;
        currentState = &bl.updateState;
        messageScreen.printMessages(messageScreen.INFO, "& ", "...vhmehkptvn ewbwn", 1000);
        currentState = lastState;
        currentState->showStateScreen();
    }

    void onMTUChange(uint16_t mtu, NimBLEConnInfo& connInfo) override
    {
        Serial.printf("MTU changed to: %u\n", mtu);
        onOtaMtuChanged(mtu);
    }
};

void initBluetooth()
{
    Serial.println("init Bluetooth start");

    // AUTH: טעינת PIN ו-trusted slots מ-NVS — לפני הקמת השירותים
    authManager.init();

    NimBLEDevice::init("LedHaarets");
    NimBLEDevice::setMTU(517);
    NimBLEServer* pServer = NimBLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks());

    initCoreService(pServer);
    initFirmwareService(pServer);
    NimBLEAdvertising* pAdvertising = NimBLEDevice::getAdvertising();
    char combinedData[Metadata::combinedData_len];
    Metadata::combineManufacturerData(combinedData);

    NimBLEAdvertisementData advData;
    advData.setManufacturerData(std::string(combinedData, Metadata::combinedData_len));
    pAdvertising->setAdvertisementData(advData);

    // שם המכשיר ב-scan response — ה-advertisement הראשי מלא (31 בייט)
    // עם ה-manufacturer data, ולכן השם משודר בחבילה הנפרדת.
    // השם נטען מ-NVS (ברירת מחדל: SYS_DEFAULT_DEVICE_NAME) וניתן לשינוי
    // מהאפליקציה דרך SYS_OP_SET_NAME.
    applyDeviceName(loadDeviceName());

    pAdvertising->addServiceUUID(CORE_SERVICE_UUID);
    pAdvertising->addServiceUUID(OTA_SERVICE_UUID);
    Serial.println("Advertising start...");

    NimBLEDevice::startAdvertising();
    Serial.println("Advertising started");
    Serial.println("init Bluetooth finish");
}

void bleApplyState(bool enable)
{
    if (enable)
    {
        BLEDevice::getAdvertising()->start();
    }
    else
    {
        BLEDevice::getAdvertising()->stop();
    }
}