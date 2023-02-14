#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emulator.h>


int main(void) {
	CPU cpu = {0};
	ROM rom;
	Gameboy gb;

	rom_init("res/DMG_ROM.bin", &rom);

	rom_read_metadata(&rom);
	rom_load_into_gameboy_memory(&rom, &gb);

	printf("Starting the GameBoy emulator...\nRunning '%s' now (ROM is %i bytes large)\n====== LOG ======\n", rom.name, rom.size);

	/* cpu.PC = 0x0150; */
	unsigned char* memory = gb.memory;

	while (cpu.PC < rom.size) {
		uint8 opcode = memory[cpu.PC++];

		switch (opcode) {
			/* NOP */
			case 0x00:
				break;

			/* LD BC, nn */
			case 0x01: {
				cpu_load_u16_to_r16(&cpu, memory, &cpu.B, &cpu.C);
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

			/* LD B, n */
			case 0x06: {
				cpu.B = memory[cpu.PC++];
				break;
			}

			/* RLCA */
			case 0x07: {
				cpu.Z = 0;
				cpu.N = 0;
				cpu.H = 0;

				cpu.CARRY = cpu.A & 0x01;
				cpu.A = (cpu.A << 1) | (cpu.CARRY >> 7);
				break;
			}

			/* LD (u16),SP */
			case 0x08: {
				uint16 address = get_u16_from_two_u8(memory[cpu.PC + 2], memory[cpu.PC + 1]);

				memory[address] = high_bit(cpu.SP);
				memory[address + 1] = low_bit(cpu.SP);

				cpu.PC += 2;

				break;
			}

			/* ADD HL,BC */
			case 0x09: {
				uint16 BC = get_u16_from_two_u8(cpu.B, cpu.C);
				uint16 HL = get_u16_from_two_u8(cpu.H, cpu.L);
				uint16 result = HL + BC;

				cpu.H = (result & 0xFF00) >> 8;
				cpu.L = result & 0x00FF;

				cpu.N = 0;
				cpu.H = (result > 0xFFFF);
				cpu.CARRY = (((HL & 0xFFF) + (BC & 0xFFF)) > 0xFFF);

				break;
			}

			/* LD A,(BC) */
			case 0x0A: {
				uint16 BC = get_u16_from_two_u8(cpu.B, cpu.C);
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
				cpu.C = memory[cpu.PC++];

				break;
			}

			/* RRCA */
			case 0x0F: {
				cpu.Z = 0;
				cpu.N = 0;
				cpu.H = 0;

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
				cpu_load_u16_to_r16(&cpu, memory, &cpu.D, &cpu.E);
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

			/* JR NZ, i8 */
			case 0x20: {
				printf("Warning: Unimplemented opcode: '0x%0x'\n", opcode);
				break;
			}

			/* LD HL,u16*/
			case 0x21: {
				cpu_load_u16_to_r16(&cpu, memory, &cpu.H, &cpu.L);
				break;
			}

			/* LD SP,u16*/
			case 0x31: {
				cpu_load_u16_to_s16(&cpu, memory, &cpu.SP);
				break;
			}

			/* LD (HL-),A*/
			case 0x32: {
				uint16 address = get_u16_from_two_u8(cpu.H, cpu.L);
				memory[address] = cpu.A;

				cpu.L--;
				if (cpu.L == 0xFF)
					cpu.H--;

				break;
			}

			/* XOR A,A*/
			case 0xAF: {
				cpu.A ^= cpu.A;

				cpu.Z = (cpu.A == 0);
				cpu.N = 0;
				cpu.H = 0;
				cpu.CARRY = 0;
				break;
			}

			/* PREFIX CB*/
			case 0xCB: {
				printf("Warning: Unimplemented opcode: '0x%0x'\n", opcode);
				cpu.PC++;
				break;
			}

			/* ... other opcodes will be added here. */
			default:
				printf("Error: Invalid opcode: '0x%0X' at %i\n", opcode, cpu.PC);
				return 1;
		}
	}

	return 0;
}