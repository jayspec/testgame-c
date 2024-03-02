#include <stdint.h>
#include "globals.h"

#define SHIP_START_X 150
#define SHIP_START_Y 100

long g_joystick_state;

int16_t g_shipXPos;
int16_t g_shipYPos;

uint8_t g_numLiveShots;

shot_t g_liveShots[MAX_SHOTS];

void global_init() {
    g_joystick_state = 0;

    g_shipXPos = SHIP_START_X;
    g_shipYPos = SHIP_START_Y;

    g_numLiveShots = 0;
}