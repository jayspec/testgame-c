#include "globals.h"

#include <cx16.h>
#include <stdint.h>
#include "ship.h"
#include "joystick.h"


static ship_state ship;
static const direction default_direction = RIGHT;

void init_ship() {
    ship.xPos = SHIP_START_X;
    ship.yPos = SHIP_START_Y;
    ship.direction = RIGHT;
    ship.collisionMask = 0b0001;
}

ship_state *get_ship_state() {
    return &ship;
}

void move_player_ship() {
    JoyState joystick_state = get_joystick_state();
    // right
    if (!(joystick_state.data0 & JOY_RIGHT_MASK)) {
        ship.xPos += SHIP_SPEED;
        if (ship.xPos > MAX_X) {
            ship.xPos = MIN_X;
        }
        ship.direction = RIGHT;
    }
    // left
    else if (!(joystick_state.data0 & JOY_LEFT_MASK)) {
        ship.xPos -= SHIP_SPEED;
        if (ship.xPos < MIN_X) {
            ship.xPos = MAX_X;
        }
        ship.direction = LEFT;
    }   
    // down
    if (!(joystick_state.data0 & JOY_DOWN_MASK)) {
        ship.yPos += SHIP_SPEED;
        if (ship.yPos > MAX_Y) {
            ship.yPos = MIN_Y;
        }
        ship.direction = DOWN;
    // up
    } else if (!(joystick_state.data0 & JOY_UP_MASK)) {
        ship.yPos -= SHIP_SPEED;
        if (ship.yPos < MIN_Y) {
            ship.yPos = MAX_Y;
        }
        ship.direction = UP;
    }

}