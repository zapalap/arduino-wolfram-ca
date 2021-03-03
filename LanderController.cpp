#include "LanderController.h"
#include "Vector.h"
#include "Controller.h"
#include "FrameContext.h"
#include "DotMover.h"

LanderController::LanderController()
{
    randomSeed(analogRead(A5));
    Serial.println("Creating lander");
    lander.location.x = 16;
    lander.location.y = 8;

    Serial.println("Creating attractor");
    attractor.location.x = random(0, 31);
    attractor.location.y = random(0, 15);

    Serial.println("Following...");
    followRandomPoint();
}

void LanderController::enter(FrameContext frameContext)
{
}

FrameContext LanderController::update(FrameContext frameContext)
{
    handleJoy(frameContext.joyState);
    clearMatrix(frameContext);

    lander.update(frameContext);
    serialPrintVector("Vel", lander.velocity);
    serialPrintVector("Acc", lander.acceleration);
    serialPrintVector("Loc", lander.location);
    drawMovers(frameContext);

    FrameContext updatedContext(frameContext.matrix, round(attractor.location.x) * 100 + round(attractor.location.y), frameContext.encoderState, frameContext.joyState, frameContext.selectButtonState);
    return updatedContext;
}

void LanderController::serialPrintVector(String name, const Vector &vector)
{
    Serial.println(name + ": (x: " + String(vector.x) + ", y: " + String(vector.y) + ")");
}

void LanderController::handleJoy(JoyState joyState)
{
    if (joyState.x > 580 || joyState.x < 370 || joyState.y > 580 || joyState.y < 370)
    {
        followRandomPoint();
    }
}

void LanderController::drawMovers(FrameContext frameContext)
{
    Serial.println("Drawing lander");
    lander.draw(frameContext);

    Serial.println("Drawing attractor");
    attractor.draw(frameContext);
}

void LanderController::followRandomPoint()
{
    Serial.println("Relocating attractor");

    attractor.location.x = random(0, 31);
    attractor.location.y = random(0, 15);

    Serial.println("Calculating follow force");
    Vector force = lander.location.subtract(attractor.location);
    force.normalize();
    force.mult(0.3);

    Serial.println("Calculating follow force");
    lander.applyForce(force);
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