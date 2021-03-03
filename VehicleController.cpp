#include "VehicleController.h"
#include "FrameContext.h"
#include "Controller.h"

const byte VehicleController::shipSprite[5][5] = {
    {1, 1, 0, 0, 0},
    {1, 1, 1, 0, 0},
    {0, 0, 1, 1, 0},
    {1, 1, 1, 0, 0},
    {1, 1, 0, 0, 0}};

const byte VehicleController::redSunSprite[5][5] = {
    {0, 1, 1, 0, 0},
    {1, 1, 1, 1, 0},
    {1, 1, 1, 1, 0},
    {0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0}};

VehicleController::VehicleController()
{
    x = 6;
    y = 12;
    blackSunx = 27;
    blackSuny = 13;
    redSunx = 27;
    redSuny = 14;
    frame = 0;
    sunStep = 1;
}

void VehicleController::enter(FrameContext frameContext)
{
    clearMatrix(frameContext);
    for (char x = 0; x < 32; x++)
    {
        terrain[x] = 0;
    }
}

FrameContext VehicleController::update(FrameContext frameContext)
{
    frame++;

    drawVehicle(frameContext);
    generateTerain(frameContext);
    handleJoy(frameContext.joyState);

    FrameContext updatedContext(frameContext.matrix, x * 100 + y, frameContext.encoderState, frameContext.joyState, frameContext.selectButtonState);
    return updatedContext;
}

void VehicleController::handleJoy(JoyState joyState)
{
    if (joyState.x > 580)
    {
        x--;
        x = limit(x, 0, 20);
    }

    if (joyState.x < 370)
    {
        x++;
        x = limit(x, 0, 20);
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

void VehicleController::renderSprite(char sprx, char spry, const byte (*spritePtr)[5], FrameContext frameContext)
{
    for (char sx = 0; sx < 5; sx++)
    {
        for (char sy = 0; sy < 5; sy++)
        {
            frameContext.matrix[limit(spry - sy, 1, 15)][limit(sprx + sx, 0, 31)] = spritePtr[sy][sx];
        }
    }
}

void VehicleController::drawVehicle(FrameContext frameContext)
{
    clearMatrix(frameContext);
    renderSprite(x, y, shipSprite, frameContext);
}

byte VehicleController::getRandomHeight()
{

    int dieCast = random(0, 100);

    if (dieCast <= 1)
    {
        return random(4, 8);
    }

    if (dieCast <= 2)
    {
        return random(4, 10);
    }

    if (dieCast <= 65)
    {
        return random(2, 6);
    }

    if (dieCast <= 85)
    {
        return random(2, 4);
    }

    return random(0, 3);
}

void VehicleController::generateTerain(FrameContext frameContext)
{
    byte nextHeight = getRandomHeight();

    for (char x = 0; x < 31; x++)
    {
        terrain[x] = terrain[x + 1];
    }

    terrain[31] = nextHeight;

    for (char x = 0; x < 32; x++)
    {
        for (char y = 0; y <= terrain[x]; y++)
        {
            frameContext.matrix[y][x] = 2;
        }
    }

    renderSprite(redSunx, redSuny, redSunSprite, frameContext);
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
    if (value > max)
    {
        value = max;
        return value;
    }

    if (value < min)
    {
        value = min;
        return value;
    }

    return value;
}