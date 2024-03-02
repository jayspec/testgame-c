#include <stdint.h>
#include <cx16.h>

#include "globals.h"
#include "sprite_load.h"
#include "ship_control.h"
#include "joystick.h"
#include "shot.h"

int main() {
    global_init();

    load_sprites();

    while(1) {
        get_joystick_state();
        waitvsync();
        move_player_ship();
        handle_shots();
    }

}