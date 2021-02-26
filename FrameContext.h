#ifndef FRAME_CONTEXT_H
#define FRAME_CONTEXT_H

#include <arduino.h>

struct JoyState
{
    int x;
    int y;
};

class FrameContext
{
public:
    FrameContext(byte (*matrixPtr)[32], int segmentDisplayState, int encoder, JoyState joy, byte selectButton);
    byte (*matrix)[32];
    int encoderState;
    JoyState joyState;
    int segmentDisplayState;
    byte selectButtonState;
};

#endif
