#ifndef BLUETOOTH_H
#define BLUETOOTH_H 

#include <Arduino.h>
#include <NimBLEDevice.h>
#include "Metadata.h"
#include "Commands.h"

extern bool deviceConnected; 

void initBluetooth();
void bleApplyState(bool);
String loadDeviceName();
void applyDeviceName(const String& name);

#endif