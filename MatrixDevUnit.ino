#include <Encoder.h>
#include <arduino-timer.h>

#include "FrameContext.h"
#include "Controller.h"

#include "GameOfLifeController.h"
#include "CellularAutomataController.h"
#include "VehicleController.h"
#include "GameOfLifeController.h"
#include "LanderController.h"

#include "View.h"
#include "HT1632View.h"

#define MATRIX_BOUND_X 32
#define MATRIX_BOUND_Y 16

#define ENCODER_DT 8
#define ENCODER_CLK 2

#define JOY_Y A0
#define JOY_X A1

#define SELECT_BUTTON 11
#define MAX_CONTROLLERS 3

Encoder encoder(ENCODER_CLK, ENCODER_DT);
byte matrix[16][32];
byte (*matrixPtr)[32] = matrix;

Timer<1, millis> selectDebounceTimer;

HT1632View view(MATRIX_BOUND_X, MATRIX_BOUND_Y);

CellularAutomataController caController;
VehicleController vehicleController;
GameOfLifeController gofController;
LanderController landerController;

Controller *controllers[MAX_CONTROLLERS] = {
    &vehicleController,
    &caController,
    &gofController};

char currentController = 0;
bool selectCheckPendning;
bool selectPushPending;

void setup()
{
    Serial.begin(115200);

    pinMode(JOY_X, INPUT);
    pinMode(JOY_Y, INPUT);
    pinMode(SELECT_BUTTON, INPUT_PULLUP);

    for (int x = 0; x < 32; x++)
    {
        for (int y = 0; y < 16; y++)
        {
            matrix[y][x] = 0;
        }
    }
}

void render(FrameContext frameContext)
{
    view.render(frameContext);
}

void loop()
{
    selectDebounceTimer.tick();

    JoyState joyState;
    joyState.x = analogRead(JOY_X);
    joyState.y = analogRead(JOY_Y);

    int encoderValue = encoder.read();

    FrameContext frameContext(matrixPtr, 0, encoderValue, joyState, HIGH);

    handleSelectButton(frameContext);

    if (!(encoderValue > 1 || encoderValue < -1))
    {
        encoderValue = 0;
    }
    else
    {
        encoder.write(0);
    }

    FrameContext updatedFrameContext = controllers[currentController]->update(frameContext);
    render(updatedFrameContext);

    delay(20);
}

void handleSelectButton(FrameContext frameContext)
{
    byte selectButtonState = digitalRead(SELECT_BUTTON);

    if (!selectCheckPendning && selectButtonState == LOW)
    {
        selectCheckPendning = true;
        selectDebounceTimer.in(100, ensureSelectPushed);
    }

    if (selectPushPending)
    {
        currentController++;

        if (currentController > MAX_CONTROLLERS - 1)
        {
            currentController = 0;
        }

        controllers[currentController]->enter(frameContext);
        selectPushPending = false;
    }
}

bool ensureSelectPushed()
{
    int switchState = digitalRead(SELECT_BUTTON);

    if (switchState == LOW)
    {
        selectPushPending = true;
    }

    selectCheckPendning = false;

    return true;
}
