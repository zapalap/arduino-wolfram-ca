#ifndef CELLULAR_CA_CONTROLLER_H
#define CELLULAR_CA_CONTROLLER_H

#include <arduino.h>
#include "Controller.h"
#include "FrameContext.h"

class CellularAutomataController : public Controller
{
public:
    CellularAutomataController();
    FrameContext update(FrameContext frameContext);

private:
    byte rule;
    byte currentGen[32];
    byte nextGen[32];
    byte applyRule(byte neighbors[3]);
    void resetCurrentGen();
    void handleEncoder(int encoderValue);
    void handleJoy(JoyState joyState);
    void createNextGeneration();
    void updateMatrix(byte (*matrixPtr)[32]);
};

#endif
