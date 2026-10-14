#ifndef AREA_H
#define AREA_H 


#include "Text.h"
#include "Display.h"

class Area
{
    public:
        // bool showScope = true;
        bool showScope = false;
        int16_t x;
        int16_t y;
        int16_t width;
        int16_t high;
        uint16_t background;
        Area(int16_t _x, int16_t _y, int16_t _width, int16_t _high, uint16_t _background) : x(_x), y(_y), width(_width), high(_high), background(_background) {}
        void clearArea();

        void addText(const char* str, Text &txt);
        uint16_t calculateTextLength(const char* str, Text &txt);
        bool textFits(const char* str, Text &txt);

        void addText(String* str, Text &txt);
        uint16_t calculateTextLength(String* str, Text &txt);
        bool textFits(String* str, Text &txt);

        void addText(int str, Text &txt);
        
        void addRect(bool fill, int16_t x0, int16_t y0, int16_t w, int16_t h, uint16_t color);
        void addCircle(bool fill, int16_t x0, int16_t y0, int16_t r, uint16_t color);
        void addRoundRect(bool fill, int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color);
        void addTriangle(bool fill, int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color);
        void addRoundRectBorder(uint16_t color = YELLOW);
        void addRectBorder();
        void addArrows();
};

#endif