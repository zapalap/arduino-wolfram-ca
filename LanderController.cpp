#include "LanderController.h"
#include "Vector.h"
#include "Controller.h"
#include "FrameContext.h"

LanderController::LanderController()
{
    lander = new DotMover(*(new Vector(16, 8)));
    followRandomPoint();
}

void LanderController::enter(FrameContext frameContext)
{
}

FrameContext LanderController::update(FrameContext frameContext)
{
    handleJoy(frameContext.joyState);
    clearMatrix(frameContext);

    lander->update(frameContext);
    lander->draw(frameContext);

    FrameContext updatedContext(frameContext.matrix, round(lander->location->x) * 100 + round(lander->location->y), frameContext.encoderState, frameContext.joyState, frameContext.selectButtonState);
    return updatedContext;
}

void LanderController::serialPrintVector(String name, Vector *vector)
{
    Serial.println(name + ": (x: " + String(vector->x) + ", y: " + String(vector->y) + ")");
}

void LanderController::handleJoy(JoyState joyState)
{
    if (joyState.x > 580 || joyState.x < 370 || joyState.y > 580 || joyState.y < 370)
    {
        followRandomPoint();
    }
}

void LanderController::followRandomPoint()
{
    byte rx = random(0, 31);
    byte ry = random(0, 15);

    Vector *attractor = new Vector(rx, ry);
    Vector force = lander->location->subtract(*attractor);
    force.normalize();
    force.mult(0.3);
    lander->applyForce(force);
}

void LanderController::clearMatrix(FrameContext frameContext)
{
    for (byte x = 0; x < 32; x++)
    {
        for (byte y = 0; y < 16; y++)
        {
            frameContext.matrix[y][x] = 0;
        }
    }
}