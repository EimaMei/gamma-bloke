#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <emulator.h>
#include <ESGL/esgl.h>

#define debug_mode    0 /* Enables/disables any debug printfs. */
#define window_mode   1 /* Enables/disables any window rendering (and by extension any PPU code & rendering). */


int main(void) {
	#if window_mode == 1
	ESGL_Handle win = ESGL_window_create("Gamma Bloke", 160 * 4, 144 * 4, 128, 0, 0);
	#endif

	CPU cpu = {0};
	ROM rom;
	Gameboy gb = {0};

	rom_init("res/DMG_ROM.bin", &rom);

	rom_read_metadata(&rom);
	rom_load_into_gameboy_memory(&rom, &gb);

	printf("Starting the GameBoy emulator...\nRunning '%s' now (ROM is %i bytes large)\n====== LOG ======\n", rom.name, rom.size);

	/* cpu.PC = 0x100; */
	cpu.memory = gb.memory;
	u8* memory = gb.memory;

	#if debug_mode == 1
	int print_something = 0;
	#endif

	while (1) {
		#if window_mode == 1
		int event = ESGL_window_check_events(&win);

		if (event == 33)
			break;

		#if debug_mode == 1
		if (ESGL_window_key_is_pressed(&win, EK_1))
			print_something = 1;
		else if (ESGL_window_key_is_pressed(&win, EK_2))
			print_something = 2;
		else if (ESGL_window_key_is_pressed(&win, EK_0))
			print_something = 0;
		#endif

		/* Operations for the LCD have been provided. */
		if (gb.lcd_control != memory[REG_LLDC]) {
			gb.lcd_control = memory[REG_LLDC];

			#if debug_mode == 1
			printf(
				"LCD control - %s (%02X)\nSCY, SCX - %i, %i\nLY - %i\nLYC - %i\nWY, WX - %i, %i\n",
				toBinaryString(memory[REG_LLDC]), memory[REG_LLDC],
				memory[REG_SCY], memory[REG_SCX],
				memory[REG_LY],
				memory[REG_LYC],
				memory[REG_WX], memory[REG_WY]
			);
			#endif
		}

		if (get_bit(gb.lcd_control, 7) == 1) { /* PPU is on. */
			ESGL_draw_rect(&win, 0, 0, 256, 256, 0, 0, 255); /* The limit of the background. */
			ESGL_draw_rect(&win, 0, 0, 166, 144, 0, 255, 0); /* Visible screen. */

			/* Draw the background first. */
			u8 current_scanline = 0;
			u16 start_address = get_bit(gb.lcd_control, 4) == 0 ? BG_AND_WIN_TILE_MAP_0 : BG_AND_WIN_TILE_MAP_1;

			if (start_address == BG_AND_WIN_TILE_MAP_1) {
				i8 x = memory[REG_SCX], y = memory[REG_SCY];

				u16 start;
				for (start = start_address; start < start_address + BG_AND_WIN_TILE_MAP_LIMIT + 2; start += 16) {
					unsigned char buffer[16] = {
						memory[start +  0], memory[start +  1],
						memory[start +  2], memory[start +  3],
						memory[start +  4], memory[start +  5],
						memory[start +  6], memory[start +  7],
						memory[start +  8], memory[start +  9],
						memory[start + 10], memory[start + 11],
						memory[start + 12], memory[start + 13],
						memory[start + 14], memory[start + 15]
					};

					draw_gameboy_tile_i8(&win, buffer, memory[REG_SCX], &x, &memory[REG_LY], current_scanline);
					current_scanline++;
				}
			}

			if (get_bit(gb.lcd_control, 5) == 1) { /* Window is on. */
			}

			if (get_bit(gb.lcd_control, 1) == 1) { /* Sprites should be drawn on screen */
			}

			ESGL_window_clear(&win);
		}
		#endif


		if (cpu.PC < rom.size) {
			u8 opcode = memory[cpu.PC++];

			switch (opcode) {
				/* NOP */
				case 0x00:
					break;

				/* LD BC, nn */
				case 0x01: {
					cpu_load_u16_to_r16(&cpu, &cpu.B, &cpu.C, memory[cpu.PC + 1], memory[cpu.PC]);
					cpu.PC += 2;
					break;
				}

				/* LD (BC), A */
				case 0x02: {
					cpu_load_u8_to_memory(&cpu, cpu.A, cpu.B, cpu.C);
					break;
				}

				/* INC BC */
				case 0x03: {
					cpu_inc_r16(&cpu.B, &cpu.C);
					break;
				}

				/* INC B */
				case 0x04: {
					cpu_inc_r8(&cpu, &cpu.B);
					break;
				}

				/* DEC B */
				case 0x05: {
					cpu_dec_r8(&cpu, &cpu.B);
					break;
				}

				/* LD B,u8 */
				case 0x06: {
					cpu_load_u8_to_r8(&cpu, &cpu.B, memory[cpu.PC++]);
					break;
				}

				/* RLCA */
				case 0x07: {
					u8 last_bit = (cpu.A & 0b10000000) >> 7;
					cpu.A <<= 1;
					cpu.A |= last_bit;

					cpu_flag_set(&cpu, FLAG_ZERO, 0);
					cpu_flag_set(&cpu, FLAG_NEGATIVE, 0);
					cpu_flag_set(&cpu, FLAG_HALF_CARRY, 0);
					cpu_flag_set(&cpu, FLAG_CARRY, (last_bit == 1));

					break;
				}

				/* LD (u16),SP */
				case 0x08: {
					u16 address = get_u16_from_two_u8(memory[cpu.PC + 1], memory[cpu.PC]);

					memory[address] = high_bit(cpu.SP);
					memory[address + 1] = low_bit(cpu.SP);

					cpu.PC += 2;

					break;
				}

				/* ADD HL,BC */
				case 0x09: {
					u16 BC = get_u16_from_two_u8(cpu.B, cpu.C);
					u16 HL = get_u16_from_two_u8(cpu.H, cpu.L);
					u16 result = HL + BC;

					cpu.H = high_bit(result);
					cpu.L = low_bit(result);

					cpu_flag_set(&cpu, FLAG_NEGATIVE, 0);
					cpu_flag_set(&cpu, FLAG_HALF_CARRY, ((HL & 0xF) + (BC & 0xF) > 0xF));
					cpu_flag_set(&cpu, FLAG_CARRY, ((HL & 0xFFF) + (BC & 0xFFF) > 0xFFF));

					break;
				}

				/* LD A,(BC) */
				case 0x0A: {
					u16 BC = get_u16_from_two_u8(cpu.B, cpu.C);
					cpu.A = memory[BC];

					break;
				}

				/* DEC BC */
				case 0x0B: {
					cpu_dec_r16(&cpu.B, &cpu.C);
					break;
				}

				/* INC C */
				case 0x0C: {
					cpu_inc_r8(&cpu, &cpu.C);
					break;
				}

				/* DEC C */
				case 0x0D: {
					cpu_dec_r8(&cpu, &cpu.C);
					break;
				}

				/* LD C,u8 */
				case 0x0E: {
					cpu_load_u8_to_r8(&cpu, &cpu.C, memory[cpu.PC++]);
					break;
				}

				/* RRCA */
				/*case 0x0F: {
					cpu.Z = 0;
					cpu.N = 0;
					cpu.HALF_CARRY = 0;

					cpu.CARRY = cpu.A & 0x01;
					cpu.A = (cpu.A >> 1) | (cpu.CARRY << 7);


					break;
				}*/

				/* STOP */
				/*case 0x10: {
					break;
				}*/

				/* LD DE,u16 */
				case 0x11: {
					cpu_load_u16_to_r16(&cpu, &cpu.D, &cpu.E, memory[cpu.PC + 1], memory[cpu.PC]);
					cpu.PC += 2;
					break;
				}

				/* LD (DE),A */
				case 0x12: {
					cpu_load_u8_to_memory(&cpu, cpu.A, cpu.D, cpu.E);
					break;
				}

				/* INC DE */
				case 0x13: {
					cpu_inc_r16(&cpu.D, &cpu.E);
					break;
				}

				/* INC D*/
				case 0x14: {
					cpu_dec_r8(&cpu, &cpu.E);
					break;
				}

				/* DEC D */
				case 0x15: {
					cpu_dec_r8(&cpu, &cpu.D);
					break;
				}

				/* LD D,u8 */
				case 0x16: {
					cpu_load_u8_to_r8(&cpu, &cpu.D, memory[cpu.PC++]);
					break;
				}

				/* JR i8 */
				case 0x18: {
					i8 value = (i8)memory[cpu.PC++];
					cpu.PC += value;
					break;
				}

				/* LD E,u8 */
				case 0x1E: {
					cpu_load_u8_to_r8(&cpu, &cpu.E, memory[cpu.PC++]);
					break;
				}

				/* ... */

				/* RLA */
				case 0x17: {
					u8 last_bit = (cpu.A & 0b10000000) >> 7;
					cpu.A <<= 1;
					cpu.A |= (cpu.F >> 4) & 0x1;

					cpu_flag_set(&cpu, FLAG_ZERO, 0);
					cpu_flag_set(&cpu, FLAG_NEGATIVE, 0);
					cpu_flag_set(&cpu, FLAG_HALF_CARRY, 0);
					cpu_flag_set(&cpu, FLAG_CARRY, (last_bit == 1));

					break;
				}

				/* LD A,(DE) */
				case 0x1A: {
					u16 address = get_u16_from_two_u8(cpu.D, cpu.E);
					cpu.A = memory[address];

					break;
				}

				/* DEC E */
				case 0x1D: {
					cpu_dec_r8(&cpu, &cpu.E);
					break;
				}

				/* JR NZ, i8 */
				case 0x20: {
					i8 value = (i8)memory[cpu.PC++];

					if (get_bit(cpu.F, 7) == 0)
						cpu.PC += value;

					break;
				}

				/* LD HL,u16*/
				case 0x21: {
					cpu_load_u16_to_r16(&cpu, &cpu.H, &cpu.L, memory[cpu.PC + 1], memory[cpu.PC]);
					cpu.PC += 2;
					break;
				}

				/* LD (HL+),A*/
				case 0x22: {
					u16 address = get_u16_from_two_u8(cpu.H, cpu.L);
					memory[address] = cpu.A;

					cpu.L++;
					if (cpu.L == 0x00)
						cpu.H++;

					break;
				}

				/* INC HL */
				case 0x23: {
					cpu_inc_r16(&cpu.H, &cpu.L);
					break;
				}

				/* INC H */
				case 0x24: {
					cpu_inc_r8(&cpu, &cpu.H);
					break;
				}

				/* JR Z, i8 */
				case 0x28: {
					i8 value = (i8)memory[cpu.PC++];

					if (get_bit(cpu.F, 7) == 1)
						cpu.PC += value;

					break;
				}

				/* LD L,u8 */
				case 0x2E: {
					cpu_load_u8_to_r8(&cpu, &cpu.L, memory[cpu.PC++]);
					break;
				}

				/* LD SP,u16*/
				case 0x31: {
					cpu_load_u16_to_sp(&cpu, memory[cpu.PC + 1], memory[cpu.PC]);
					cpu.PC += 2;
					break;
				}

				/* LD (HL-),A*/
				case 0x32: {
					u16 address = get_u16_from_two_u8(cpu.H, cpu.L);
					memory[address] = cpu.A;

					cpu.L--;
					if (cpu.L == 0xFF)
						cpu.H--;

					break;
				}

				/* LD A,u8 */
				case 0x3E: {
					cpu_load_u8_to_r8(&cpu, &cpu.A, memory[cpu.PC++]);
					break;
				}

				/* DEC A */
				case 0x3D: {
					cpu_dec_r8(&cpu, &cpu.A);
					break;
				}

				/* LD C,A */
				case 0x4F: {
					cpu_load_u8_to_r8(&cpu, &cpu.C, cpu.A);
					break;
				}

				/* LD D,A */
				case 0x57: {
					cpu_load_u8_to_r8(&cpu, &cpu.D, cpu.A);
					break;
				}

				/* LD H,A */
				case 0x67: {
					cpu_load_u8_to_r8(&cpu, &cpu.H, cpu.A);
					break;
				}

				/* LD (HL),A*/
				case 0x77: {
					u16 address = get_u16_from_two_u8(cpu.H, cpu.L);
					memory[address] = cpu.A;
					break;
				}

				/* LD A,E */
				case 0x7B: {
					cpu_load_u8_to_r8(&cpu, &cpu.A, cpu.E);
					break;
				}

				/* LD A,H */
				case 0x7C: {
					cpu_load_u8_to_r8(&cpu, &cpu.A, cpu.H);
					break;
				}

				/* ADD A,(HL) */
				case 0x86: {
					u8 value = memory[get_u16_from_two_u8(cpu.H, cpu.L)];
					u8 result = cpu.A + value;

					cpu_flag_set(&cpu, FLAG_ZERO, (cpu.A == 0));
					cpu_flag_set(&cpu, FLAG_NEGATIVE, 0);
					cpu_flag_set(&cpu, FLAG_HALF_CARRY, (((cpu.A & 0xF) + (value & 0xF)) > 0xF));
					cpu_flag_set(&cpu, FLAG_CARRY, (((cpu.A & 0xF) + (value & 0xF)) > 0xF));

					cpu.A = result;

					break;
				}

				/* SUB A,B */
				case 0x90: {
					cpu.A -= cpu.B;

					cpu_flag_set(&cpu, FLAG_ZERO, (cpu.A == 0));
					cpu_flag_set(&cpu, FLAG_NEGATIVE, 1);
					cpu_flag_set(&cpu, FLAG_HALF_CARRY, ((cpu.A & 0x0F) < (cpu.B & 0x0F)));
					cpu_flag_set(&cpu, FLAG_CARRY, (cpu.A < cpu.B));

					break;
				}

				/* XOR A,A*/
				case 0xAF: {
					cpu.A = (cpu.A ^ cpu.A);

					cpu_flag_set(&cpu, FLAG_ZERO, (cpu.A == 0));
					cpu_flag_set(&cpu, FLAG_NEGATIVE, 0);
					cpu_flag_set(&cpu, FLAG_HALF_CARRY, 0);
					cpu_flag_set(&cpu, FLAG_CARRY, 0);

					break;
				}

				/* CP A,(HL) */
				case 0xBE: {
					u8 value = memory[get_u16_from_two_u8(cpu.H, cpu.L)];
					u8 result = cpu.A - value;

					cpu_flag_set(&cpu, FLAG_ZERO, (result == 0));
					cpu_flag_set(&cpu, FLAG_NEGATIVE, 1);
					cpu_flag_set(&cpu, FLAG_HALF_CARRY, (cpu.A & 0x0F) < (value & 0x0F));
					cpu_flag_set(&cpu, FLAG_CARRY, (cpu.A < value));

					break;
				}

				/* POP BC */
				case 0xC1: {
					cpu.C = memory[cpu.SP++];
					cpu.B = memory[cpu.SP++];
					break;
				}

				/* PUSH BC */
				case 0xC5: {
					cpu.SP--;
					memory[cpu.SP] = cpu.B;
					cpu.SP--;
					memory[cpu.SP] = cpu.C;
					break;
				}

				/* PREFIX CB */
				case 0xCB: {
					int res = prefix_cb(&cpu, memory);
					if (res == -1)
						return 1;

					break;
				}

				/* RET */
				case 0xC9: {
					cpu.PC = get_u16_from_two_u8(memory[cpu.SP + 1], memory[cpu.SP]);
					cpu.SP += 2;
					break;
				}

				/* CALL u16 */
				case 0xCD: {
					u16 nn = get_u16_from_two_u8(memory[cpu.PC + 1], memory[cpu.PC]);
					cpu.PC += 2;

					cpu.SP--;
					memory[cpu.SP] = high_bit(cpu.PC);
					cpu.SP--;
					memory[cpu.SP] = low_bit(cpu.PC);

					cpu.PC = nn;

					break;
				}

				/* LD (FF00+u8),A */
				case 0xE0: {
					cpu_load_u8_to_address_ff00_plus_u8(&cpu, memory[cpu.PC++], cpu.A);
					break;
				}

				/* LD (FF00+C),A */
				case 0xE2: {
					cpu_load_u8_to_address_ff00_plus_u8(&cpu, cpu.C, cpu.A);
					break;
				}

				/* LD (u16),A */
				case 0xEA: {
					memory[get_u16_from_two_u8(memory[cpu.PC + 1], memory[cpu.PC])] = cpu.A;
					cpu.PC += 2;

					break;
				}

				/* LD A,(FF00+u8) */
				case 0xF0: {
					cpu.A = memory[0xFF00 + memory[cpu.PC++]];
					break;
				}

				/* CP A,u8 */
				case 0xFE: {
					u8 value = memory[cpu.PC++];
					u8 result = cpu.A - value;

					cpu_flag_set(&cpu, FLAG_ZERO, (result == 0));
					cpu_flag_set(&cpu, FLAG_NEGATIVE, 1);
					cpu_flag_set(&cpu, FLAG_HALF_CARRY, (cpu.A & 0x0F) < (value & 0x0F));
					cpu_flag_set(&cpu, FLAG_CARRY, (cpu.A < value));

					break;
				}

				/* ... other opcodes will be added here. */
				default:
					printf("Error: Invalid opcode: '0x%0X' at %i\n", opcode, cpu.PC);
					return 1;
			}

			#if debug_mode == 1
			if (print_something > 0) {
				printf(
					"0x%04X/%04i: 0x%02X\n\tA: %02X\n\tB: %02X (BC: %04X)\n\tC: %02X\n\tD: %02X (DE: %04X)\n\tE: %02X\n\tF: (Z = %i, N = %i, H = %i, C = %i)\n\tH: %02X (HL: %04X)\n\tL: %02X\n\tPC: %04X, SP: %04X\n",
					cpu.PC, cpu.PC, opcode,
					cpu.A,
					cpu.B, get_u16_from_two_u8(cpu.B, cpu.C),
					cpu.C,
					cpu.D, get_u16_from_two_u8(cpu.D, cpu.E),
					cpu.E,
					(((cpu.F >> 7) & 1) ? 1 : 0), (((cpu.F >> 6) & 1) ? 1 : 0), (((cpu.F >> 5) & 1) ? 1 : 0), (((cpu.F >> 4) & 1) ? 1 : 0),
					cpu.H, get_u16_from_two_u8(cpu.H, cpu.L),
					cpu.L,
					cpu.PC, cpu.SP
				);
				if (print_something >= 2)
					system("pause");
			}
			#endif
		}
		else {
			printf("Finished running '%s'!\n", rom.name);
			break;
		}
		/*Sleep(1000 / 60);*/
	}
	ESGL_window_close(&win);

	return 0;
}