#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "globals.h"


typedef struct {
    int16_t xPos;
    int16_t yPos;
    bool isActive;
    direction direction;
    uint8_t collisionMask;
} enemy_t;

extern void init_enemies();
extern void handle_enemies();
extern enemy_t *get_enemy_at_index(uint8_t index);