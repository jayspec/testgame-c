#include <stdint.h>
#include <cx16.h>

#include "globals.h"
#include "sprite_load.h"
#include "ship_control.h"

int main() {

    load_sprites();

    while(1) {
        waitvsync();
        move_player_ship();
    }

}