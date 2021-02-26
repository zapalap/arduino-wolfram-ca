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
    char blackSunx;
    char blackSuny;
    char redSunx;
    char redSuny;
    char sunStep;
    long frame;
    byte terrain[32];
    void handleJoy(JoyState joyState);
    void drawVehicle(FrameContext frameContext);
    char limit(char value, char min, char max);
    void clearMatrix(FrameContext frameContext);
    void generateTerain(FrameContext frameContext);
    byte getRandomHeight();
    void renderSprite(char x, char y, const byte (*spritePtr)[5], FrameContext frameContext);

    static const byte shipSprite[5][5];
    static const byte redSunSprite[5][5];
};

#endif