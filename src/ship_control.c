#include "globals.h"

#include <cx16.h>
#include <stdint.h>
#include <stdbool.h>

#define SHIP_SPEED 5
#define V_FLIP 0x02
#define H_FLIP 0x01

const unsigned long vert_ship_pos_addr = SPRITE_ATTR_BASE + 2;
const unsigned long horiz_ship_pos_addr = SPRITE_ATTR_BASE + SPRITE_ATTR_ENTRY_SIZE + 2;

void move_player_ship() {
    static bool vFlip = false;
    static bool hFlip = false;
    static bool isHorizontal = true;
    uint8_t zDepth;

    // right
    if (!(g_joystick_state.data0 & JOY_RIGHT_MASK)) {
        g_shipXPos += SHIP_SPEED;
        if (g_shipXPos > MAX_X) {
            g_shipXPos = MIN_X;
        }
        isHorizontal = true;
        hFlip = false;
    }
    // left
    else if (!(g_joystick_state.data0 & JOY_LEFT_MASK)) {
        g_shipXPos -= SHIP_SPEED;
        if (g_shipXPos < MIN_X) {
            g_shipXPos = MAX_X;
        }
        isHorizontal = true;
        hFlip = true;
    }   
    // down
    if (!(g_joystick_state.data0 & JOY_DOWN_MASK)) {
        g_shipYPos += SHIP_SPEED;
        if (g_shipYPos > MAX_Y) {
            g_shipYPos = MIN_Y;
        }
        isHorizontal = false;
        vFlip = true;
    // up
    } else if (!(g_joystick_state.data0 & JOY_UP_MASK)) {
        g_shipYPos -= SHIP_SPEED;
        if (g_shipYPos < MIN_Y) {
            g_shipYPos = MAX_Y;
        }
        isHorizontal = false;
        vFlip = false;
    }


    // Set the new position of the ship, display the vertical or horizontal one as necessary
    VERA.address = vert_ship_pos_addr & 0xffff;
    VERA.address_hi = ((vert_ship_pos_addr >> 16) | VERA_INC_1); // Hi byte of sprite 1 position address, stride 1    
    VERA.data0 = g_shipXPos;
    VERA.data0 = g_shipXPos >> 8;
    VERA.data0 = g_shipYPos;
    VERA.data0 = g_shipYPos >> 8;
    if (isHorizontal) {
        zDepth = 0; // First is vertical
    } else {
        zDepth = SHIP_Z_DEPTH;
    }
    if (vFlip) {
        VERA.data0 = ((zDepth << 2) | V_FLIP);
    } else {
        VERA.data0 = zDepth << 2;
    }

    VERA.address = horiz_ship_pos_addr & 0xffff;
    VERA.address_hi = ((horiz_ship_pos_addr >> 16) | VERA_INC_1); // Hi byte of sprite 1 position address, stride 1    
    VERA.data0 = g_shipXPos;
    VERA.data0 = g_shipXPos >> 8;
    VERA.data0 = g_shipYPos;
    VERA.data0 = g_shipYPos >> 8;
    if (isHorizontal) {
        zDepth = SHIP_Z_DEPTH;
    } else {
        zDepth = 0;
    }
    if (hFlip) {
        VERA.data0 = ((zDepth << 2) | H_FLIP);
    } else {
        VERA.data0 = zDepth << 2;
    }

}