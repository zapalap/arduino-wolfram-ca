#include "FrameContext.h"

FrameContext::FrameContext(byte (*matrixPtr)[32], int segment, int encoder, JoyState joy, byte selectButton)
{
    matrix = matrixPtr;
    encoderState = encoder;
    joyState = joy;
    segmentDisplayState = segment;
    selectButtonState = selectButton;
};