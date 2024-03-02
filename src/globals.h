#pragma once
#define SPRITE_ATTR_BASE 0x1fc08
#define SPRITE_ATTR_ENTRY_SIZE 8

#define MIN_X -15
#define MAX_X 640
#define MIN_Y -15
#define MAX_Y 480
#define MAX_SHOTS 6

extern long g_joystick_state;

extern int16_t g_shipXPos;
extern int16_t g_shipYPos;

extern uint8_t g_numLiveShots;

typedef struct {
    int16_t xPos;
    int16_t yPos;
    unsigned long attr_address;
    uint8_t isActive;
} shot_t;

extern shot_t g_liveShots[];

extern void global_init();
