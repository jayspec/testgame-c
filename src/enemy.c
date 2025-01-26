#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

#include "globals.h"
#include <cx16.h>

#include "enemy.h"

#ifdef DEBUG
#include "debug.h"
#endif

static enemy_t enemies[MAX_ENEMIES];

static uint8_t nextEnemyCountdown = ENEMY_DELAY;
static uint8_t numEnemies;
static unsigned long random_seed;

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

enemy_t *get_enemy_at_index(uint8_t index) {
    return &enemies[index];
}

void advance_enemy_at_index(uint8_t attr_index) {
    enemy_t *enemy = &enemies[attr_index];

    if (enemy->isActive) {

        if (enemy->direction == RIGHT) {
            enemy->xPos += ENEMY_SPEED;
        } else if (enemy->direction == LEFT) {
            enemy->xPos -= ENEMY_SPEED;
        } else if (enemy->direction == DOWN) {
            enemy->yPos += ENEMY_SPEED;
        } else if (enemy->direction == UP) {
            enemy->yPos -= ENEMY_SPEED;
        }

        if (enemy_is_off_screen(enemy->xPos, enemy->yPos)) {
            enemy->isActive = false;
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
    numEnemies++;

    enemy_t *enemy = NULL;
    for (uint8_t i = 0; i < MAX_ENEMIES; i++) {
        if (!enemies[i].isActive) {
            enemy = &enemies[i];
            break;
        }
    }

    if (enemy == NULL) {
        return;
    }

    direction dir = (rand() % 4);
    enemy->direction = dir;
    enemy->isActive = true;

    switch(dir) {
        case UP:
            enemy->xPos = MAX_X / 2;
            enemy->yPos = MAX_Y - 1;
            break;
        case DOWN:
            enemy->xPos = MAX_X / 2;
            enemy->yPos = MIN_Y + 1;
            break;
        case LEFT:
            enemy->xPos = MAX_X - 1;
            enemy->yPos = MAX_Y / 2;
            break;
        case RIGHT:
            enemy->xPos = MIN_X + 1;
            enemy->yPos = MAX_Y / 2;
            break;
        default:
            // do nothing
            break;
    }
}

