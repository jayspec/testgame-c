#include <stdint.h>
#include <cx16.h>
#include <cbm.h>

#include "globals.h"

#define SPRITE_VRAM_BASE_ADDR 0x04000
#define SPRITE_SIZE 512 // 32x32 pixels, 4bpp

#define SPRITE_ATTR_ENTRY_SIZE 8

void load_sprite_at_index(const char* filename, uint8_t index, int16_t xPos, int16_t yPos, uint8_t zDepth) {
    unsigned long vram_addr = (SPRITE_VRAM_BASE_ADDR + (SPRITE_SIZE * index));
    unsigned long attr_addr = (SPRITE_ATTR_BASE + (SPRITE_ATTR_ENTRY_SIZE * index));

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

}

void load_sprites() {

    vera_sprites_enable(1);

    // load the sprites
    load_sprite_at_index("assets/plane.img", 0, g_shipXPos, g_shipYPos, 3);
}

