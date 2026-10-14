#ifndef CORE_SERVICE_H
#define CORE_SERVICE_H

#include "Bluetooth.h"
#include "CoreProtocol.h"


#define CORE_SERVICE_UUID      "7b79e39b-488a-4f76-a60f-bd3a29da174f" 
#define CORE_CMD_CHAR_UUID     "326109ee-bcea-4809-88b1-5e0da1289a8c" 
#define CORE_EVT_CHAR_UUID     "fbe9b56e-270b-4765-a3ec-b917bb0ad0e1" 

void initCoreService(NimBLEServer* pServer);

// שליחת EVT — זמין גם לקריאה חיצונית אם נדרש בעתיד
void coreSendEvt(uint8_t domain, uint8_t opcode, uint8_t status,
                 const uint8_t* data = nullptr, size_t dataLen = 0);

#endif // CORE_SERVICE_H