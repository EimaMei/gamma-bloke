#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <emulator.h>
#include <esgl.h>


int main(void) {
	ESGL_Handle window = ESGL_window_create("Gamma Bloke", 160 * 4, 144 * 4, 255, 0, 0);

	CPU cpu = {0};
	ROM rom;
	Gameboy gb;

	rom_init("res/DMG_ROM.bin", &rom);

	rom_read_metadata(&rom);
	rom_load_into_gameboy_memory(&rom, &gb);

	printf("Starting the GameBoy emulator...\nRunning '%s' now (ROM is %i bytes large)\n====== LOG ======\n", rom.name, rom.size);

	/* cpu.PC = 0x0150; */
	unsigned char* memory = gb.memory;
	int printSomething = 0;

	while (cpu.PC < rom.size) {
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
				cpu_load_u8_to_memory(memory, cpu.A, cpu.B, cpu.C);
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
				cpu.B = memory[cpu.PC++];
				break;
			}

			/* RLCA */
			case 0x07: {
				cpu.Z = 0;
				cpu.N = 0;
				cpu.HALF_CARRY = 0;

				cpu.CARRY = cpu.A & 0x01;
				cpu.A = (cpu.A << 1) | (cpu.CARRY >> 7);
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

				cpu.H = (result & 0xFF00) >> 8;
				cpu.L = result & 0x00FF;

				cpu.N = 0;
				cpu.HALF_CARRY = (result > 0xFFFF);
				cpu.CARRY = (((HL & 0xFFF) + (BC & 0xFFF)) > 0xFFF);

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
				cpu_load_u8_to_r8(&cpu, memory, &cpu.C);
				break;
			}

			/* RRCA */
			case 0x0F: {
				cpu.Z = 0;
				cpu.N = 0;
				cpu.HALF_CARRY = 0;

				cpu.CARRY = cpu.A & 0x01;
				cpu.A = (cpu.A >> 1) | (cpu.CARRY << 7);

				break;
			}

			/* STOP */
			case 0x10: {
				/* Does... something? */
				break;
			}

			/* LD DE,u16 */
			case 0x11: {
				cpu_load_u16_to_r16(&cpu, &cpu.D, &cpu.E, memory[cpu.PC + 1], memory[cpu.PC]);
				cpu.PC += 2;
				break;
			}

			/* LD (DE),A */
			case 0x12: {
				cpu_load_u8_to_memory(memory, cpu.A, cpu.D, cpu.E);
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

			/* ... */

			/* RLA */
			case 0x17: {
				cpu.A <<= cpu.CARRY;

				cpu.Z = 0;
				cpu.N = 0;
				cpu.H = 0;
				cpu.CARRY = (cpu.A & 0x01);

				/* printf("Warning: '0xCB' prefixed '0x17' opcode needs fixing.\n"); */
				break;
			}

			/* LD A,(DE) */
			case 0x1A: {
				u16 address = get_u16_from_two_u8(cpu.D, cpu.E);
				cpu.A = memory[address];

				break;
			}

			/* JR NZ, i8 */
			case 0x20: {
				i8 value = (i8)memory[cpu.PC++];

				if (cpu.Z == 0)
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
				cpu_load_u8_to_r8(&cpu, memory, &cpu.A);
				break;
			}

			/* LD C,A */
			case 0x4F: {
				cpu.C = cpu.A;
				break;
			}

			/* LD (HL),A*/
			case 0x77: {
				u16 address = get_u16_from_two_u8(cpu.H, cpu.L);
				memory[address] = cpu.A;
				break;
			}

			/* XOR A,A*/
			case 0xAF: {
				cpu.A ^= cpu.A;

				cpu.Z = (cpu.A == 0);
				cpu.N = 0;
				cpu.HALF_CARRY = 0;
				cpu.CARRY = 0;
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
				u16 address = cpu.SP;
				cpu.SP += 2;
				cpu.PC = ((memory[address + 1] << 8) | memory[address]);

				break;
			}

			/* CALL u16 */
			case 0xCD: {
				u16 nn = get_u16_from_two_u8(memory[cpu.PC + 1], memory[cpu.PC]);

				memory[cpu.SP--] = high_bit(nn);
				memory[cpu.SP--] = low_bit(nn);

				cpu.PC = nn;

				break;
			}

			/* LD (FF00+u8),A */
			case 0xE0: {
				memory[0xFF00 + memory[cpu.PC++]] = cpu.A;

				break;
			}

			/* LD (FF00+C),A */
			case 0xE2: {
				memory[0xFF00 + cpu.C] = cpu.A;

				break;
			}

			/* ... other opcodes will be added here. */
			default:
				printf("Error: Invalid opcode: '0x%0X' at %i\n", opcode, cpu.PC);
				return 1;
		}

		#if debug_mode == 1
		if (opcode == 0x4F)
			printSomething = 1;

		if (printSomething == 1)
			printf("0x%02X\n", opcode, cpu.PC);
		#endif
	}
	printf("Finished running '%s'!\n", rom.name);

	while (1) {
		int event = ESGL_window_check_events(&window);

		if (event == 33)
			break;

		ESGL_window_clear(&window);
	}
	ESGL_window_close(&window);

	return 0;
}