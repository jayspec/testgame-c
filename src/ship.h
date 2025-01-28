#pragma once
#include "globals.h"

#include <stdint.h>

typedef struct ship_state {
    int16_t xPos;
    int16_t yPos;
    direction direction;
    uint8_t collisionMask;
} ship_state;

extern void init_ship();
extern void move_player_ship();
extern ship_state *get_ship_state();
