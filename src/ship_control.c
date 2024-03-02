#include <cx16.h>
#include <stdint.h>
#include "globals.h"

#define MIN_X -15
#define MAX_X 640
#define MIN_Y -15
#define MAX_Y 480
#define SHIP_SPEED 5

long joystick_state = 0;
unsigned long sprite1_pos_addr = SPRITE_ATTR_BASE + 2;

void move_player_ship() {
    joystick_state = cx16_k_joystick_get(0);  
    // right
    if (!(joystick_state & JOY_RIGHT_MASK)) {
        g_shipXPos += SHIP_SPEED;
        if (g_shipXPos > MAX_X) {
            g_shipXPos = MIN_X;
        }
    }
    // left
    else if (!(joystick_state & JOY_LEFT_MASK)) {
        g_shipXPos -= SHIP_SPEED;
        if (g_shipXPos < MIN_X) {
            g_shipXPos = MAX_X;
        }
    }   
    // down
    if (!(joystick_state & JOY_DOWN_MASK)) {
        g_shipYPos += SHIP_SPEED;
        if (g_shipYPos > MAX_Y) {
            g_shipYPos = MIN_Y;
        }
    // up
    } else if (!(joystick_state & JOY_UP_MASK)) {
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