#include "globals.h"

#include <cx16.h>

#include "collision.h"
#include "game_objects.h"

#include <stdbool.h>
#include <stdint.h>

#ifdef DEBUG
#include "debug.h"
#endif

#define SPRITE_OBJECT_HEIGHT 32
#define SPRITE_OBJECT_WIDTH 32

bool check_object_collision_at_object_indexes(uint8_t obj1, uint8_t obj2);

void handle_collisions() {
    if (game_objects.checkCollisions) {
        if (VERA.irq_flags & 0b00010000) {
            // check ship/enemy collisions
            for (uint8_t i = ENEMY_FIRST_OBJ_INDEX; i <= ENEMY_LAST_OBJ_INDEX; i++) {
                if (check_object_collision_at_object_indexes(SHIP_OBJ_INDEX, i)) {
                    if (game_objects.isDestroyed[SHIP_OBJ_INDEX]) {
                        // ship is already destroyed, don't do anything
                        break;
                    }
                    game_objects.isDestroyed[SHIP_OBJ_INDEX] = true;
                    game_objects.destructionCounter[SHIP_OBJ_INDEX] = NUM_DESTRUCTION_FRAMES;
                    break;
                }
            }
        }
        if (VERA.irq_flags & 0b00100000) {
            // check shot/enemy collisions
            for (uint8_t shot_index = SHOT_FIRST_OBJ_INDEX; shot_index <= SHOT_LAST_OBJ_INDEX; shot_index++) {
                for (uint8_t enemy_index = ENEMY_FIRST_OBJ_INDEX; enemy_index <= ENEMY_LAST_OBJ_INDEX; enemy_index++) {
                    if (check_object_collision_at_object_indexes(shot_index, enemy_index)) {
                        game_objects.isDestroyed[enemy_index] = true;
                        game_objects.destructionCounter[enemy_index] = NUM_DESTRUCTION_FRAMES;
                        game_objects.isDestroyed[shot_index] = true;
                    }
                }
            }
        }

        game_objects.checkCollisions = false;
    }

}

bool check_object_collision_at_object_indexes(uint8_t index1, uint8_t index2) {
    return (
        game_objects.xPos[index1] < game_objects.xPos[index2] + SPRITE_OBJECT_WIDTH &&
        game_objects.xPos[index1] + SPRITE_OBJECT_WIDTH > game_objects.xPos[index2] &&
        game_objects.yPos[index1] < game_objects.yPos[index2] + SPRITE_OBJECT_HEIGHT &&
        game_objects.yPos[index1] + SPRITE_OBJECT_HEIGHT > game_objects.yPos[index2]);
}
