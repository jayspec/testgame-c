#include "globals.h"

#include <cx16.h>
#include <stdint.h>
#include "game_objects.h"
#include "ship.h"
#include "joystick.h"


static const direction default_direction = RIGHT;

void init_ship() {
    game_objects.xPos[SHIP_OBJ_INDEX] = SHIP_START_X;
    game_objects.yPos[SHIP_OBJ_INDEX] = SHIP_START_Y;
    game_objects.direction[SHIP_OBJ_INDEX] = default_direction;
    game_objects.collisionMask[SHIP_OBJ_INDEX] = 0b0001;
}

void move_player_ship() {
    JoyState joystick_state = get_joystick_state();
    // right
    if (!(joystick_state.data0 & JOY_RIGHT_MASK)) {
        game_objects.xPos[SHIP_OBJ_INDEX] += SHIP_SPEED;
        if (game_objects.xPos[SHIP_OBJ_INDEX] > MAX_X) {
            game_objects.xPos[SHIP_OBJ_INDEX] = MIN_X;
        }
        game_objects.direction[SHIP_OBJ_INDEX] = RIGHT;
    }
    // left
    else if (!(joystick_state.data0 & JOY_LEFT_MASK)) {
        game_objects.xPos[SHIP_OBJ_INDEX] -= SHIP_SPEED;
        if (game_objects.xPos[SHIP_OBJ_INDEX] < MIN_X) {
            game_objects.xPos[SHIP_OBJ_INDEX] = MAX_X;
        }
        game_objects.direction[SHIP_OBJ_INDEX] = LEFT;
    }   
    // down
    if (!(joystick_state.data0 & JOY_DOWN_MASK)) {
        game_objects.yPos[SHIP_OBJ_INDEX] += SHIP_SPEED;
        if (game_objects.yPos[SHIP_OBJ_INDEX] > MAX_Y) {
            game_objects.yPos[SHIP_OBJ_INDEX] = MIN_Y;
        }
        game_objects.direction[SHIP_OBJ_INDEX] = DOWN;
    // up
    } else if (!(joystick_state.data0 & JOY_UP_MASK)) {
        game_objects.yPos[SHIP_OBJ_INDEX] -= SHIP_SPEED;
        if (game_objects.yPos[SHIP_OBJ_INDEX] < MIN_Y) {
            game_objects.yPos[SHIP_OBJ_INDEX] = MAX_Y;
        }
        game_objects.direction[SHIP_OBJ_INDEX] = UP;
    }

}