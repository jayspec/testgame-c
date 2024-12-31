#include "globals.h"

#include <stdint.h>
#include <cx16.h>

#include "sprites.h"
#include "ship.h"

#define V_FLIP 0x02
#define H_FLIP 0x01

void update_sprites() {
    VERA.address = SPRITE_ATTR_BASE & 0xffff;
    VERA.address_hi = ((SPRITE_ATTR_BASE >> 16) | VERA_INC_1); // Hi byte of sprite 1 position address, stride 1

    uint8_t discard;
    uint8_t zDepth;
    uint8_t flip;
    ship_state *ship = get_ship_state();

    // Update the ship position
    for (uint8_t i = 0; i < NUM_SHIP_SPRITES; i++) {
        // skip the first two bytes - address and mode, which is wet when loaded
        VERA.data0;
        VERA.data0;

        VERA.data0 = ship->xPos;
        VERA.data0 = ship->xPos >> 8;
        VERA.data0 = ship->yPos;
        VERA.data0 = ship->yPos >> 8;

        if (i == VERTICAL_SHIP_INDEX) {
            if (ship->direction == UP || ship->direction == DOWN) {
                zDepth = SHIP_Z_DEPTH;
            } else {
                zDepth = 0;
            }
        } else {
            if (ship->direction == LEFT || ship->direction == RIGHT) {
                zDepth = SHIP_Z_DEPTH;
            } else {
                zDepth = 0;
            }
        }

        if (ship->direction == LEFT) {
            flip = H_FLIP;
        } else if (ship->direction == DOWN) {
            flip = V_FLIP;
        } else {
            flip = 0;
        }

        VERA.data0 = ((zDepth << 2) | flip);

        // skip the last byte: height width, and palette offset
        VERA.data0;

    }
    
}