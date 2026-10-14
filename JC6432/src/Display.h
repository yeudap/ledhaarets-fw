#ifndef Display_H
#define Display_H

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>
#include <ESP32-HUB75-VirtualMatrixPanel_T.hpp>

// Configure for your panel(s) as appropriate!
#define VDISP_NUM_ROWS 2
#define VDISP_NUM_COLS 2
#define PANEL_RES_X 128
#define PANEL_RES_Y 64
#define PANEL_CHAIN_LEN     (VDISP_NUM_ROWS*VDISP_NUM_COLS)

#define PANEL_CHAIN_TYPE CHAIN_BOTTOM_RIGHT_UP_ZZ
// #define PANEL_CHAIN_TYPE CHAIN_TOP_RIGHT_DOWN_ZZ

extern MatrixPanel_I2S_DMA *dma_display;
extern VirtualMatrixPanel_T<PANEL_CHAIN_TYPE>* display;

void initDisplay();

#endif