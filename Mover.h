#ifndef MOVER_H
#define MOVER_H

#include "Vector.h"
#include "FrameContext.h"

class Mover
{
public:
    Vector *location;
    Vector *velocity;
    Vector *acceleration;
    virtual void update(FrameContext frameContext);
    virtual void draw(FrameContext frameContext);
    virtual void applyForce(Vector force);
};

#endif