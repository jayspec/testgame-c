#include <stdint.h>
#include <cx16.h>
#include <cbm.h>

#include "globals.h"

void load_sprites() {
    unsigned long sprite_vram_addr = 0x04000;

    void *vram_ptr = (void *)(sprite_vram_addr & 0xffff);
    unsigned char upper = (unsigned char)(sprite_vram_addr >> 16);

    global_init();

    vera_sprites_enable(1);

    // load the sprites
    cbm_k_setnam("assets/plane.img");
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
    VERA.address = SPRITE_ATTR_BASE & 0xffff;
    VERA.address_hi = ((SPRITE_ATTR_BASE >> 16) | VERA_INC_1); // Hi byte of sprite 1 attribute address, stride 1

    // Set the address in VRAM where the sprite is located
    uint8_t sprite_vram_lo = (sprite_vram_addr >> 5);
    uint8_t sprite_vram_hi = (sprite_vram_addr >> 13); // Bit 8 = Mode 0 (4bpp, 16 color)

    VERA.data0 = sprite_vram_lo;
    VERA.data0 = sprite_vram_hi;

    // Set initial position
    VERA.data0 = g_shipXPos;
    VERA.data0 = g_shipXPos >> 8;
    VERA.data0 = g_shipYPos;
    VERA.data0 = g_shipYPos >> 8;

    // No flip, no collision mask, Z-depth 3 (in front of layer 1)
    VERA.data0 = 0x0c;
    // Palette offset 0, 32x32 pixels
    VERA.data0 = 0xa0;
}