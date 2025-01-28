#include "globals.h"

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "shot.h"
#include "ship.h"
#include "joystick.h"


static uint8_t nextShotCountdown = SHOT_FRAME_DELAY;

static uint8_t numLiveShots;
static shot_t liveShots[MAX_SHOTS];

void advance_shot_at_index(uint8_t attr_index);
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
        if (numLiveShots < MAX_SHOTS) {
            fire_shot();
            nextShotCountdown = SHOT_FRAME_DELAY;
            numLiveShots++;
        }
    }
}

shot_t *get_shot_at_index(uint8_t index) {
    return &liveShots[index];
}

void advance_shot_at_index(uint8_t attr_index) {
    shot_t *shot = &liveShots[attr_index];

    if (shot->isActive) {

        if (shot->direction == RIGHT) {
            shot->xPos += SHOT_SPEED;
        } else if (shot->direction == LEFT) {
            shot->xPos -= SHOT_SPEED;
        } else if (shot->direction == DOWN) {
            shot->yPos += SHOT_SPEED;
        } else if (shot->direction == UP) {
            shot->yPos -= SHOT_SPEED;
        }

        if (shot_is_off_screen(shot->xPos, shot->yPos)) {
            shot->isActive = 0;
            numLiveShots--;
        }
    }
}

bool shot_is_off_screen(int16_t x, int16_t y) {
    return (x > MAX_X || x < MIN_X || y > MAX_Y || y < MIN_Y);
}

void fire_shot() {
    shot_t *shot = NULL;
    for (uint8_t i = 0; i < MAX_SHOTS; i++) {
        if (!liveShots[i].isActive) {
            shot = &liveShots[i];
            break;
        }
    }

    if (shot == NULL) {
        return;
    }

    ship_state *ship = get_ship_state();
    shot->xPos = ship->xPos;
    shot->yPos = ship->yPos;
    shot->direction = ship->direction;
    shot->collisionMask = 0b0010;
    shot->isActive = true;
}

