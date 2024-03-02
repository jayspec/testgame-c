#include <stdint.h>
#include <cx16.h>
#include <cbm.h>

#include "globals.h"

#define SPRITE_VRAM_BASE_ADDR 0x04000
#define SPRITE_SIZE 512 // 32x32 pixels, 4bpp

unsigned long load_sprite_to_vram_at_index(const char* filename, uint8_t vram_index) {
    unsigned long vram_addr = (SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * vram_index));

    void *vram_ptr = (void *)(vram_addr & 0xffff);
    unsigned char upper = (unsigned char)(vram_addr >> 16);

    // load the sprite
    cbm_k_setnam(filename);
    // headerless load
    cbm_k_setlfs(0, 8, 2);

    if (upper) {
        cbm_k_load(3, vram_ptr);
    }
    else {
        cbm_k_load(2, vram_ptr);
    }

    return vram_addr;
}

unsigned long set_sprite_attributes_at_index(uint8_t attr_index, unsigned long vram_addr, int16_t xPos, int16_t yPos, uint8_t zDepth) {
    unsigned long attr_addr = (SPRITE_ATTR_BASE + (SPRITE_ATTR_ENTRY_SIZE * attr_index));

    VERA.control = 0; // Use VERA port 0
    // Set the attribute address to start loading
    VERA.address = attr_addr & 0xffff;
    VERA.address_hi = ((attr_addr >> 16) | VERA_INC_1); // Hi byte of sprite 1 attribute address, stride 1

    // Set the address in VRAM where the sprite is located
    VERA.data0 = (vram_addr >> 5);
    VERA.data0 = (vram_addr >> 13); // Bit 8 = Mode 0 (4bpp, 16 color)

    // Set initial position
    VERA.data0 = xPos;
    VERA.data0 = xPos >> 8;
    VERA.data0 = yPos;
    VERA.data0 = yPos >> 8;

    // No flip, no collision mask, specified Z-depth
    VERA.data0 = 0b0000000 | (zDepth << 2);
    // Palette offset 0, 32x32 pixels
    VERA.data0 = 0xa0;

    return attr_addr;
}

void load_sprites() {

    vera_sprites_enable(1);

    // load the sprites into vram,
    // Then set their attributes
    unsigned long plane_vram_addr = load_sprite_to_vram_at_index("assets/plane.img", 0);
    set_sprite_attributes_at_index(0, plane_vram_addr, g_shipXPos, g_shipYPos, 3);

    // Load the shots.  There can be multiple shots, all pointing to the same image in vram
    unsigned long shot_vram_addr = load_sprite_to_vram_at_index("assets/shot.img", 1);
    for (uint8_t i = 0; i < MAX_SHOTS; i++) {
        // i + 1 because the ship is at 0
        unsigned long sprite_attr_addr = set_sprite_attributes_at_index(i + 1, shot_vram_addr, 0, 0, 0);
        
        shot_t shot;
        shot.attr_address = sprite_attr_addr;
        shot.isActive = 0;
        shot.xPos = 0;
        shot.yPos = 0;

        g_liveShots[i] = shot;
    }
}

