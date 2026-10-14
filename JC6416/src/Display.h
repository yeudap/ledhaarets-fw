#ifndef Display_H
#define Display_H

#include <ESP32-HUB75-MatrixPanel-I2S-DMA.h>

#define PANEL_WIDTH 128
#define PANEL_HEIGHT 64
#define PANELS_NUMBER 2
#define PANEL_COLOR_DEPTH 3
#define PIN_E 18

#define PANE_WIDTH (PANEL_WIDTH * PANELS_NUMBER)
#define PANE_HEIGHT (PANEL_HEIGHT)

extern MatrixPanel_I2S_DMA * display;

void initDisplay();

#endif