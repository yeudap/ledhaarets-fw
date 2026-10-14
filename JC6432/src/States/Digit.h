#ifndef DIGIT_H
#define DIGIT_H

#include <Arduino.h>
#include "Colors.h"
#include "Display.h"

class Coordinate {
  public:
    uint16_t x;
    uint16_t y;
    Coordinate(){}
    Coordinate(uint16_t _x, uint16_t _y) : x(_x), y(_y){}
};
class Digit {
  
  public:
    Digit() {}
    Digit(uint16_t xo, uint16_t yo);
    int value;
    int newValue;
    Coordinate p1;
    Coordinate p2;
    Coordinate p3;
    Coordinate p4;
    Coordinate p5;
    Coordinate p6;

    void drawPixel(uint16_t x, uint16_t y, int c);
    void drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, int c);
    void Morph(int i, uint8_t curr, int c);
    void Morph01(int i, int c);
    void Morph12(int i, int c);
    void Morph23(int i, int c);
    void Morph34(int i, int c);
    void Morph45(int i, int c);
    void Morph56(int i, int c);
    void Morph67(int i, int c);
    void Morph78(int i, int c);
    void Morph89(int i, int c);
    void Morph90(int i, int c);
    void Morph10(int i, int c);
    void Morph20(int i, int c);
    void Morph30(int i, int c);
    void Morph50(int i, int c);
};

#endif