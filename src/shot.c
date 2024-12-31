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

void advance_shot_at_index(uint8_t attr_index) {
    shot_t *shot = &liveShots[attr_index];

    if (shot->isActive) {
        // VERA.address = (shot->attr_address + 2) & 0xffff;
        // VERA.address_hi = (((shot->attr_address + 2) >> 16) | VERA_INC_1);

        shot->xPos += SHOT_SPEED;

        // VERA.data0 = shot->xPos;
        // VERA.data0 = (shot->xPos >> 8);
        // VERA.data0 = shot->yPos;
        // VERA.data0 = (shot->yPos >> 8);

        if (shot->xPos > MAX_X) {
            // VERA.data0 = 0; // no Z-depth, no collisions, no flip
            shot->isActive = 0;
            numLiveShots--;
        } else {
            // VERA.data0 = (3 << 2); // Z-depth of 3, visible
        }

    }
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
    shot->isActive = true;
}

