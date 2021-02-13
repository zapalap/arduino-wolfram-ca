#include "VehicleController.h"
#include "FrameContext.h"
#include "Controller.h"

VehicleController::VehicleController()
{
    x = 16;
    y = 8;
}

void VehicleController::enter(FrameContext frameContext)
{
    clearMatrix(frameContext);
}

FrameContext VehicleController::update(FrameContext frameContext)
{
    drawVehicle(frameContext);
    handleJoy(frameContext.joyState);

    FrameContext updatedContext(frameContext.matrix, x * 100 + y, frameContext.encoderState, frameContext.joyState);
    return updatedContext;
}

void VehicleController::handleJoy(JoyState joyState)
{
    if (joyState.x > 580)
    {
        x--;
        x = limit(x, 0, 31);
    }

    if (joyState.x < 370)
    {
        x++;
        x = limit(x, 0, 31);
    }

    if (joyState.y > 580)
    {
        y--;
        y = limit(y, 0, 15);
    }

    if (joyState.y < 370)
    {
        y++;
        y = limit(y, 0, 15);
    }
}

void VehicleController::drawVehicle(FrameContext frameContext)
{
    clearMatrix(frameContext);
    frameContext.matrix[y][x] = 1;
}

void VehicleController::clearMatrix(FrameContext frameContext)
{
    for (byte x = 0; x < 32; x++)
    {
        for (byte y = 0; y < 16; y++)
        {
            frameContext.matrix[y][x] = 0;
        }
    }
}

char VehicleController::limit(char value, char min, char max)
{
    if (value >= max)
    {
        value = min;
        return value;
    }

    if (value < min)
    {
        value = max;
        return value;
    }

    return value;
}