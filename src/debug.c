#include "globals.h"

#include <stdint.h>
#include <stdbool.h>

#define EMU_CHECK 0x9fbe
#define DEBUG1 0x9fb9
#define DEBUG2 0x9fba
#define CONSOLE_OUT 0x9fbb

bool is_in_emulator();

void debug_to_emu_console(char *message) {
    #ifndef DEBUG
    return;
    #endif

    uint8_t *console_out_ptr = (uint8_t *)(CONSOLE_OUT);

    if (is_in_emulator()) {
        // We're in the emulator
        char *c = message;
        while(*c != '\0') {
            *console_out_ptr = *c;
            c++;
        }
    }
}

void debug_byte(uint8_t byte, uint8_t channel) {
    #ifndef DEBUG
    return;
    #endif

    uint8_t *debug_1_ptr = (uint8_t *)(DEBUG1);
    uint8_t *debug_2_ptr = (uint8_t *)(DEBUG2);

    if (is_in_emulator()) {
        if (channel == 2) {
            *debug_2_ptr = byte;
        } else {
            *debug_1_ptr= byte;

        }
    }
}

bool is_in_emulator() {
    uint8_t *emu_check_ptr = (uint8_t *)(EMU_CHECK);
    return (*emu_check_ptr == '1' && *(emu_check_ptr + 1) == '6');
}