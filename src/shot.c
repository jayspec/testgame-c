#include <stdint.h>
#include <cx16.h>
#include <stdio.h>
#include "globals.h"

#define SHOT_SPEED 7
#define SHOT_FRAME_DELAY 10

uint8_t nextShotCountdown = SHOT_FRAME_DELAY;

void advance_shot_at_index(uint8_t attr_index) {
    shot_t shot = g_liveShots[attr_index];

    if (shot.isActive) {
        //printf("moving active shot %d", attr_index);
        VERA.address = (shot.attr_address + 2) & 0xffff;
        VERA.address_hi = (((shot.attr_address + 2) >> 16) | VERA_INC_1);

        shot.xPos += SHOT_SPEED;

        VERA.data0 = shot.xPos;
        VERA.data0 = (shot.xPos >> 8);
        VERA.data0 = shot.yPos;
        VERA.data0 = (shot.yPos >> 8);

        if (shot.xPos > MAX_X) {
            VERA.data0 = 0; // no Z-depth, no collisions, no flip
            shot.isActive = 0;
            g_numLiveShots--;
        } else {
            VERA.data0 = (3 << 2); // Z-depth of 3, visible
        }

        g_liveShots[attr_index] = shot;
    }
}

void fire_shot() {
    shot_t shot;
    int8_t shot_id = -1;
    for (uint8_t i = 0; i < MAX_SHOTS; i++) {
        if (!g_liveShots[i].isActive) {
            shot = g_liveShots[i];
            shot_id = i;
            break;
        }
    }

    if (shot_id == -1) {
        return;
    }

    shot.xPos = g_shipXPos;
    shot.yPos = g_shipYPos;
    shot.isActive = 1;

    g_liveShots[shot_id] = shot;

}

void handle_shots() {

    for (uint8_t i = 0; i < MAX_SHOTS; i++) {
        advance_shot_at_index(i);
    }

    if (nextShotCountdown > 0) {
        nextShotCountdown--;
    }

    //printf("ns countdown: %d, joystick state: %lx\n", nextShotCountdown, g_joystick_state);
    if ((nextShotCountdown == 0) && (!(g_joystick_state & JOY_BTN_B_MASK))) {
        //printf("fire\n");
        if (g_numLiveShots < MAX_SHOTS) {
            fire_shot();
            nextShotCountdown = SHOT_FRAME_DELAY;
            g_numLiveShots++;
        }
    }

}