#include "globals.h"
#include <cx16.h>
#include <stdint.h>

#include "game_objects.h"

#include "sound.h"

void init_sound() {
    
    VERA.address = PSG_REGISTER_BASE & 0xffff;
    VERA.address_hi = ((PSG_REGISTER_BASE >> 16) | VERA_INC_1); // Hi byte of PSG register address, stride 1

    // start the note at A4
    VERA.data0 = A4_VALUE & 0x00ff; // Channel 0 frequency low byte
    VERA.data0 = A4_VALUE >> 8; // Channel 0 frequency high byte

    VERA.data0 = RIGHT_LEFT << 6; // right abd left speakers, volume 0
    VERA.data0 = NOISE << 6; // channel 0, noise waveform, no pulse with/xor
}

void update_sounds() {
    static uint8_t explosionSoundCounter = 0;
    
    const long rlVolumeRegister = PSG_REGISTER_BASE + 2;

    VERA.address = rlVolumeRegister & 0xffff;
    VERA.address_hi = ((rlVolumeRegister >> 16) | VERA_INC_0); // Hi byte of PSG register address, no stride

    for (uint8_t i = 0; i < MAX_GAME_OBJECTS; i++) {
        if (game_objects.isActive[i] && game_objects.isDestroyed[i] && game_objects.destructionCounter[i] > 0) {
            explosionSoundCounter = EXPLOSION_SOUND_LENGTH;
            break;
        }
    }

    if (explosionSoundCounter > 0) {
        VERA.data0 = (RIGHT_LEFT << 6 | 0x3f); // channel 0 volume full
        explosionSoundCounter--;
    } else {
        VERA.data0 = RIGHT_LEFT << 6; // channel 0 volume off
    }
}
