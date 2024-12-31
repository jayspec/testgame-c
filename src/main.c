#include "globals.h"

#include <stdint.h>
#include <cx16.h>

#include "sprite_load.h"
#include "sprite_update.h"
#include "ship.h"
#include "joystick.h"
#include "shot.h"

void initialize();

int main() {
    initialize();

    while(1) {
        refresh_joystick_state();
        move_player_ship();
        handle_shots();
        waitvsync();
        update_sprites();
    }

    return 0;
}

void initialize() {
    init_joystick();
    init_ship();
    init_shots();
    load_sprites();
}