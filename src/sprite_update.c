#include "globals.h"

#include <stdint.h>
#include <cx16.h>

#include "game_objects.h"
#include "sprites.h"

#ifdef DEBUG
#include "debug.h"
#endif

#define V_FLIP 0x02
#define H_FLIP 0x01

#define EXPLOSION_PALETTE_OFFSET 4

typedef enum { SHIP_VERTICAL, SHIP_HORIZONTAL, SHOT_VERTICAL, SHOT_HORIZONTAL, ENEMY_VERTICAL, ENEMY_HORIZONTAL, EXPLOSION } sprite_type;

void update_sprites() {
    VERA.address = SPRITE_ATTR_BASE & 0xffff;
    VERA.address_hi = ((SPRITE_ATTR_BASE >> 16) | VERA_INC_1); // Hi byte of sprite 1 position address, stride 1

    uint8_t discard;
    uint8_t zDepth;
    uint8_t flip;
    uint8_t sprite_num;

    if (game_objects.isDestroyed[SHIP_OBJ_INDEX]) {
        sprite_num = EXPLOSION; // fix later
        game_objects.destructionCounter[SHIP_OBJ_INDEX]--;
    } else {
        if (game_objects.direction[SHIP_OBJ_INDEX] == LEFT || game_objects.direction[SHIP_OBJ_INDEX] == RIGHT) {
            sprite_num = SHIP_HORIZONTAL;
        } else {
            sprite_num = SHIP_VERTICAL;
        }
    }

    /* Update the ship position */
    // Set the address in VRAM where the ship sprite is located
    VERA.data0 = ((SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * sprite_num)) >> 5);
    VERA.data0 = ((SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * sprite_num)) >> 13); // Bit 8 = Mode 0 (4bpp, 16 color)

    VERA.data0 = game_objects.xPos[SHIP_OBJ_INDEX];
    VERA.data0 = game_objects.xPos[SHIP_OBJ_INDEX] >> 8;
    VERA.data0 = game_objects.yPos[SHIP_OBJ_INDEX];
    VERA.data0 = game_objects.yPos[SHIP_OBJ_INDEX] >> 8;

    if (game_objects.direction[SHIP_OBJ_INDEX] == LEFT && !game_objects.isDestroyed[SHIP_OBJ_INDEX]) {
        flip = H_FLIP;
    } else if (game_objects.direction[SHIP_OBJ_INDEX] == DOWN) {
        flip = V_FLIP;
    } else {
        flip = 0;
    }

    if (game_objects.isDestroyed[SHIP_OBJ_INDEX] && game_objects.destructionCounter[SHIP_OBJ_INDEX] == 0) {
        zDepth = 0;
    } else {
        zDepth = SHIP_Z_DEPTH;
    }

    VERA.data0 = ((game_objects.collisionMask[SHIP_OBJ_INDEX] << 4) | (zDepth << 2) | flip);

    // if the ship has been destroyed, switch to the explosion's palette offset
    if (game_objects.isDestroyed[SHIP_OBJ_INDEX]) {
        VERA.data0 = ((SPRITE_HEIGHT << 6) | (SPRITE_WIDTH << 4) | EXPLOSION_PALETTE_OFFSET);
    } else {
        // skip the last byte: height width, and palette offset
        VERA.data0;
    }

    /* Update all the shots */

    for (int currentShot = SHOT_FIRST_OBJ_INDEX; currentShot <= SHOT_LAST_OBJ_INDEX; currentShot++) {
        if (game_objects.direction[currentShot] == LEFT || game_objects.direction[currentShot] == RIGHT) {
            sprite_num = SHOT_HORIZONTAL;
        } else {
            sprite_num = SHOT_VERTICAL;
        }

        // Set the address in VRAM where the shot sprite is located
        VERA.data0 = ((SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * sprite_num)) >> 5);
        VERA.data0 = ((SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * sprite_num)) >> 13); // Bit 8 = Mode 0 (4bpp, 16 color)

        if (game_objects.isActive[currentShot]) {
            VERA.data0 = game_objects.xPos[currentShot];
            VERA.data0 = (game_objects.xPos[currentShot] >> 8);
            VERA.data0 = game_objects.yPos[currentShot];
            VERA.data0 = (game_objects.yPos[currentShot] >> 8);

            zDepth = 3;
            flip = 0;
            if (game_objects.direction[currentShot] == LEFT) {
                flip = H_FLIP;
            } else if (game_objects.direction[currentShot] == DOWN) {
                flip = V_FLIP;
            }

            VERA.data0 = ((game_objects.collisionMask[currentShot] << 4) | (zDepth << 2) | flip);

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

    for (int currentEnemy = ENEMY_FIRST_OBJ_INDEX; currentEnemy <= ENEMY_LAST_OBJ_INDEX; currentEnemy++) {

        if (game_objects.direction[currentEnemy] == LEFT || game_objects.direction[currentEnemy] == RIGHT) {
            sprite_num = ENEMY_HORIZONTAL;
        } else {
            sprite_num = ENEMY_VERTICAL;
        }

        // Set the address in VRAM where the shot sprite is located
        VERA.data0 = ((SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * sprite_num)) >> 5);
        VERA.data0 = ((SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * sprite_num)) >> 13); // Bit 8 = Mode 0 (4bpp, 16 color)

        if (game_objects.isActive[currentEnemy]) {
            VERA.data0 = game_objects.xPos[currentEnemy];
            VERA.data0 = (game_objects.xPos[currentEnemy] >> 8);
            VERA.data0 = game_objects.yPos[currentEnemy];
            VERA.data0 = (game_objects.yPos[currentEnemy] >> 8);

            if (game_objects.isDestroyed[currentEnemy]) {
                zDepth = 0;
            } else {
                zDepth = 3;
            }
            
            flip = 0;
            if (game_objects.direction[currentEnemy] == LEFT) {
                flip = H_FLIP;
            } else if (game_objects.direction[currentEnemy] == DOWN) {
                flip = V_FLIP;
            }

            VERA.data0 = ((game_objects.collisionMask[currentEnemy] << 4) | (zDepth << 2) | flip);

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