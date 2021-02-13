#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "FrameContext.h"

class Controller
{
public:
    virtual FrameContext update(FrameContext frameContext) = 0;
};

#endif