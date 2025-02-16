#include "globals.h"

#include <stdint.h>
#include <cx16.h>
#include <cbm.h>

#include "sprites.h"
#include "sprite_load.h"

#ifdef DEBUG
#include <stdio.h>
#include "debug.h"
#endif

#define PALETTE_VRAM_BASE_ADDR 0x1FA00
#define PALETTE_LINE_SIZE 16 * 2 // 2 bytes per entry, 16 entries per line


unsigned long load_objects_to_vram_at_index(const char * filename, unsigned long base_vram_addr, uint16_t object_size, uint8_t object_index);
unsigned long load_sprites_to_vram_at_index(const char* filename, uint8_t sprite_index);
unsigned long load_palette_to_vram_at_index(const char* filename, uint8_t palette_index);
void set_attributes_for_sprites_in_sheet(sprite_sheet_collection *sprite_sheets, uint8_t sprite_sheet_index, uint8_t sprite_index, uint8_t sprite_attr_index);

void load_sprite_sheets() {
    // enable sprites
    vera_sprites_enable(1);

    sprite_sheet_collection sprite_sheets;
    /* Hero sprite sheet */
    sprite_sheets.filename[HERO_SPRITE_SHEET] = "assets/heros.img";
    sprite_sheets.palette_filename[HERO_SPRITE_SHEET] = "assets/heros.pal";
    sprite_sheets.palette_index[HERO_SPRITE_SHEET] = 1;
    sprite_sheets.collision_mask[HERO_SPRITE_SHEET] = 0b0001;
    sprite_sheets.default_x[HERO_SPRITE_SHEET] = SHIP_START_X;
    sprite_sheets.default_y[HERO_SPRITE_SHEET] = SHIP_START_Y;
    sprite_sheets.default_z_depth[HERO_SPRITE_SHEET] = SHIP_Z_DEPTH;
    sprite_sheets.sprites_in_sheet[HERO_SPRITE_SHEET] = 2;
    sprite_sheets.max_on_screen[HERO_SPRITE_SHEET] = 1;

    /* shot sprite sheet */
    sprite_sheets.filename[SHOT_SPRITE_SHEET] = "assets/shot.img";
    sprite_sheets.palette_filename[SHOT_SPRITE_SHEET] = "assets/shot.pal";
    sprite_sheets.palette_index[SHOT_SPRITE_SHEET] = 2;
    sprite_sheets.collision_mask[SHOT_SPRITE_SHEET] = 0b0010;
    sprite_sheets.default_x[SHOT_SPRITE_SHEET] = 0;
    sprite_sheets.default_y[SHOT_SPRITE_SHEET] = 0;
    sprite_sheets.default_z_depth[SHOT_SPRITE_SHEET] = 0;
    sprite_sheets.sprites_in_sheet[SHOT_SPRITE_SHEET] = 2;
    sprite_sheets.max_on_screen[SHOT_SPRITE_SHEET] = MAX_SHOTS;

    /* enemy sprite sheet */
    sprite_sheets.filename[ENEMY_SPRITE_SHEET] = "assets/enemy.img";
    sprite_sheets.palette_filename[ENEMY_SPRITE_SHEET] = "assets/enemy.pal";
    sprite_sheets.palette_index[ENEMY_SPRITE_SHEET] = 3;
    sprite_sheets.collision_mask[ENEMY_SPRITE_SHEET] = 0b0011;
    sprite_sheets.default_x[ENEMY_SPRITE_SHEET] = 0;
    sprite_sheets.default_y[ENEMY_SPRITE_SHEET] = 0;
    sprite_sheets.default_z_depth[ENEMY_SPRITE_SHEET] = 0;
    sprite_sheets.sprites_in_sheet[ENEMY_SPRITE_SHEET] = 2;
    sprite_sheets.max_on_screen[ENEMY_SPRITE_SHEET] = MAX_ENEMIES;

    /* explosion sprite sheet */
    sprite_sheets.filename[EXPLOSION_SPRITE_SHEET] = "assets/explo.img";
    sprite_sheets.palette_filename[EXPLOSION_SPRITE_SHEET] = "assets/explo.pal";
    sprite_sheets.palette_index[EXPLOSION_SPRITE_SHEET] = 4;
    sprite_sheets.collision_mask[EXPLOSION_SPRITE_SHEET] = 0b0000;
    sprite_sheets.default_x[EXPLOSION_SPRITE_SHEET] = 0;
    sprite_sheets.default_y[EXPLOSION_SPRITE_SHEET] = 0;
    sprite_sheets.default_z_depth[EXPLOSION_SPRITE_SHEET] = 0;
    sprite_sheets.sprites_in_sheet[EXPLOSION_SPRITE_SHEET] = 8;
    sprite_sheets.max_on_screen[EXPLOSION_SPRITE_SHEET] = 1;

    // load the sprites into vram,
    // Then set their attributes

    uint8_t current_sprite_attr_index = 0;
    uint8_t current_sprite_vram_index = 0;
    for(uint8_t i = 0; i < NUM_SPRITE_SHEETS; i++) {
        load_palette_to_vram_at_index(sprite_sheets.palette_filename[i], sprite_sheets.palette_index[i]);
        sprite_sheets.vram_addr[i] = load_sprites_to_vram_at_index(sprite_sheets.filename[i], current_sprite_vram_index);
        for (uint8_t j = 0; j < sprite_sheets.max_on_screen[i]; j++) {
            set_attributes_for_sprites_in_sheet(&sprite_sheets, i, j, current_sprite_attr_index++);
        }
        current_sprite_vram_index += sprite_sheets.sprites_in_sheet[i];
    }
}

