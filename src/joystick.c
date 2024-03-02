#include <stdint.h>
#include <cx16.h>
#include "globals.h"

void get_joystick_state() {
    g_joystick_state = cx16_k_joystick_get(0);  
}