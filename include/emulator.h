#pragma once

#include <stdio.h>
#include <stdlib.h>


/* Useful types. */
#define i8 char /* Signed 8-bit integer */
#define i16 short /* Signed 16-bit integer */
#define i32 int  /* Signed 32-bit integer */

#define u8 unsigned char /* Unsigned 8-bit integer */
#define u16 unsigned short  /* Unsigned 16-bit integer */
#define u32 unsigned int  /* Unsigned 32-bit integer */

typedef void* ESGL_Handle; /* I don't really wanna intertwine both 'emulator.h' and 'esgl.h', so I am just redeclaring ESGL_Handle as a void* */

/* Msb/lsb macros. */
#define high_bit(value) ((value & 0xFF00) >> 8)
#define low_bit(value) (value & 0x00FF)

#define get_u16_from_two_u8(first, second) (u16)((first << 8) | second)
#define get_bit(value, bit) ((value & ( 1 << bit )) >> bit)

/* Register 'F' flags. */
#define FLAG_ZERO         (1 << 7)
#define FLAG_NEGATIVE     (1 << 6)
#define FLAG_HALF_CARRY   (1 << 5)
#define FLAG_CARRY        (1 << 4)

/* VRAM block start adresses. */
#define VRAM_BLOCK_0      0x8000
#define VRAM_BLOCK_1      0x8800
#define VRAM_BLOCK_2      0x9000
#define VRAM_LIMIT        0x0FFF

/* Tile map areas' start adresses. */
#define TILE_MAP_0     0x9800
#define TILE_MAP_1     0x9C00
#define TILE_MAP_LIMIT 0x03FF

/* Tile map areas' start adresses. */
#define BG_AND_WIN_TILE_MAP_0     0x8800
#define BG_AND_WIN_TILE_MAP_1     0x8000
#define BG_AND_WIN_TILE_MAP_LIMIT 0x0FFF

/* Hardware registers. */
#define REG_LLDC          0xFF40 /* LCD control. */
#define REG_STAT          0xFF41 /* LCD status. */
#define REG_SCY           0xFF42 /* Viewport Y position. */
#define REG_SCX           0xFF43 /* Viewport X position. */
#define REG_LY            0xFF44 /* LCD Y position. */
#define REG_LYC           0xFF45 /* LY (LCD Y) compare. */

#define REG_WY            0xFF4A /* Window Y position. */
#define REG_WX            0xFF4B /* Window X position + 7. */



typedef struct Gameboy {
    u8 memory[0xFFFF]; /* RAM of the system. */

    /*
    Contains all info about how the LCD should be displayed.

    Bit             Name                            What the bit means.
    7	    LCD and PPU enable	                0=Off, 1=On
    6	    Window tile map area	            0=9800-9BFF, 1=9C00-9FFF
    5	    Window enable	                    0=Off, 1=On
    4	    BG and Window tile data area	    0=8800-97FF, 1=8000-8FFF
    3	    BG tile map area	                0=9800-9BFF, 1=9C00-9FFF
    2	    OBJ size	                        0=8x8, 1=8x16
    1	    OBJ enable	                        0=Off, 1=On
    0	    BG and Window enable/priority	    0=Off, 1=On

    Source: https://gbdev.io/pandocs/LCDC.html#ff40--lcdc-lcd-control
    */
    u8 lcd_control;
} Gameboy;


typedef struct CPU {
    u8 A, B, C, D, E, F, H, L; /* Regular 8-bit registers (r8). */
    u16 PC, SP; /* Regular 16-bit registers (r16/s18). */
    u8* memory; /* Pointer to the RAM. */
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

/* Sets the flag of register F depending on the boolean (0 - unset, 1 - set). */
void cpu_flag_set(CPU* cpu, u8 flag, u8 boolean_result);

/* Loads an u16 into a regular 16-bit register. */
void cpu_load_u16_to_r16(CPU* cpu, u8* r8_high_bit, u8* r8_low_bit, u8 value_high_bit, u8 value_low_bit);
/* loads an u16 into a special 16-bit register (mostly just SP). */
void cpu_load_u16_to_sp(CPU* cpu, u8 value_high_bit, u8 value_low_bit);
/* Loads an u8 into the GameBoy's memory. */
void cpu_load_u8_to_memory(CPU* cpu,  u8 value, u8 high, u8 low);
/* Loads an u8 into an r8. */
void cpu_load_u8_to_r8(CPU* cpu, u8* r8, u8 value);
/* Loads an u8 to the value pointed by memory address [0xFF00 + u8]. */
void cpu_load_u8_to_address_ff00_plus_u8(CPU* cpu, u8 u8_low_bit_adress, u8 value);

/* Increments 16-bit register by one. */
void cpu_inc_r16(u8* high, u8* low);
/* Increments 8-bit register by one. */
void cpu_inc_r8(CPU* cpu, u8* low);
/* Decrements 16-bit register by one. */
void cpu_dec_r16(u8* high, u8* low);
/* Decrements 8-bit register by one. */
void cpu_dec_r8(CPU* cpu, u8* low);

/* Deals with PREFIX 0xCB opcodes. */
int prefix_cb(CPU* cpu, unsigned char* memory);



/* ====== PPU/GPU related functions ====== */

/* Draws an 8x8 16 byted GameBoy tile to the screen (X and Y values are i8 typed). */
void draw_gameboy_tile_i8(ESGL_Handle* window, unsigned char* buffer, u8 SCX, i8* x, i8* y, u8 scanline);



/* ====== Misc stuff ======*/

/* Takes an u8 int and outputs a base 2 (binary) version of it. Useful for easily finding out what information the value's carring. */
char* debug_u8_to_binary(u8 value);