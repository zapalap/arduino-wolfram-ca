#include "FrameContext.h"
#include "HT1632View.h"
#include <HT1632.h>

const byte PROGMEM translationMatrix[16][16][2] = {
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

HT1632View::HT1632View(byte boundX, byte boundY) : numberDisplay(TM1637_CLK, TM1637_DIO)
{
    this->boundX = boundX;
    this->boundY = boundY;
    HT1632.begin(HT1632_CS1, HT1632_CS2, HT1632_WR, HT1632_DATA);
    HT1632.renderTarget(1);
    HT1632.setBrightness(16);
    HT1632.renderTarget(0);
    HT1632.setBrightness(16);
    numberDisplay.setBrightness(5, true);
}

void HT1632View::render(FrameContext frameContext)
{
    numberDisplay.showNumberDec(frameContext.segmentDisplayState, true);
    HT1632.renderTarget(0);
    renderMatrixSlice(frameContext, 0, boundX / 2, 0);
    HT1632.render();

    HT1632.renderTarget(1);
    renderMatrixSlice(frameContext, boundX / 2, boundX, -16);
    HT1632.render();
}

void HT1632View::renderMatrixSlice(FrameContext frameContext, int xStart, int xEnd, int xOffset)
{
    for (byte x = xStart; x < xEnd; x++)
    {
        for (byte y = 0; y < boundY; y++)
        {
            Coordinates translated = translate(x + xOffset, y);
            // 9 marks a transparent pixel
            if (frameContext.matrix[y][x] == 9)
            {
                continue;
            }

            if (frameContext.matrix[y][x] > 0)
            {
                HT1632.setPixel(translated.x, translated.y);
            }
            else
            {
                HT1632.clearPixel(translated.x, translated.y);
            }
        }
    }
}

Coordinates HT1632View::translate(byte x, byte y)
{
    Coordinates translated;
    translated.x = pgm_read_byte(&(translationMatrix[x][y][0]));
    translated.y = pgm_read_byte(&(translationMatrix[x][y][1]));

    return translated;
}
