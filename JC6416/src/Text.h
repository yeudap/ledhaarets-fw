#ifndef TEXT_H
#define TEXT_H

// https://tchapi.github.io/Adafruit-GFX-Font-Customiser/

#include "Colors.h"
#include "gfxfont.h"
#include "Fonts/font5.h"
#include "Fonts/font6.h"
#include "Fonts/font7.h"
#include "Fonts/font8.h"
#include "Fonts/font9.h"
#include "Fonts/font18.h"
#include "Fonts/logo.h"
#include "Fonts/number19.h"
#include "Fonts/number22.h"
#include "Fonts/number23.h"
#include "Fonts/number23d.h"
#include "Fonts/number25.h"
#include "Fonts/number25d.h"
#include "Fonts/ariel10.h"
#include "../../../../.pio\libdeps\esp32dev\Adafruit GFX Library\Fonts\FreeSansBold9pt7b.h"

enum Align {LEFT, CENTER, RIGHT};
const int numOfFonts = 5;

class Text
{
public:

    int color;
    const GFXfont* font;
    uint16_t marginX;
    uint16_t marginY;
    Align align;
    Text() {}
    Text(int _color, const GFXfont* _font, uint16_t _marginX, uint16_t _marginY, Align _align=LEFT) : color(_color), font(_font), marginX(_marginX), marginY(_marginY), align(_align) {}
};


#endif