unsigned long load_objects_to_vram_at_index(const char* filename, unsigned long base_vram_addr, uint16_t object_size, uint8_t object_index) {
    unsigned long vram_addr = (base_vram_addr + (object_size * object_index));

    void *vram_ptr = (void *)(vram_addr & 0xffff);
    unsigned char upper = (unsigned char)(vram_addr >> 16);

    // load the sprite
    cbm_k_setnam(filename);
    // load with header
    cbm_k_setlfs(0, 8, 0);

    if (upper) {
        cbm_k_load(3, vram_ptr);
    }
    else {
        cbm_k_load(2, vram_ptr);
    }

    return vram_addr;
}

unsigned long load_sprites_to_vram_at_index(const char* filename, uint8_t sprite_index) {
    return load_objects_to_vram_at_index(filename, SPRITE_VRAM_BASE_ADDR, SPRITE_SIZE, sprite_index);
}

unsigned long load_palette_to_vram_at_index(const char* filename, uint8_t palette_index) {
    return load_objects_to_vram_at_index(filename, PALETTE_VRAM_BASE_ADDR, PALETTE_LINE_SIZE, palette_index);
}

void set_attributes_for_sprites_in_sheet(sprite_sheet_collection *sprite_sheets, uint8_t sprite_sheet_index, uint8_t sprite_vram_index, uint8_t sprite_attr_index) {
    unsigned long attr_addr = (SPRITE_ATTR_BASE + (SPRITE_ATTR_ENTRY_SIZE * sprite_attr_index));

    VERA.control = 0; // Use VERA port 0
    // Set the attribute address to start loading
    VERA.address = attr_addr & 0xffff;
    VERA.address_hi = ((attr_addr >> 16) | VERA_INC_1); // Hi byte of sprite 1 attribute address, stride 1

    // Set the address in VRAM where the sprite is located
    VERA.data0 = ((sprite_sheets->vram_addr[sprite_sheet_index] + (SPRITE_SIZE * sprite_vram_index)) >> 5);
    VERA.data0 = ((sprite_sheets->vram_addr[sprite_sheet_index] + (SPRITE_SIZE * sprite_vram_index)) >> 13); // Bit 8 = Mode 0 (4bpp, 16 color)

    // Set initial position
    VERA.data0 = sprite_sheets->default_x[sprite_sheet_index];
    VERA.data0 = sprite_sheets->default_x[sprite_sheet_index] >> 8;
    VERA.data0 = sprite_sheets->default_y[sprite_sheet_index];
    VERA.data0 = sprite_sheets->default_y[sprite_sheet_index] >> 8;

    // No flip, specified collision mask, specified Z-depth
    VERA.data0 = (sprite_sheets->collision_mask[sprite_sheet_index] << 4) | (sprite_sheets->default_z_depth[sprite_sheet_index] << 2);

    // Size = 32x32 pixels, w/ configurable palette offset
    VERA.data0 = ((SPRITE_HEIGHT << 6) | (SPRITE_WIDTH << 4) | sprite_sheets->palette_index[sprite_sheet_index]);

}

