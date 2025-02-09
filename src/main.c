#include "collision.h"
#include "globals.h"
#include <cx16.h>

#include <stdint.h>

#include "sprite_load.h"
#include "sprite_update.h"
#include "ship.h"
#include "joystick.h"
#include "shot.h"
#include "enemy.h"
#include "irq.h"
#include "game_objects.h"

#ifdef DEBUG
#include "debug.h"
#endif

void initialize();
void init_irq();
uint8_t irq_handler();

int main() {
    initialize();

    while(1) {
        refresh_joystick_state();
        move_player_ship();
        handle_shots();
        handle_enemies();
        handle_collisions();
        waitvsync();
        update_sprites();
    }

    return 0;
}

void initialize() {
    init_irq();
    init_joystick();
    init_ship();
    init_shots();
    init_enemies();
    load_sprite_sheets();
}

void init_irq() {
    VERA.irq_enable = 0b00000101;
    uint8_t (*handler)(void) = irq_handler;
    set_irq(handler);
}

uint8_t __attribute__((interrupt_norecurse,no_isr)) irq_handler() {
    if (VERA.irq_flags & 0b100) {
        VERA.irq_flags = 0b100;
        game_objects.checkCollisions = true;
        return 1;
    } else {
        return 0;
    }
}