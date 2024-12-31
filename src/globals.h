#pragma once

#ifndef __CX16__
#define __CX16__
#endif
#ifndef __CBM__
#define __CBM__
#endif

#define MIN_X -15
#define MAX_X 640
#define MIN_Y -15
#define MAX_Y 480

#define SHIP_SPEED 5
#define SHIP_START_X 150
#define SHIP_START_Y 100
#define SHIP_Z_DEPTH 3

#define SHOT_SPEED 7
#define SHOT_FRAME_DELAY 10
#define MAX_SHOTS 6

typedef enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction;
