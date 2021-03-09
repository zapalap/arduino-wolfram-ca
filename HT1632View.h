#ifndef HT1632_VIEW_H
#define HT1632_VIEW_H

#include "View.h"
#include "FrameContext.h"
#include <TM1637Display.h>

#define TM1637_CLK 10
#define TM1637_DIO 9

#define HT1632_DATA 3
#define HT1632_WR 4
#define HT1632_CS1 5
#define HT1632_CS2 6

struct Coordinates
{
    byte x;
    byte y;
};

class HT1632View : public View
{
public:
    HT1632View(byte boundX, byte boundY);
    void render(FrameContext frameContext);

private:
    void renderMatrixSlice(FrameContext frameContext, int xStart, int xEnd, int xOffset);
    Coordinates translate(byte x, byte y);
    float boundX;
    float boundY;
    TM1637Display numberDisplay;
};

#endif
