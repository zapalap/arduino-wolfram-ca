#include <font_5x4.h>
#include <font_8x4.h>
#include <font_8x5.h>
#include <HT1632.h>
#include <images.h>

#define MATRIX_BOUNDX 16
#define MATRIX_BOUNDY 16

const byte DATA = 3;
const byte WR = 4;
const byte CS = 5;

long frame = 0;

byte physicalLedMap[MATRIX_BOUNDX][MATRIX_BOUNDY] = {
    {0, 1, 2, 3, 4, 5, 6, 7},
    {15, 14, 13, 12, 11, 10, 9, 8}};

byte cells[MATRIX_BOUNDX][MATRIX_BOUNDY];

byte simpleCA[MATRIX_BOUNDX] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0};

byte rule[8] = {0, 0, 1, 1, 1, 1, 0, 0};
byte neighborConfigurations[8][3] = {{1, 1, 1}, {1, 1, 0}, {1, 0, 1}, {1, 0, 0}, {0, 1, 1}, {0, 1, 0}, {0, 0, 1}, {0, 0, 0}};

byte translationMatrix[16][16][2] = {
    {{7, 8}, {6, 8}, {5, 8}, {4, 8}, {3, 8}, {2, 8}, {1, 8}, {0, 8}, {7, 0}, {6, 0}, {5, 0}, {4, 0}, {3, 0}, {2, 0}, {1, 0}, {0, 0}},
    {{7, 9}, {6, 9}, {5, 9}, {4, 9}, {3, 9}, {2, 9}, {1, 9}, {0, 9}, {7, 1}, {6, 1}, {5, 1}, {4, 1}, {3, 1}, {2, 1}, {1, 1}, {0, 1}},
    {{7, 10}, {6, 10}, {5, 10}, {4, 10}, {3, 10}, {2, 10}, {1, 10}, {0, 10}, {7, 2}, {6, 2}, {5, 2}, {4, 2}, {3, 2}, {2, 2}, {1, 2}, {0, 2}},
    {{7, 11}, {6, 11}, {5, 11}, {4, 11}, {3, 11}, {2, 11}, {1, 11}, {0, 11}, {7, 3}, {6, 3}, {5, 3}, {4, 3}, {3, 3}, {2, 3}, {1, 3}, {0, 3}},
    {{7, 12}, {6, 12}, {5, 12}, {4, 12}, {3, 12}, {2, 12}, {1, 12}, {0, 12}, {7, 4}, {6, 4}, {5, 4}, {4, 4}, {3, 4}, {2, 4}, {1, 4}, {0, 4}},
    {{7, 13}, {6, 13}, {5, 13}, {4, 13}, {3, 13}, {2, 13}, {1, 13}, {0, 13}, {7, 5}, {6, 5}, {5, 5}, {4, 5}, {3, 5}, {2, 5}, {1, 5}, {0, 5}},
    {{7, 14}, {6, 14}, {5, 14}, {4, 14}, {3, 14}, {2, 14}, {1, 14}, {0, 14}, {7, 6}, {6, 6}, {5, 6}, {4, 6}, {3, 6}, {2, 6}, {1, 6}, {0, 6}},
    {{7, 15}, {6, 15}, {5, 15}, {4, 15}, {3, 15}, {2, 15}, {1, 15}, {0, 15}, {7, 7}, {6, 7}, {5, 7}, {4, 7}, {3, 7}, {2, 7}, {1, 7}, {0, 7}},
    {{15, 8}, {14, 8}, {13, 8}, {12, 8}, {11, 8}, {10, 8}, {9, 8}, {8, 8}, {15, 0}, {14, 0}, {13, 0}, {12, 0}, {11, 0}, {10, 0}, {9, 0}, {8, 0}},
    {{15, 9}, {14, 9}, {13, 9}, {12, 9}, {11, 9}, {10, 9}, {9, 9}, {8, 9}, {15, 1}, {14, 1}, {13, 1}, {12, 1}, {11, 1}, {10, 1}, {9, 1}, {8, 1}},
    {{15, 11}, {14, 11}, {13, 11}, {12, 11}, {11, 11}, {10, 11}, {9, 11}, {8, 11}, {15, 2}, {14, 2}, {13, 2}, {12, 2}, {11, 2}, {10, 2}, {9, 2}, {8, 2}},
    {{15, 10}, {14, 10}, {13, 10}, {12, 10}, {11, 10}, {10, 10}, {9, 10}, {8, 10}, {15, 3}, {14, 3}, {13, 3}, {12, 3}, {11, 3}, {10, 3}, {9, 3}, {8, 3}},
    {{15, 12}, {14, 12}, {13, 12}, {12, 12}, {11, 12}, {10, 12}, {9, 12}, {8, 12}, {15, 4}, {14, 4}, {13, 4}, {12, 4}, {11, 4}, {10, 4}, {9, 4}, {8, 4}},
    {{15, 13}, {14, 13}, {13, 13}, {12, 13}, {11, 13}, {10, 13}, {9, 13}, {8, 13}, {15, 5}, {14, 5}, {13, 5}, {12, 5}, {11, 5}, {10, 5}, {9, 5}, {8, 5}},
    {{15, 14}, {14, 14}, {13, 14}, {12, 14}, {11, 14}, {10, 14}, {9, 14}, {8, 14}, {15, 6}, {14, 6}, {13, 6}, {12, 6}, {11, 6}, {10, 6}, {9, 6}, {8, 6}},
    {{15, 15}, {14, 15}, {13, 15}, {12, 15}, {11, 15}, {10, 15}, {9, 15}, {8, 15}, {15, 7}, {14, 7}, {13, 7}, {12, 7}, {11, 7}, {10, 7}, {9, 7}, {8, 7}},
};

