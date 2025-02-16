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

typedef enum { SHIP_VERTICAL, SHIP_HORIZONTAL, SHOT_VERTICAL, SHOT_HORIZONTAL, ENEMY_VERTICAL, ENEMY_HORIZONTAL, EXPLOSION } sprite_type;
typedef enum { DEFAULT_PALETTE, SHIP_PALETTE, SHOT_PALETTE, ENEMY_PALETTE, EXPLOSION_PALETTE } palette_type;

void set_sprite_costume_at_index(uint8_t objectIndex);
void set_sprite_position_at_index(uint8_t objectIndex);
void set_sprite_visibility_and_orientation_at_index(uint8_t objectIndex);
void set_sprite_palette_at_index(uint8_t objectIndex);

void update_sprites() {

    VERA.address = SPRITE_ATTR_BASE & 0xffff;
    VERA.address_hi = ((SPRITE_ATTR_BASE >> 16) | VERA_INC_1); // Hi byte of sprite 1 position address, stride 1

    for (uint8_t i = 0; i < MAX_GAME_OBJECTS; i++) {
        set_sprite_costume_at_index(i);
        set_sprite_position_at_index(i);
        set_sprite_visibility_and_orientation_at_index(i);
        set_sprite_palette_at_index(i);
    }
}

void set_sprite_costume_at_index(uint8_t objectIndex) {
    uint8_t spriteCostumeNum;
    uint8_t horizontalCostumeNum, verticalCostumeNum;

    if (objectIndex == SHIP_OBJ_INDEX) {
        horizontalCostumeNum = SHIP_HORIZONTAL;
        verticalCostumeNum = SHIP_VERTICAL;
    } else if (objectIndex >= SHOT_FIRST_OBJ_INDEX && objectIndex <= SHOT_LAST_OBJ_INDEX) {
        horizontalCostumeNum = SHOT_HORIZONTAL;
        verticalCostumeNum = SHOT_VERTICAL;
    } else if (objectIndex >= ENEMY_FIRST_OBJ_INDEX && objectIndex <= ENEMY_LAST_OBJ_INDEX) {
        horizontalCostumeNum = ENEMY_HORIZONTAL;
        verticalCostumeNum = ENEMY_VERTICAL;
    }

    if (game_objects.isDestroyed[objectIndex]) {
        // If the ship has been destroyed, display the destruction animation
        spriteCostumeNum = EXPLOSION + (NUM_DESTRUCTION_FRAMES - game_objects.destructionCounter[objectIndex]);
        if (game_objects.destructionCounter[objectIndex] > 0) {
            game_objects.destructionCounter[objectIndex]--;
        }
    } else {
        if (game_objects.direction[objectIndex] == LEFT || game_objects.direction[objectIndex] == RIGHT) {
            spriteCostumeNum = horizontalCostumeNum;
        } else {
            spriteCostumeNum = verticalCostumeNum;
        }
    }

    // Set the address in VRAM where the sprite's costume is located
    VERA.data0 = ((SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * spriteCostumeNum)) >> 5);
    VERA.data0 = ((SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * spriteCostumeNum)) >> 13); // Bit 8 = Mode 0 (4bpp, 16 color)
    
}

void set_sprite_position_at_index(uint8_t objectIndex) {
    /* Update the sprite position */

    if (game_objects.isActive[objectIndex]) {
        VERA.data0 = game_objects.xPos[objectIndex];
        VERA.data0 = game_objects.xPos[objectIndex] >> 8;
        VERA.data0 = game_objects.yPos[objectIndex];
        VERA.data0 = game_objects.yPos[objectIndex] >> 8;
    } else {
        // object is inactive - don't update X or Y
        VERA.data0;
        VERA.data0;
        VERA.data0;
        VERA.data0;
    }
}

void set_sprite_visibility_and_orientation_at_index(uint8_t objectIndex) {
    uint8_t zDepth;
    uint8_t flip;

    if (game_objects.direction[objectIndex] == LEFT && !game_objects.isDestroyed[objectIndex]) {
        flip = H_FLIP;
    } else if (game_objects.direction[objectIndex] == DOWN) {
        flip = V_FLIP;
    } else {
        flip = 0;
    }

    if (!game_objects.isActive[objectIndex] || (game_objects.isDestroyed[objectIndex] && game_objects.destructionCounter[objectIndex] == 0)) {
        zDepth = Z_DEPTH_HIDDEN;
    } else {
        zDepth = Z_DEPTH_VISIBLE;
    }

    VERA.data0 = ((game_objects.collisionMask[objectIndex] << 4) | (zDepth << 2) | flip);

}

void set_sprite_palette_at_index(uint8_t objectIndex) {
    palette_type palette;
    // if the object has been destroyed, switch to the explosion's palette offset
    if (game_objects.isDestroyed[objectIndex]) {
        palette = EXPLOSION_PALETTE;
    } else if (objectIndex == SHIP_OBJ_INDEX) {
        palette = SHIP_PALETTE;
    } else if (objectIndex >= SHOT_FIRST_OBJ_INDEX && objectIndex <= SHOT_LAST_OBJ_INDEX) {
        palette = SHOT_PALETTE;
    } else if (objectIndex >= ENEMY_FIRST_OBJ_INDEX && objectIndex <= ENEMY_LAST_OBJ_INDEX) {
        palette = ENEMY_PALETTE;
    }

    VERA.data0 = ((SPRITE_HEIGHT << 6) | (SPRITE_WIDTH << 4) | palette);
}
