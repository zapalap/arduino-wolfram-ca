#include "CellularAutomataController.h"
#include "FrameContext.h"
#include "Controller.h"

#define BOUND_X 32
#define BOUND_Y 16

const byte possibleNeighbors[8][3] = {{1, 1, 1}, {1, 1, 0}, {1, 0, 1}, {1, 0, 0}, {0, 1, 1}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0}};

CellularAutomataController::CellularAutomataController()
{
    rule = 89;
    resetCurrentGen();
};

FrameContext CellularAutomataController::update(FrameContext frameContext)
{
    handleEncoder(frameContext.encoderState);
    handleJoy(frameContext.joyState);

    createNextGeneration();
    updateMatrix(frameContext.matrix);

    frameContext.segmentDisplayState = rule;

    FrameContext updatedCtx(frameContext.matrix, rule, frameContext.encoderState, frameContext.joyState, frameContext.selectButtonState);

    return updatedCtx;
};

void CellularAutomataController::createNextGeneration()
{
    for (int i = 1; i < BOUND_X - 1; i++)
    {
        byte neighbors[3] = {0, 0, 0};
        neighbors[0] = currentGen[i - 1];
        neighbors[1] = currentGen[i];
        neighbors[2] = currentGen[i + 1];

        nextGen[i] = applyRule(neighbors);
    }
};

void CellularAutomataController::updateMatrix(byte (*matrixPtr)[32])
{
    for (byte i = 0; i < BOUND_X; i++)
    {
        matrixPtr[0][i] = nextGen[i];
        currentGen[i] = nextGen[i];
    }

    for (int y = BOUND_Y - 1; y - 1 >= 0; y--)
    {
        for (int i = 0; i < BOUND_X; i++)
        {
            matrixPtr[y][i] = matrixPtr[y - 1][i];
        }
    }
};

void CellularAutomataController::handleEncoder(int encoderValue)
{
    if (encoderValue > 1)
    {
        resetCurrentGen();
        rule--;

        if (rule < 0)
        {
            rule = 254;
        }
    };

    if (encoderValue < -1)
    {
        resetCurrentGen();
        rule++;

        if (rule > 254)
        {
            rule = 0;
        }
    };
};

void CellularAutomataController::handleJoy(JoyState joyState)
{
    if (joyState.x > 580)
    {
        rule--;
    }

    if (joyState.x < 370)
    {
        rule++;
    }

    if (joyState.y > 580)
    {
        rule -= 10;
    }

    if (joyState.y < 370)
    {
        rule += 10;
    }
}

byte CellularAutomataController::applyRule(byte neighbors[3])
{
    byte ruleArray[8];
    byte x = 7;

    for (int mask = 0x80; mask != 0; mask >>= 1)
    {
        (rule & mask) ? ruleArray[x] = 1 : ruleArray[x] = 0;
        x--;
    }

    for (byte i = 0; i < 8; i++)
    {
        if (possibleNeighbors[i][0] == neighbors[0] && possibleNeighbors[i][1] == neighbors[1] && possibleNeighbors[i][2] == neighbors[2])
        {
            return ruleArray[i];
        }
    }

    return 0;
};

void CellularAutomataController::resetCurrentGen()
{
    for (byte i = 0; i < BOUND_X; i++)
    {
        currentGen[i] = 0;
    }
    currentGen[BOUND_X / 2] = 1;
}