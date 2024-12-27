#include "globals.h"

#include <cx16.h>
#include <stdint.h>

#define SHIP_SPEED 5

const unsigned long sprite1_pos_addr = SPRITE_ATTR_BASE + 2;

void move_player_ship() {

    // right
    if (!(g_joystick_state.data0 & JOY_RIGHT_MASK)) {
        g_shipXPos += SHIP_SPEED;
        if (g_shipXPos > MAX_X) {
            g_shipXPos = MIN_X;
        }
    }
    // left
    else if (!(g_joystick_state.data0 & JOY_LEFT_MASK)) {
        g_shipXPos -= SHIP_SPEED;
        if (g_shipXPos < MIN_X) {
            g_shipXPos = MAX_X;
        }
    }   
    // down
    if (!(g_joystick_state.data0 & JOY_DOWN_MASK)) {
        g_shipYPos += SHIP_SPEED;
        if (g_shipYPos > MAX_Y) {
            g_shipYPos = MIN_Y;
        }
    // up
    } else if (!(g_joystick_state.data0 & JOY_UP_MASK)) {
        g_shipYPos -= SHIP_SPEED;
        if (g_shipYPos < MIN_Y) {
            g_shipYPos = MAX_Y;
        }
    }   
    // Set the new position of the ship
    VERA.address = sprite1_pos_addr & 0xffff;
    VERA.address_hi = ((sprite1_pos_addr >> 16) | VERA_INC_1); // Hi byte of sprite 1 position address, stride 1    
    VERA.data0 = g_shipXPos;
    VERA.data0 = g_shipXPos >> 8;
    VERA.data0 = g_shipYPos;
    VERA.data0 = g_shipYPos >> 8;   
}