#include "sprite_load.h"
#include "sprite_update.h"
#include "ship.h"
#include "joystick.h"
#include "shot.h"
#include "enemy.h"

void initialize();

int main() {
    initialize();

    while(1) {
        refresh_joystick_state();
        move_player_ship();
        handle_shots();
        handle_enemies();
        waitvsync();
        update_sprites();
    }

    return 0;
}

void initialize() {
    init_joystick();
    init_ship();
    init_shots();
    init_enemies();
    load_sprites();
}