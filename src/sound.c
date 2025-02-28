#include "globals.h"
#include <cx16.h>
#include <stdint.h>
#include <stdbool.h>

#include "game_objects.h"

#include "sound.h"

void set_vera_to_psg_base();

void init_sound() {
    
    // Set the PSG registers
    set_vera_to_psg_base();

    // start the note at A4
    VERA.data0 = A4_VALUE & 0xff; // Channel 1 frequency low byte
    VERA.data0 = A4_VALUE >> 8; // Channel 1 frequency high byte

    VERA.data0 = 0x00; // mute left/right, volume 0
    VERA.data0 = 0b11000000; // channel 1, noise waveform, no pulse with/xor
}

void update_sounds() {
    set_vera_to_psg_base();

    VERA.data0;
    VERA.data0;

    bool shouldPlay = false;
    for (uint8_t i = 0; i < MAX_GAME_OBJECTS; i++) {
        if (game_objects.isActive[i] && game_objects.isDestroyed[i] && game_objects.destructionCounter[i] > 0) {
            shouldPlay = true;
            break;
        }
    }

    if (shouldPlay) {
        VERA.data0 = 0xff; // channel 1 volume full
    } else {
        VERA.data0 = 0x00; // channel 1 volume off
    }
}

void set_vera_to_psg_base() {
    VERA.address = PSG_REGISTER_BASE & 0xffff;
    VERA.address_hi = ((PSG_REGISTER_BASE >> 16) | VERA_INC_1); // Hi byte of PSG register address, stride 1
}