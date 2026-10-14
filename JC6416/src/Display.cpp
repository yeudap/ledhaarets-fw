#include "Display.h"


void initDisplay(){
    HUB75_I2S_CFG mxconfig;
    mxconfig.mx_height = PANEL_HEIGHT;
    mxconfig.chain_length = PANELS_NUMBER;
    mxconfig.gpio.e = PIN_E;               
    mxconfig.latch_blanking = 4;
    mxconfig.clkphase = false;
    mxconfig.i2sspeed = HUB75_I2S_CFG::HZ_10M;    
    display = new MatrixPanel_I2S_DMA(mxconfig);
    // Allocate memory and start DMA display
    if (not display->begin())
        Serial.println("****** ERROR! I2S memory allocation failed ***********");
}