#include "globals.h"

#include <cbm.h>
#include <cx16.h>
#include <stdint.h>
#include <stdbool.h>

#include "zsmkit.h"
#include "music.h"

#ifdef DEBUG
#include "debug.h"
#endif

enum : uint8_t {
    RESERVED_BANK = 0,
    ZSMKIT_BANK,
    MUSIC_BANK,
};
static const uint8_t PRIORITY = 0;

void load_music() {
    const char *filename = "assets/back_again.zsm";
    
    cbm_k_setnam(filename);
    cbm_k_setlfs(15,8,15);

    RAM_BANK = MUSIC_BANK;
    cbm_k_load(CBM_LOAD_RAM, (void *)BANK_RAM);

    zsm_init_engine(ZSMKIT_BANK);
    zsm_setmem(PRIORITY, (uint16_t)BANK_RAM, MUSIC_BANK);

}

void start_music() {
    struct ZsmState state = zsm_getstate(PRIORITY);

    debug_byte(state.not_playable,  1);
    debug_byte(state.playing, 2);
    if (state.not_playable) {
        debug_to_emu_console("Music not playable\n");
        return;
    }

    zsm_setloop(PRIORITY, true);
    zsm_play(PRIORITY);

    state = zsm_getstate(PRIORITY);
    debug_byte(state.not_playable,  1);
    debug_byte(state.playing, 2);

}

void stop_music() {
    zsm_stop(PRIORITY);
}

void tick_music() {
    zsm_tick(ZCM_TICK_ALL);
}
