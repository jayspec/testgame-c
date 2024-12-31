#pragma once

#include "globals.h"

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    int16_t xPos;
    int16_t yPos;
    bool isActive;
    direction direction;
} shot_t;

extern void init_shots();
extern void handle_shots();
extern shot_t *get_shot_at_index(uint8_t index);