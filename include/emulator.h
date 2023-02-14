#include <stdio.h>
#include <stdlib.h>

#define uint8 unsigned char
#define uint16 unsigned short
#define uint unsigned int


typedef struct Gameboy {
    uint8 memory[0xFFFF];
} Gameboy;


typedef struct CPU {
    uint8 A, B, C, D, E, F, H, L;
    uint16 PC, SP;
    uint8 Z, N;
} CPU;


typedef struct ROM {
    uint8* data;
    uint size;

    const char* name;
} ROM;