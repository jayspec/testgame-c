#include "game_objects.h"
#include "globals.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "shot.h"
#include "joystick.h"

#ifdef DEBUG
#include "debug.h"
#include <stdio.h>
#endif


static uint8_t nextShotCountdown = SHOT_FRAME_DELAY;

static uint8_t numLiveShots;

void advance_shot_at_index(uint8_t shot_index);
void fire_shot();
bool shot_is_off_screen(int16_t x, int16_t y);

void init_shots() {
    numLiveShots = 0;
}

void handle_shots() {
    for (uint8_t i = 0; i < MAX_SHOTS; i++) {
        advance_shot_at_index(i);
    }

    if (nextShotCountdown > 0) {
        nextShotCountdown--;
    }

    JoyState joystick_state = get_joystick_state();
    if ((nextShotCountdown == 0) && (!(joystick_state.data0 & JOY_BTN_B_MASK))) {
        char message[50];
        sprintf(message, "numLiveShots: %d\n", numLiveShots);
        debug_to_emu_console(message);
        if (numLiveShots < MAX_SHOTS) {
            fire_shot();
            nextShotCountdown = SHOT_FRAME_DELAY;
        }
    }
}

void advance_shot_at_index(uint8_t shot_index) {
    uint8_t shot_object_index = shot_index + SHOT_FIRST_OBJ_INDEX;

    // If the shot has been destroyed by killing an enemy, reset the shot
    if (game_objects.isDestroyed[shot_object_index] && game_objects.isActive[shot_object_index]) {
        
        game_objects.isDestroyed[shot_object_index] = false;
        game_objects.isActive[shot_object_index] = false;
        
        numLiveShots--;

        return;
    }

    // otherwise, advance the shot and remove it if it goes screen
    if (game_objects.isActive[shot_object_index]) {

        switch(game_objects.direction[shot_object_index]) {
            case RIGHT:
                game_objects.xPos[shot_object_index] += SHOT_SPEED;
                break;
            case LEFT:
                game_objects.xPos[shot_object_index] -= SHOT_SPEED;
                break;
            case DOWN:
                game_objects.yPos[shot_object_index] += SHOT_SPEED;
                break;
            case UP:
                game_objects.yPos[shot_object_index] -= SHOT_SPEED;
                break;
            default:
                // do nothing
                break;
        }

        if (shot_is_off_screen(game_objects.xPos[shot_object_index], game_objects.yPos[shot_object_index])) {
            game_objects.isActive[shot_object_index] = false;
            numLiveShots--;
        }
    }
}

bool shot_is_off_screen(int16_t x, int16_t y) {
    return (x > MAX_X || x < MIN_X || y > MAX_Y || y < MIN_Y);
}

void fire_shot() {
    int8_t new_shot_index = -1;
    for (uint8_t i = SHOT_FIRST_OBJ_INDEX; i <= SHOT_LAST_OBJ_INDEX; i++) {
        if (!game_objects.isActive[i]) {
            new_shot_index = i;
            break;
        }
    }

    if (new_shot_index == -1) {
        return;
    }

    numLiveShots++;
    game_objects.xPos[new_shot_index] = game_objects.xPos[SHIP_OBJ_INDEX];
    game_objects.yPos[new_shot_index] = game_objects.yPos[SHIP_OBJ_INDEX];
    game_objects.direction[new_shot_index] = game_objects.direction[SHIP_OBJ_INDEX];

    game_objects.collisionMask[new_shot_index] = 0b0010;
    game_objects.isActive[new_shot_index] = true;
}

