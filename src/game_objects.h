#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "globals.h"

#define MAX_GAME_OBJECTS MAX_SHOTS + MAX_ENEMIES + 1
#define SHIP_OBJ_INDEX 0

#define SHOT_FIRST_OBJ_INDEX SHIP_OBJ_INDEX + 1
#define SHOT_LAST_OBJ_INDEX SHIP_OBJ_INDEX + MAX_SHOTS

#define ENEMY_FIRST_OBJ_INDEX SHOT_LAST_OBJ_INDEX + 1
#define ENEMY_LAST_OBJ_INDEX SHOT_LAST_OBJ_INDEX + MAX_ENEMIES

typedef struct {
    int16_t xPos[MAX_GAME_OBJECTS];
    int16_t yPos[MAX_GAME_OBJECTS];
    bool isActive[MAX_GAME_OBJECTS];
    direction direction[MAX_GAME_OBJECTS];
    uint8_t collisionMask[MAX_GAME_OBJECTS];
} game_object_collection;

extern game_object_collection game_objects;