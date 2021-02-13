#ifndef GAME_OF_LIFE_H
#define GAME_OF_LIFE_H

#include "Controller.h"
#include "FrameContext.h"

class GameOfLifeController : public Controller
{
public:
    GameOfLifeController();
    FrameContext update(FrameContext frameContext);

private:
    int generationNumber;
    byte currentGen[16][32];
    void initialize();
    byte countAliveNeighbors(byte x, byte y);
    byte resolveRule(byte currentStatus, byte aliveNeighbors);
    void handleEncoder(int encoderState);
    void handleJoy(JoyState joyState);
    void createNewGenerationAndUpdateMatrix(FrameContext frameContext);
};

#endif