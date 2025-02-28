#define A4_VALUE 1181 // 440Hz * 2^17 / 48828.125
#define B4_VALUE 1326
#define G4_VALUE 1052

#define PULSE 0
#define SAWTOOTH 1
#define TRIANGLE 2
#define NOISE 3

#define RIGHT_LEFT 3

#define PSG_REGISTER_BASE 0x1f9c0

#define EXPLOSION_SOUND_LENGTH 30 // explosion sound is 30 frames long

extern void init_sound();
extern void update_sounds();