#include "Mover.h"
#include "DotMover.h"
#include "Vector.h"
#include "FrameContext.h"

DotMover::DotMover()
{
}

void DotMover::update(FrameContext frameContext)
{
    velocity.add(acceleration);
    location.add(velocity);
    velocity.limit(0.8);

    if (location.x > 31)
    {
        location.x = 0;
    };

    if (location.x < 0)
    {
        location.x = 31;
    };

    if (location.y > 16)
    {
        location.y = 0;
    };

    if (location.y < 0)
    {
        location.y = 15;
    };
}

void DotMover::draw(FrameContext frameContext)
{
    if ((location.x < 31 && location.x > 1) && (location.y < 15 && location.y > 1))
    {
        frameContext.matrix[round(location.y)][round(location.x)] = 1;
    }
}

void DotMover::applyForce(const Vector &force)
{
    acceleration.add(force);
}