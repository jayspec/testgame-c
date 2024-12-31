#include "globals.h"

#include <cx16.h>

#include "joystick.h"

static JoyState joystick_state;

void init_joystick() {
        joystick_state.data0 = 0;
}

void refresh_joystick_state() {
    joystick_state = cx16_k_joystick_get(0);
}

JoyState get_joystick_state() {
    return joystick_state;  
}