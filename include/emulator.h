#pragma once

#include <stdio.h>
#include <stdlib.h>
#define debug_mode 0

#define i8 char /* Signed 8-bit integer */
#define i16 short /* Signed 16-bit integer */
#define i32 int  /* Signed 32-bit integer */

#define u8 unsigned char /* Unsigned 8-bit integer */
#define u16 unsigned short  /* Unsigned 16-bit integer */
#define u32 unsigned int  /* Unsigned 32-bit integer */

#define high_bit(value) ((value & 0xFF00) >> 8)
#define low_bit(value) (value & 0x00FF)

#define get_u16_from_two_u8(first, second) (u16)((first << 8) | second)


typedef struct Gameboy {
    u8 memory[0xFFFF];
} Gameboy;


typedef struct CPU {
    u8 A, B, C, D, E, F, H, L; /* Regular 8-bit registers (r8). */
    u16 PC, SP; /* Regular 16-bit registers (r16/s18). */
    u8 Z, N, HALF_CARRY, CARRY; /* Special registers. */
} CPU;


typedef struct ROM {
    const u8* data; /* The actual ROM. */
    u32 size; /* Size of the ROM in bytes. */

    const char* name; /* Name of the ROM. */
} ROM;


/* ====== ROM related functions ====== */

/* Reads a file and turns its data to ROM. */
void rom_init(const char* filename, ROM* output);
/* Reads the metadata of the ROM. */
void rom_read_metadata(ROM* rom);
/* Loads the ROM into the GameBoy's memory. */
void rom_load_into_gameboy_memory(ROM* rom, Gameboy* gb);


/* ====== CPU related functions ====== */

/* Loads an u16 into a regular 16-bit register. */
void cpu_load_u16_to_r16(CPU* cpu, u8* r8_high_bit, u8* r8_low_bit, u8 value_high_bit, u8 value_low_bit);
/* loads an u16 into a special 16-bit register (mostly just SP). */
void cpu_load_u16_to_sp(CPU* cpu, u8 value_high_bit, u8 value_low_bit);
/* Loads an u8 into the GameBoy's memory. */
void cpu_load_u8_to_memory(unsigned char* memory, u8 value, u8 high, u8 low);
/* fd*/
void cpu_load_u8_to_r8(CPU* cpu, unsigned char* memory, u8* r8);

/* Increments 16-bit register by one. */
void cpu_inc_r16(u8* high, u8* low);
/* Increments 8-bit register by one. */
void cpu_inc_r8(CPU* cpu, u8* low);
/* Decrements 16-bit register by one. */
void cpu_dec_r16(u8* high, u8* low);
/* Increments 8-bit register by one. */
void cpu_dec_r8(CPU* cpu, u8* low);

/* Deals with PREFIX 0xCB opcodes. */
int prefix_cb(CPU* cpu, unsigned char* memory);