#pragma once
#include <stdint.h>

extern void debug_to_emu_console(char *message);
void debug_byte(uint8_t byte, uint8_t channel);