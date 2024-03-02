#include <stdint.h>

int16_t g_shipXPos; // = 150;
int16_t g_shipYPos; // = 100;

long g_joystick_state; // = 0;

void global_init() {
    g_shipXPos = 150;
    g_shipYPos = 100;
}