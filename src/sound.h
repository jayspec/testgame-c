#define A4_VALUE 1181 // 440Hz * 2^17 / 48828.125
#define B4_VALUE 1326
#define G4_VALUE 1052 

#define PSG_REGISTER_BASE 0x1f9c0

extern void init_sound();
extern void update_sounds();