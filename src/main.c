#include <stdint.h>
#include <cbm.h>
#include <cx16.h>
#include <stdio.h>

#include "globals.h"

int main() {
    unsigned long sprite_vram_addr = 0x04000;
    unsigned long sprite_attr_addr = 0x1fc08;

    unsigned long sprite1_pos_addr = sprite_attr_addr + 2;

    void *vram_ptr = (void *)(sprite_vram_addr & 0xffff);
    unsigned char upper = (unsigned char)(sprite_vram_addr >> 16);


    const short MIN_X = -15;
    const short MAX_X = 640;
    const short MIN_Y = -15;
    const short MAX_Y = 480;

    const uint8_t SHIP_SPEED = 5;

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
    VERA.address = sprite_attr_addr & 0xffff;
    VERA.address_hi = ((sprite_attr_addr >> 16) | VERA_INC_1); // Hi byte of sprite 1 attribute address, stride 1

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

    while(1) {
        waitvsync();
        cx16_k_joystick_scan();
        g_joystick_state = cx16_k_joystick_get(0);

        // right
        if (!(g_joystick_state & JOY_RIGHT_MASK)) {
            g_shipXPos += SHIP_SPEED;
            if (g_shipXPos > MAX_X) {
                g_shipXPos = MIN_X;
            }
        }
        // left
        else if (!(g_joystick_state & JOY_LEFT_MASK)) {
            g_shipXPos -= SHIP_SPEED;
            if (g_shipXPos < MIN_X) {
                g_shipXPos = MAX_X;
            }
        }

        // down
        if (!(g_joystick_state & JOY_DOWN_MASK)) {
            g_shipYPos += SHIP_SPEED;
            if (g_shipYPos > MAX_Y) {
                g_shipYPos = MIN_Y;
            }
        // up
        } else if (!(g_joystick_state & JOY_UP_MASK)) {
            g_shipYPos -= SHIP_SPEED;
            if (g_shipYPos < MIN_Y) {
                g_shipYPos = MAX_Y;
            }
        }

        // Set the new position of the ship
        VERA.address = sprite1_pos_addr & 0xffff;
        VERA.address_hi = ((sprite1_pos_addr >> 16) | VERA_INC_1); // Hi byte of sprite 1 position address, stride 1

        VERA.data0 = g_shipXPos;
        VERA.data0 = g_shipXPos >> 8;
        VERA.data0 = g_shipYPos;
        VERA.data0 = g_shipYPos >> 8;

    }

}