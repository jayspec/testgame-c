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

#define Z_DEPTH_VISIBLE 3
#define Z_DEPTH_HIDDEN 0

#define SHOT_SPEED 7
#define SHOT_FRAME_DELAY 10
#define MAX_SHOTS 6

#define ENEMY_SPEED 2
#define MAX_ENEMIES 2
#define ENEMY_DELAY 20

#define NUM_DESTRUCTION_FRAMES 8 // number of frames it takes for an object to be destroyed

#define DEBUG

typedef enum direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
} direction;
