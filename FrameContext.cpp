#include "FrameContext.h"

FrameContext::FrameContext(byte (*matrixPtr)[32], int segment, int encoder, JoyState joy)
{
    matrix = matrixPtr;
    encoderState = encoder;
    joyState = joy;
    segmentDisplayState = segment;
};