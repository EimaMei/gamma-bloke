#include <stdio.h>
#include <stdlib.h>

#define uint8 unsigned char
#define uint16 unsigned short
#define uint32 unsigned int

#define high_bit(value) ((value & 0xFF00) >> 8)
#define low_bit(value) (value & 0x00FF)

#define get_u16_from_two_u8(first, second) (uint16)((first << 8) | second)


typedef struct Gameboy {
    uint8 memory[0xFFFF];
} Gameboy;


typedef struct CPU {
    uint8 A, B, C, D, E, F, H, L; /* Regular 8-bit registers (r8). */
    uint16 PC, SP; /* Regular 16-bit registers (r16/s18). */
    uint8 Z, N, CARRY; /* Special registers. */
} CPU;


typedef struct ROM {
    const uint8* data; /* The actual ROM. */
    uint32 size; /* Size of the ROM in bytes. */

    const char* name; /* Name of the ROM. */
} ROM;


/* Reads a file and turns its data to ROM. */
void rom_init(const char* filename, ROM* output);
/* Reads the metadata of the ROM. */
void rom_read_metadata(ROM* rom);
/* Loads the ROM into the GameBoy's memory. */
void rom_load_into_gameboy_memory(ROM* rom, Gameboy* gb);

/* Loads an uint16 into a regular 16-bit register. */
void cpu_load_u16_to_r16(CPU* cpu, unsigned char* memory, uint8* high, uint8* low);
/* loads an uint16 into a special 16-bit register (mostly just SP). */
void cpu_load_u16_to_s16(CPU* cpu, unsigned char* memory, uint16* special);
/* Loads an uint8 into the GameBoy's memory. */
void cpu_load_u8_to_memory(unsigned char* memory, uint32 value, uint8 high, uint8 low);

/* Increments 16-bit register by one. */
void cpu_inc_r16(uint8* high, uint8* low);
/* Increments 8-bit register by one. */
void cpu_inc_r8(CPU* cpu, uint8* low);
/* Decrements 16-bit register by one. */
void cpu_dec_r16(uint8* high, uint8* low);
/* Increments 8-bit register by one. */
void cpu_dec_r8(CPU* cpu, uint8* low);