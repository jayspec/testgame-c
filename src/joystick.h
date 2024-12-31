#pragma once

#include "globals.h"
#include <cx16.h>

extern void init_joystick();
extern void refresh_joystick_state();
extern JoyState get_joystick_state();