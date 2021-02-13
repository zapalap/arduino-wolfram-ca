#ifndef VEHICLE_H
#define VEHICLE_H

#include "Controller.h"
#include "FrameContext.h"

class VehicleController : public Controller
{
public:
    VehicleController();
    FrameContext update(FrameContext frameContext);
    void enter(FrameContext frameContext);

private:
    char x;
    char y;
    void handleJoy(JoyState joyState);
    void drawVehicle(FrameContext frameContext);
    char limit(char value, char min, char max);
    void clearMatrix(FrameContext frameContext);
};

#endif