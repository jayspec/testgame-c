#include <stdint.h>

#pragma once

#define NUM_SPRITE_SHEETS 4

#define HERO_SPRITE_SHEET 0
#define SHOT_SPRITE_SHEET 1
#define ENEMY_SPRITE_SHEET 2
#define EXPLOSION_SPRITE_SHEET 3

typedef struct {
    char *filename[NUM_SPRITE_SHEETS];
    char *palette_filename[NUM_SPRITE_SHEETS];
    unsigned long vram_addr[NUM_SPRITE_SHEETS];
    uint8_t collision_mask[NUM_SPRITE_SHEETS];
    int default_x[NUM_SPRITE_SHEETS];
    int default_y[NUM_SPRITE_SHEETS];
    uint8_t palette_index[NUM_SPRITE_SHEETS];
    uint8_t default_z_depth[NUM_SPRITE_SHEETS];
    uint8_t sprites_in_sheet[NUM_SPRITE_SHEETS];
    uint8_t max_on_screen[NUM_SPRITE_SHEETS];
} sprite_sheet_collection;

extern void load_sprite_sheets();