struct inputData
{
    long frame;
} data;

void setup()
{
    Serial.begin(115200);
    HT1632.begin(CS, WR, DATA);
    for (size_t x = 0; x < MATRIX_BOUNDX; x++)
    {
        for (size_t y = 0; y < MATRIX_BOUNDY; y++)
        {
            cells[y][x] = 0;
        }
    }
}

void update(inputData data)
{
    byte newCA[MATRIX_BOUNDX] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    for (size_t i = 0; i < MATRIX_BOUNDX; i++)
    {
        byte neighbors[3];
        neighbors[0] = i - 1 > 0 ? simpleCA[i - 1] : simpleCA[15];
        neighbors[1] = simpleCA[i];
        neighbors[2] = i + 1 < 8 ? simpleCA[i + 1] : simpleCA[0];

        newCA[i] = resolveRule(neighbors);
    }

    for (size_t i = 0; i < MATRIX_BOUNDX; i++)
    {
        cells[MATRIX_BOUNDY - 1][i] = newCA[i];
        simpleCA[i] = newCA[i];
    }

    for (size_t y = 0; y < MATRIX_BOUNDY; y++)
    {
        if (y + 1 < MATRIX_BOUNDY)
        {
            for (size_t i = 0; i < MATRIX_BOUNDX; i++)
            {
                cells[y][i] = cells[y + 1][i];
            }
        }
    }
}

byte resolveRule(byte neighbors[3])
{
    for (size_t i = 0; i < 8; i++)
    {
        if (neighborConfigurations[i][0] == neighbors[0] && neighborConfigurations[i][1] == neighbors[1] && neighborConfigurations[i][2] == neighbors[2])
        {
            return rule[i];
        }
    }

    return 0;
}

void render(inputData data)
{
    for (size_t x = 0; x < MATRIX_BOUNDX; x++)
    {
        for (size_t y = 0; y < MATRIX_BOUNDY; y++)
        {
            byte translatedx = translationMatrix[y][x][0];
            byte translatedy = translationMatrix[y][x][1];

            if (cells[y][x] == 1)
            {
                HT1632.setPixel(translatedx, translatedy);
            }
            else
            {
                HT1632.clearPixel(translatedx, translatedy);
            }
        }
    }

    HT1632.render();
}

void loop()
{
    data.frame = frame;

    update(data);
    render(data);
    frame++;

    delay(250);
}
