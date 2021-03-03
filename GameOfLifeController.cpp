#include "GameOfLifeController.h"
#include "FrameContext.h"

#define BOUND_X 32
#define BOUND_Y 16

GameOfLifeController::GameOfLifeController()
{
    generationNumber = 0;
    initialize();
}

FrameContext GameOfLifeController::update(FrameContext frameContext)
{
    handleJoy(frameContext.joyState);

    createNewGenerationAndUpdateMatrix(frameContext);
    generationNumber++;

    if (generationNumber > 9999)
    {
        generationNumber = 0;
    }

    FrameContext updatedContext(frameContext.matrix, generationNumber, frameContext.encoderState, frameContext.joyState, frameContext.selectButtonState);
    return updatedContext;
}

void GameOfLifeController::createNewGenerationAndUpdateMatrix(FrameContext frameContext)
{
    bool nextGen[BOUND_Y][BOUND_X];
    for (byte x = 0; x < BOUND_X; x++)
    {
        for (byte y = 0; y < BOUND_Y; y++)
        {
            nextGen[y][x] = resolveRule(currentGen[y][x], countAliveNeighbors(x, y));
        }
    }

    for (byte x = 0; x < BOUND_X; x++)
    {
        for (byte y = 0; y < BOUND_Y; y++)
        {
            currentGen[y][x] = nextGen[y][x];
            frameContext.matrix[y][x] = nextGen[y][x];
        }
    }
}

void GameOfLifeController::initialize()
{
    randomSeed(analogRead(A5));
    generationNumber = 0;
    byte c = random(6);
    for (byte x = 0; x < BOUND_X; x++)
    {
        for (byte y = 0; y < BOUND_Y; y++)
        {
            bool value = x % random(10) == 0 ? 1 : 0;
            currentGen[y][x] = value;
        }
    }
}

byte GameOfLifeController::countAliveNeighbors(byte x, byte y)
{
    byte alive = 0;

    for (int nx = -1; nx <= 1; nx++)
    {
        for (int ny = -1; ny <= 1; ny++)
        {
            int dx = x + nx;
            int dy = y + ny;

            if (dx > BOUND_X - 1)
            {
                dx = 0;
            }

            if (dx < 0)
            {
                dx = BOUND_X - 1;
            }

            if (dy > BOUND_Y - 1)
            {
                dy = 0;
            }

            if (dy < 0)
            {
                dy = BOUND_Y - 1;
            }

            alive += currentGen[dy][dx];
        }
    }

    alive -= currentGen[y][x];

    return alive;
}

byte GameOfLifeController::resolveRule(byte currentStatus, byte aliveNeighbors)
{
    if (currentStatus == 0 && aliveNeighbors == 6 || aliveNeighbors == 3)
    {
        return 1;
    }

    if (currentStatus == 1 && (aliveNeighbors == 2 || aliveNeighbors == 3))
    {
        return 1;
    }

    return 0;
}

void GameOfLifeController::handleEncoder(int encoderState)
{
    if (encoderState != 0)
    {
        initialize();
    }
}

void GameOfLifeController::handleJoy(JoyState joyState)
{
    if (joyState.x > 580 || joyState.x < 370 || joyState.y > 580 || joyState.y < 370)
    {
        initialize();
    }
}