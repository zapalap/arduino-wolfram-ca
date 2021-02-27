#ifndef LANDER_H
#define LANDER_H

#include "Controller.h"
#include "FrameContext.h"
#include "Vector.h"
#include "DotMover.h"
#include "Mover.h"

class LanderController : public Controller
{
public:
    LanderController();
    FrameContext update(FrameContext frameContext);
    void enter(FrameContext frameContext);

private:
    DotMover *lander;

    void handleJoy(JoyState joyState);
    void serialPrintVector(String name, Vector *vector);
    void clearMatrix(FrameContext frameContext);
    void followRandomPoint();
};

#endif