#include "Display.h"
#include "esp_heap_caps.h"

MatrixPanel_I2S_DMA *dma_display;
VirtualMatrixPanel_T<PANEL_CHAIN_TYPE>* display;

void initDisplay(){
    HUB75_I2S_CFG mxconfig(
    PANEL_RES_X,   // module width
    PANEL_RES_Y,   // module height
    PANEL_CHAIN_LEN
    );
    mxconfig.gpio.e = 18;               
    mxconfig.clkphase = false;
    mxconfig.double_buff = false;
    mxconfig.latch_blanking = 4;
    mxconfig.i2sspeed = HUB75_I2S_CFG::HZ_8M;    
    mxconfig.setPixelColorDepthBits(3);
    mxconfig.min_refresh_rate = 60;
    Serial.printf("ColorDepthBits=%u\n", mxconfig.getPixelColorDepthBits());

    dma_display = new MatrixPanel_I2S_DMA(mxconfig);
    if (!dma_display->begin())
        Serial.println("****** ERROR! I2S memory allocation failed ***********");
    dma_display->setBrightness8(128);
    dma_display->clearScreen();
    display = new VirtualMatrixPanel_T<PANEL_CHAIN_TYPE>(VDISP_NUM_ROWS, VDISP_NUM_COLS, PANEL_RES_X, PANEL_RES_Y);
    display->setDisplay(*dma_display);
    Serial.printf("Calculated refresh rate: %d Hz\n", dma_display->calculated_refresh_rate);
    Serial.printf("Free DMA heap: %u\n", heap_caps_get_free_size(MALLOC_CAP_DMA));
    Serial.printf("Largest DMA block: %u\n", heap_caps_get_largest_free_block(MALLOC_CAP_DMA));

}
static void dumpMem(const char* tag){
  Serial.printf("[%s] FreeDMA=%u LargestDMA=%u Free=%u Largest=%u\n",
    tag,
    heap_caps_get_free_size(MALLOC_CAP_DMA),
    heap_caps_get_largest_free_block(MALLOC_CAP_DMA),
    ESP.getFreeHeap(),
    heap_caps_get_largest_free_block(MALLOC_CAP_8BIT)
  );
}
