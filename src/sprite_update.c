#include "enemy.h"
#include "globals.h"

#include <stdint.h>
#include <cx16.h>

#include "shot.h"
#include "sprites.h"
#include "ship.h"
#include "enemy.h"

#define V_FLIP 0x02
#define H_FLIP 0x01

typedef enum { SHIP_VERTICAL, SHIP_HORIZONTAL, SHOT_VERTICAL, SHOT_HORIZONTAL, ENEMY_VERTICAL, ENEMY_HORIZONTAL } sprite_type;

void update_sprites() {
    VERA.address = SPRITE_ATTR_BASE & 0xffff;
    VERA.address_hi = ((SPRITE_ATTR_BASE >> 16) | VERA_INC_1); // Hi byte of sprite 1 position address, stride 1

    uint8_t discard;
    uint8_t zDepth;
    uint8_t flip;
    uint8_t sprite_num;

    ship_state *ship = get_ship_state();

    if (ship->direction == LEFT || ship->direction == RIGHT) {
        sprite_num = SHIP_HORIZONTAL;
    } else {
        sprite_num = SHIP_VERTICAL;
    }

    /* Update the ship position */
    // Set the address in VRAM where the ship sprite is located
    VERA.data0 = ((SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * sprite_num)) >> 5);
    VERA.data0 = ((SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * sprite_num)) >> 13); // Bit 8 = Mode 0 (4bpp, 16 color)

    VERA.data0 = ship->xPos;
    VERA.data0 = ship->xPos >> 8;
    VERA.data0 = ship->yPos;
    VERA.data0 = ship->yPos >> 8;

    if (ship->direction == LEFT) {
        flip = H_FLIP;
    } else if (ship->direction == DOWN) {
        flip = V_FLIP;
    } else {
        flip = 0;
    }

    VERA.data0 = ((ship->collisionMask << 4) | (SHIP_Z_DEPTH << 2) | flip);

    // skip the last byte: height width, and palette offset
    VERA.data0;

    /* Update all the shots */

    for (int i = 0; i < MAX_SHOTS; i++) {
        shot_t* shot = get_shot_at_index(i);

        if (shot->direction == LEFT || shot->direction == RIGHT) {
            sprite_num = SHOT_HORIZONTAL;
        } else {
            sprite_num = SHOT_VERTICAL;
        }

        // Set the address in VRAM where the shot sprite is located
        VERA.data0 = ((SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * sprite_num)) >> 5);
        VERA.data0 = ((SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * sprite_num)) >> 13); // Bit 8 = Mode 0 (4bpp, 16 color)

        if (shot->isActive) {
            VERA.data0 = shot->xPos;
            VERA.data0 = (shot->xPos >> 8);
            VERA.data0 = shot->yPos;
            VERA.data0 = (shot->yPos >> 8);

            zDepth = 3;
            flip = 0;
            if (shot->direction == LEFT) {
                flip = H_FLIP;
            } else if (shot->direction == DOWN) {
                flip = V_FLIP;
            }

            VERA.data0 = ((shot->collisionMask << 4) | (zDepth << 2) | flip);

        } else {
            // shot is inactive - don't update X or Y
            VERA.data0;
            VERA.data0;
            VERA.data0;
            VERA.data0;

            zDepth = 0;
            VERA.data0 = (zDepth << 2);
        }

        // skip the last byte
        VERA.data0;

    }

    /* Update all the enemies */

    for (int i = 0; i < MAX_ENEMIES; i++) {
        enemy_t* enemy = get_enemy_at_index(i);

        if (enemy->direction == LEFT || enemy->direction == RIGHT) {
            sprite_num = ENEMY_HORIZONTAL;
        } else {
            sprite_num = ENEMY_VERTICAL;
        }

        // Set the address in VRAM where the shot sprite is located
        VERA.data0 = ((SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * sprite_num)) >> 5);
        VERA.data0 = ((SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * sprite_num)) >> 13); // Bit 8 = Mode 0 (4bpp, 16 color)

        if (enemy->isActive) {
            VERA.data0 = enemy->xPos;
            VERA.data0 = (enemy->xPos >> 8);
            VERA.data0 = enemy->yPos;
            VERA.data0 = (enemy->yPos >> 8);

            zDepth = 3;
            flip = 0;
            if (enemy->direction == LEFT) {
                flip = H_FLIP;
            } else if (enemy->direction == DOWN) {
                flip = V_FLIP;
            }

            VERA.data0 = ((enemy->collisionMask << 4) | (zDepth << 2) | flip);

        } else {
            // shot is inactive - don't update X or Y
            VERA.data0;
            VERA.data0;
            VERA.data0;
            VERA.data0;

            zDepth = 0;
            VERA.data0 = (zDepth << 2);
        }

        // skip the last byte
        VERA.data0;

    }


    
}