#ifndef VIEW_H
#define VIEW_H

#include "FrameContext.h"

class View
{
public:
    virtual void render(FrameContext frameContext);
};

#endif