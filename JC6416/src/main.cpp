#include "Hardware.h"
#include "DB/DB.h"
#include "Logic.h"

extern ButtonCommand currentButtonCommand;
extern MenuCommand currentMenuCommand;
extern int long_push_temp;
extern Button menuButton;
extern Button upButton;
extern Button downButton;
extern Button okButton;

State* currentState;
Strings strings;
Logic bl;

void setup()
{
  delay(1000);
  Serial.begin(115200);
  delay(100);
  Serial.printf("\nLEDHAARETS %s v%s %s\n", DEVICE_MODEL, PROJECT_VER, HARDWARE_VERSION);
  initDisplay();  
  strings.setStringsHeb();
  logoScreen.printScreen();
  setupPreferences();
  int i = userPreferences.brightness;
  for (i; i > 0; i--)
  {
    display->setBrightness8(i);
    delay(5);
  }
  for (i = 1; i < 255; i++)
  {
    display->setBrightness8(i);
    delay(5);
  }
  display->setBrightness8(userPreferences.brightness);
  configureHardware(HARDWARE_MODEL);
  validate_rtc();
  validate_time();
  initPlace();
  bl.init();
  if (okButton.isPushed())
  {
    Serial.println("force_reset");
    currentState = &bl.setResetPrefState;
    currentState->showStateScreen();
  }
  else
  {
    currentState = &bl.mainState;
    bl.mainState.setNewDay();
    currentState->showStateScreen();
  }
  Serial.println("setup done.");
  Serial.print("SWversion: ");
  Serial.println(Metadata::SWversion);
  if(userPreferences.bt == 1) initBluetooth();
  // userPreferences.printAllValues();
  sleep(1);
  // dumpMem("Main");
}

void loop()
{
  currentState->stateLoop();
  if (okButton.isPushed() && menuButton.isPushed())
  {
    if(long_push_temp==long_push_button)
    {
      currentState = &bl.menuConfgState;
      currentState->showStateScreen();
    }
    long_push_temp++;
  }
  else if (okButton.isPushed() && downButton.isPushed())
  {
    if(long_push_temp==long_push_button)
    {
      currentState = &bl.viewMemoryMapState;
      currentState->showStateScreen();
    }
    long_push_temp++;
  }
  else if (upButton.isPushed() && downButton.isPushed())
  {
    if(long_push_temp==long_push_button)
    {
      messageScreen.printMessages(messageScreen.INFO, "! ck oha", "hbsh  kujwt  gmcn  iugav");
      ESP.restart();
    }
    long_push_temp++;
  }
  else{
    if (menuButton.isPushed() && currentState->menuPushed() != nullptr)
    {
      currentState = currentState->menuState();
      currentState->showStateScreen();
    }
    if (upButton.isPushed() && currentState->upPushed() != nullptr)
    {
      currentState = currentState->upState();
      currentState->showStateScreen();
    }
    if (downButton.isPushed() && currentState->downPushed() != nullptr)
    {
      currentState = currentState->downState();
      currentState->showStateScreen();
    }
    if (okButton.isPushed() && currentState->okPushed() != nullptr)
    {
      currentState = currentState->okState();
      currentState->showStateScreen();
    }
    if (currentMenuCommand != MenuCommand::NONE)
    {
      currentState = bl.goToMenuCommand(currentMenuCommand);
      currentState->showStateScreen();
    }
    long_push_temp = 0;
  }
  currentButtonCommand= ButtonCommand::NONE;
  currentMenuCommand= MenuCommand::NONE;
  delay(delay_button_time);
}
