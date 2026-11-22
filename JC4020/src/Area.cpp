#include "Area.h"


void Area::clearArea()
{
  display->fillRect(x, y, width, high, background);
  if(showScope)
  {
    display->drawRect(x, y, width, high, BLUE);
  }
}

void Area::addText(String* str, Text &txt)
{
  uint16_t txtLen = calculateTextLength(str, txt);
  uint16_t areaTextWidth = width-2*txt.marginX; // text place inside area
  int16_t cursorPosition = x+txt.marginX; // aligned to LEFT
  if(txtLen < areaTextWidth) 
  { 
    if(txt.align == CENTER) { cursorPosition += (areaTextWidth-txtLen)/2; }
    if(txt.align == RIGHT)  { cursorPosition += (areaTextWidth-txtLen); }
  }
  display->setTextColor(txt.color);
  display->setCursor(cursorPosition, y+txt.marginY);
  display->print(*str);
}
bool Area::textFits(String* str, Text &txt)
{
  uint16_t txtLen = calculateTextLength(str, txt);
  uint16_t areaTextWidth = width-2*txt.marginX;
  return txtLen <= areaTextWidth;
}
uint16_t Area::calculateTextLength(String* str, Text &txt)
{
  int16_t x1, y1;
  uint16_t w, h;
  display->setFont(txt.font);
  display->setTextWrap(false);
  display->getTextBounds(str->c_str(), x+txt.marginX, y+txt.marginY, &x1, &y1, &w, &h);
  display->setTextWrap(true);
  return w;
}

void Area::addText(const char* str, Text &txt)
{
  uint16_t txtLen = calculateTextLength(str, txt);
  uint16_t areaTextWidth = width-2*txt.marginX; // text place inside area
  int16_t cursorPosition = x+txt.marginX; // aligned to LEFT
  if(txtLen < areaTextWidth) 
  { 
    if(txt.align == CENTER) { cursorPosition += (areaTextWidth-txtLen)/2; }
    if(txt.align == RIGHT)  { cursorPosition += (areaTextWidth-txtLen); }
  }
  display->setTextColor(txt.color);
  display->setCursor(cursorPosition, y+txt.marginY);
  display->print(str);
}
uint16_t Area::calculateTextLength(const char* str, Text &txt)
{
  int16_t x1, y1;
  uint16_t w, h;
  display->setFont(txt.font);
  display->setTextWrap(false);
  display->getTextBounds(str, x+txt.marginX, y+txt.marginY, &x1, &y1, &w, &h);
  display->setTextWrap(true);
  return w;
}
bool Area::textFits(const char* str, Text &txt)
{
  uint16_t txtLen = calculateTextLength(str, txt);
  uint16_t areaTextWidth = width-2*txt.marginX;
  return txtLen <= areaTextWidth;
}

void Area::addText(int str, Text &txt)
{
  char buffer[10];
  sprintf(buffer, "%d", str);
  this->addText(buffer, txt);
}

void Area::addRect(bool fill, int16_t x0, int16_t y0, int16_t w, int16_t h, uint16_t color)
{
  if (fill)
    display->fillRect(x0, y0, w, h, color);
  else
    display->drawRect(x0, y0, w, h, color);
}
void Area::addCircle(bool fill, int16_t x0, int16_t y0, int16_t radius, uint16_t color)
{
  if (fill)
    display->fillCircle(x0, y0, radius, color);
  else
    display->drawCircle(x0, y0, radius, color);
}
void Area::addRoundRect(bool fill, int16_t x0, int16_t y0, int16_t w, int16_t h, int16_t radius, uint16_t color)
{
  if (fill)
    display->fillRoundRect(x0, y0, w, h, radius, color);
  else
    display->drawRoundRect(x0, y0, w, h, radius, color);
}
void Area::addRoundRectBorder(uint16_t color)
{    
  display->drawRoundRect(this->x+1, this->y, this->width-2, this->high, 3, color);
}
void Area::addRectBorder()
{    
  display->drawRect(this->x, this->y, this->width, this->high, BLUE);
}
void Area::addTriangle(bool fill, int16_t x0, int16_t y0, int16_t x1, int16_t y1,int16_t x2, int16_t y2, uint16_t color)
{
  if (fill)
    display->fillTriangle(x0, y0, x1, y1, x2, y2, color);
  else
    display->drawTriangle(x0, y0, x1, y1, x2, y2, color);
}
void Area::addArrows()
{
    uint16_t center = x + width/2;
    display->fillTriangle(center-2, y-1, center, y-3, center+2, y-1 , YELLOW);
    display->fillTriangle(center-2, high+y+1, center, high+y+3, center+2, high+y+1 , YELLOW);
}