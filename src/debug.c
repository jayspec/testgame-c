#include "globals.h"

#include <stdint.h>

#define EMU_CHECK 0x9fbe
#define CONSOLE_OUT 0x9fbb

void debug_to_emu_console(char *message) {
    #ifndef DEBUG
    return;
    #endif

    uint8_t *emu_check_ptr = (uint8_t *)(EMU_CHECK);
    uint8_t *console_out_ptr = (uint8_t *)(CONSOLE_OUT);

    if (*emu_check_ptr == '1' && *(emu_check_ptr + 1) == '6') {
        // We're in the emulator
        char *c = message;
        while(*c != '\0') {
            *console_out_ptr = *c;
            c++;
        }
    }
}