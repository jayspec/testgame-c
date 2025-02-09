#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "globals.h"
#include <cx16.h>

#include "game_objects.h"
#include "enemy.h"

#ifdef DEBUG
#include "debug.h"
#include <stdio.h>
#endif

static uint8_t nextEnemyCountdown = ENEMY_DELAY;
static uint8_t numEnemies;
static unsigned int random_seed;

void advance_enemy_at_index(uint8_t attr_index);
bool enemy_is_off_screen(int16_t x, int16_t y);
void create_enemy();

void init_enemies() {
    random_seed = cx16_k_entropy_get();
    srand(random_seed);
    numEnemies = 0;
}

void handle_enemies() {

    for (uint8_t i = 0; i < MAX_ENEMIES; i++) {
        advance_enemy_at_index(i);
    }

    if (nextEnemyCountdown > 0) {
        nextEnemyCountdown--;
    } else {
        create_enemy();
    }

}

void advance_enemy_at_index(uint8_t enemy_index) {
    uint8_t enemy_object_index = ENEMY_FIRST_OBJ_INDEX + enemy_index;

    if (game_objects.isActive[enemy_object_index]) {

        switch(game_objects.direction[enemy_object_index]) {
            case RIGHT:
                game_objects.xPos[enemy_object_index] += ENEMY_SPEED;
                break;
            case LEFT:
                game_objects.xPos[enemy_object_index] -= ENEMY_SPEED;
                break;
            case DOWN:
                game_objects.yPos[enemy_object_index] += ENEMY_SPEED;
                break;
            case UP:
                game_objects.yPos[enemy_object_index] -= ENEMY_SPEED;
                break;
            default:
                // do nothing
                break;
        }

        if (game_objects.isDestroyed[enemy_object_index] || enemy_is_off_screen(game_objects.xPos[enemy_object_index], game_objects.yPos[enemy_object_index])) {
            game_objects.isActive[enemy_object_index] = false;
            game_objects.isDestroyed[enemy_object_index] = false;
            numEnemies--;
        }
    }
}

bool enemy_is_off_screen(int16_t x, int16_t y) {
    return (x > MAX_X || x < MIN_X || y > MAX_Y || y < MIN_Y);
}

void create_enemy() {
    if (nextEnemyCountdown > 0 || numEnemies >= MAX_ENEMIES) {
        return;
    }
    nextEnemyCountdown = ENEMY_DELAY;

    int8_t enemy_index_to_create = -1;
    for (uint8_t i = ENEMY_FIRST_OBJ_INDEX; i <= ENEMY_LAST_OBJ_INDEX; i++) {
        if (!game_objects.isActive[i]) {
            numEnemies++;
            enemy_index_to_create = i;
            break;
        }
    }

    if (enemy_index_to_create == -1) {
        return;
    }

    // Using abs() to fix bug in library?  rand() returns negative numbers (I don't think there's an overflow...)
    direction dir = (abs(rand()) % 4);
    game_objects.direction[enemy_index_to_create] = dir;
    game_objects.isActive[enemy_index_to_create] = true;
    game_objects.collisionMask[enemy_index_to_create] = 0b0011;

    switch(dir) {
        case UP:
            game_objects.xPos[enemy_index_to_create] = MAX_X / 2;
            game_objects.yPos[enemy_index_to_create] = MAX_Y - 1;
            break;
        case DOWN:
            game_objects.xPos[enemy_index_to_create] = MAX_X / 2;
            game_objects.yPos[enemy_index_to_create] = MIN_Y + 1;
            break;
        case LEFT:
            game_objects.xPos[enemy_index_to_create] = MAX_X - 1;
            game_objects.yPos[enemy_index_to_create] = MAX_Y / 2;
            break;
        case RIGHT:
            game_objects.xPos[enemy_index_to_create] = MIN_X + 1;
            game_objects.yPos[enemy_index_to_create] = MAX_Y / 2;
            break;
        default:
            // do nothing
            break;
    }
}

