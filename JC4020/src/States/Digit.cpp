#include "Digit.h"

const int segHeight = 8;
const int segWidth = segHeight;
const int segDiff = 11;
const int animSpeed = 30;

Digit::Digit(uint16_t xo, uint16_t yo)
{
  value = -1;
  p1 = Coordinate(xo,           yo);
  p2 = Coordinate(xo + segDiff, yo);
  p3 = Coordinate(xo,           yo + segDiff);
  p4 = Coordinate(xo + segDiff, yo + segDiff);
  p5 = Coordinate(xo,           yo + 2*segDiff);
  p6 = Coordinate(xo + segDiff, yo + 2*segDiff);
}

void Digit::drawPixel(uint16_t x, uint16_t y, int c)
{
  display->drawPixel(x,   y,   c);
  display->drawPixel(x,   y+1, c);
  display->drawPixel(x,   y-1, c);
  display->drawPixel(x+1, y,   c);
  display->drawPixel(x-1, y,   c);
}
void Digit::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, int c)
{
  if(x1 == x2) // vertical
  {
    display->drawLine(x1,   y1+1, x2,   y2-1, c);
    display->drawLine(x1-1, y1+2, x2-1, y2-2, c);
    display->drawLine(x1+1, y1+2, x2+1, y2-2, c);
  }
  if(y1 == y2) // horizonal
  {
    display->drawLine(x1+1, y1,   x2-1, y2,   c);
    display->drawLine(x1+2, y1-1, x2-2, y2-1, c);
    display->drawLine(x1+2, y1+1, x2-2, y2+1, c);
  }
}
void MorphMulty(Digit* h0, Digit* h1, Digit* m0, Digit* m1, Digit* s0, Digit* s1, int c)
{
  if(h0->value != h0->newValue) {
    for (int i = 0; i <= segDiff; i++)
    {
      h0->Morph(i, h0->newValue, c);
      h1->Morph(i, 0, c);
      m0->Morph(i, 0, c);
      m1->Morph(i, 0, c);
      s0->Morph(i, 0, c);
      s1->Morph(i, 0, c);
      delay(animSpeed);
    }
  }
  else if(h1->value != h1->newValue)
  {
    for (int i = 0; i <= segDiff; i++)
    {
      h1->Morph(i, h1->newValue, c);
      m0->Morph(i, 0, c);
      m1->Morph(i, 0, c);
      s0->Morph(i, 0, c);
      s1->Morph(i, 0, c);
      delay(animSpeed);
    }
  }
  else if(m0->value != m0->newValue)
  {
    for (int i = 0; i <= segDiff; i++)
    {
      m0->Morph(i, m0->newValue, c);
      m1->Morph(i, 0, c);
      s0->Morph(i, 0, c);
      s1->Morph(i, 0, c);
      delay(animSpeed);
    }
  }
  else if(m1->value != m1->newValue)
  {
    for (int i = 0; i <= segDiff; i++)
    {
      m1->Morph(i, m1->newValue, c);
      s0->Morph(i, 0, c);
      s1->Morph(i, 0, c);
      delay(animSpeed);
    }
  }
  else if(s0->value != s0->newValue)
  {
    for (int i = 0; i <= segDiff; i++)
    {
      s0->Morph(i, s0->newValue, c);
      s1->Morph(i, 0, c);
      delay(animSpeed);
    }
  }
  else if(s1->value != s1->newValue)
  {
    for (int i = 0; i <= segDiff; i++)
    {
      s1->Morph(i, s1->newValue, c);
      delay(animSpeed);
    }
  }
  h0->value = h0->newValue;
  h1->value = h1->newValue;
  m0->value = m0->newValue;
  m1->value = m1->newValue;
  s0->value = s0->newValue;
  s1->value = s1->newValue;
}
void Digit::Morph(int i, uint8_t newValue, int c)
{
  switch (newValue) {
    case 1: Morph01(i, c); break; // or Morph21
    case 2: Morph12(i, c); break;
    case 3: Morph23(i, c); break;
    case 4: Morph34(i, c); break;
    case 5: Morph45(i, c); break;
    case 6: Morph56(i, c); break;
    case 7: Morph67(i, c); break;
    case 8: Morph78(i, c); break;
    case 9: Morph89(i, c); break;
    case 0:
      switch (this->value) {
        case 1: Morph10(i, c); break;
        case 2: Morph20(i, c); break;
        case 3: Morph30(i, c); break;
        case 5: Morph50(i, c); break;
        case 9: Morph90(i, c); break;
      } break;
  }
}
void Digit::Morph01(int i ,int c)
{
  // Gradually Erase A, G, D
  if(i>1 && i<=segDiff-1)
  {
    drawPixel(i + p1.x, p1.y, BLACK); // A
    drawPixel(i + p3.x, p3.y, BLACK); // G
    drawPixel(i + p5.x, p5.y, BLACK); // D
  }

  // Move F left to right
  drawLine(i + p1.x-1, p1.y, i + p3.x-1, p3.y, BLACK);
  drawLine(i + p1.x,   p1.y, i + p3.x  , p3.y, c);
  
  // Move E left to right
  drawLine(i + p3.x-1, p3.y, i + p5.x-1, p5.y, BLACK);
  drawLine(i + p3.x,   p3.y, i + p5.x,   p5.y, c);
}
void Digit::Morph12(int i ,int c)
{
  // Move E right to left
  drawLine(p4.x+1 - i, p4.y, p6.x+1 - i, p6.y, BLACK);
  drawLine(p4.x - i,   p4.y, p6.x - i,   p6.y, c);

  // Gradually Draw A, G, D
  if(i>2 && i<segDiff)
  {
    drawPixel(p2.x -i+1, p2.y, c); // A
    drawPixel(p4.x -i+1, p4.y, c); // G
    drawPixel(p6.x -i+1, p6.y, c); // D
  }
}
void Digit::Morph23(int i ,int c)
{
  // Move E left to right
  drawLine(i + p3.x-1, p3.y, i + p5.x-1, p5.y, BLACK);
  drawLine(i + p3.x,   p3.y, i + p5.x,   p5.y, c);
  drawLine(p3.x, p3.y, p4.x, p4.y, c); // keep G
  drawLine(p5.x, p5.y, p6.x, p6.y, c); // keep D
}
void Digit::Morph34(int i ,int c)
{
  if(i>1 && i<segDiff-1)
  {
    drawPixel(p2.x - i, p2.y, BLACK); // Erase A
    drawPixel(p5.x + i, p5.y, BLACK); // Erase D
    drawPixel(p1.x, p1.y + i, c); // Draw F
  }
}
void Digit::Morph45(int i ,int c)
{
  if(i>1 && i<segDiff-1)
  {
    drawPixel(p4.x, p4.y - i, BLACK); // Erase B
    drawPixel(p2.x - i, p2.y, c); // Draw A
    drawPixel(p6.x - i, p6.y, c); // Draw D
  }
}
void Digit::Morph56(int i ,int c)
{
  // copy E right to left
  drawLine(p4.x+1 - i, p4.y, p6.x+1 - i, p6.y, BLACK);
  drawLine(p4.x,       p4.y, p6.x,       p6.y, c); // keep C
  drawLine(p3.x,       p3.y, p4.x,       p4.y, c); // keep G
  drawLine(p5.x,       p5.y, p6.x,       p6.y, c); // keep D
  drawLine(p4.x - i,   p4.y, p6.x - i,   p6.y, c);
}
void Digit::Morph67(int i ,int c)
{
  // Move F left to right
  drawLine(i + p1.x-1, p1.y, i + p3.x-1, p3.y, BLACK);
  drawLine(i + p1.x,   p1.y, i + p3.x  , p3.y, c);
  
  // Move E left to right
  drawLine(i + p3.x-1, p3.y, i + p5.x-1, p5.y, BLACK);
  drawLine(i + p3.x,   p3.y, i + p5.x,   p5.y, c);
  
  drawLine(p1.x, p1.y, p2.x, p2.y, c); // keep A

  // Erase D and G gradually
  if(i>1 && i<segDiff-1)
  {
    drawPixel(p3.x + i, p3.y, BLACK); // G
    drawPixel(p5.x + i, p5.y, BLACK); // D
  }
}
void Digit::Morph78(int i ,int c)
{
  // Copy B, C right to left
  drawLine(p2.x+1 - i, p2.y, p4.x+1 - i, p4.y, BLACK);
  drawLine(p2.x - i,   p2.y, p4.x - i,   p4.y, c);
  drawLine(p2.x, p2.y, p4.x, p4.y, c);
  
  drawLine(p4.x+1 - i, p4.y, p6.x+1 - i, p6.y, BLACK);
  drawLine(p4.x - i,   p4.y, p6.x - i,   p6.y, c);
  drawLine(p4.x, p4.y, p6.x, p6.y, c);

  drawLine(p1.x, p1.y, p2.x, p2.y, c); // keep A

  // Gradually Draw G, D
  if(i>2 && i<segDiff)
  {
    drawPixel(p4.x - i+1, p4.y, c); // G
    drawPixel(p6.x - i+1, p6.y, c); // D
  }
}
void Digit::Morph89(int i ,int c)
{
  // Move E left to right
  drawLine(i + p3.x-1, p3.y, i + p5.x-1, p5.y, BLACK);
  drawLine(i + p3.x,   p3.y, i + p5.x,   p5.y, c);
  drawLine(p3.x, p3.y, p4.x, p4.y, c); // keep G
  drawLine(p5.x, p5.y, p6.x, p6.y, c); // keep D
}
void Digit::Morph10(int i ,int c)
{
  // Copy B, C right to left
  drawLine(p2.x+1 - i, p2.y, p4.x+1 - i, p4.y, BLACK);
  drawLine(p2.x - i,   p2.y, p4.x - i,   p4.y, c);
  drawLine(p2.x,   p2.y, p4.x,   p4.y, c);
  
  drawLine(p4.x+1 - i, p4.y, p6.x+1 - i, p6.y, BLACK);
  drawLine(p4.x - i,   p4.y, p6.x - i,   p6.y, c);
  drawLine(p4.x,   p4.y, p6.x,   p6.y, c);

  // Gradually Draw A, D
  if(i>2 && i<segDiff)
  {
    drawPixel(p2.x - i+1, p2.y, c); // A
    drawPixel(p6.x - i+1, p6.y, c); // D
  }
}
void Digit::Morph20(int i ,int c)
{
  if(i>2 && i<segDiff)
  {
    drawPixel(p3.x+5 - (i-3)/2, p3.y, BLACK); // Erase G
    drawPixel(p3.x+6 + (i-3)/2, p3.y, BLACK); // Erase G
    drawPixel(p3.x, p3.y - i+1, c); // Draw F
    drawPixel(p4.x, p4.y + i-1, c); // Draw C
  }
}
void Digit::Morph30(int i ,int c)
{
  if(i>1 && i<segDiff-1)
  {
    drawPixel(p4.x - i, p4.y, BLACK); // Erase G
    drawPixel(p3.x, p3.y - i, c); // Draw F
    drawPixel(p3.x, p3.y + i, c); // Draw E
  }
}
void Digit::Morph50(int i ,int c)
{
  if(i>2 && i<segDiff)
  {
    drawPixel(p3.x+5 - (i-3)/2, p3.y, BLACK); // Erase G
    drawPixel(p3.x+6 + (i-3)/2, p3.y, BLACK); // Erase G
    drawPixel(p4.x, p4.y - i+1, c); // Draw F
    drawPixel(p3.x, p3.y + i-1, c); // Draw C
  }
}
void Digit::Morph90(int i ,int c)
{
  if(i>1 && i<segDiff-1)
  {
    drawPixel(p4.x - i, p4.y, BLACK); // Erase G
    drawPixel(p3.x, p3.y + i, c); // Draw E
  }
}
