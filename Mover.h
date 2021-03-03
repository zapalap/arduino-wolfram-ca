#ifndef MOVER_H
#define MOVER_H

#include "Vector.h"
#include "FrameContext.h"

class Mover
{
public:
    virtual void update(FrameContext frameContext);
    virtual void draw(FrameContext frameContext);
    virtual void applyForce(const Vector &force);
};

#endif