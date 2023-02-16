#include <emulator.h>


void cpu_load_u16_to_r16(CPU* cpu, u8* r8_high_bit, u8* r8_low_bit, u8 value_high_bit, u8 value_low_bit) {
	*r8_high_bit = value_high_bit;
	*r8_low_bit = value_low_bit;
}


void cpu_load_u16_to_sp(CPU* cpu, u8 value_high_bit, u8 value_low_bit) {
	cpu->SP = get_u16_from_two_u8(value_high_bit, value_low_bit);
}


void cpu_load_u8_to_memory(unsigned char* memory, u8 value, u8 high, u8 low) {
	memory[(high << 8) + low] = value;
}


void cpu_load_u8_to_r8(CPU* cpu, unsigned char* memory, u8* r8) {
	*r8 = memory[cpu->PC++];
}


void cpu_inc_r16(u8* high, u8* low) {
	(*low)++;

	if (low == 0)
		(*high)++;
}


void cpu_dec_r16(u8* high, u8* low) {
	(*low)--;

	if (low == 0)
		(*high)--;
}


void cpu_inc_r8(CPU* cpu, u8* low) {
	(*low)++;

	cpu->Z = (*low == 0);
	cpu->N = 0;
	cpu->HALF_CARRY = (*low & 0x0F);
}


void cpu_dec_r8(CPU* cpu, u8* low) {
	(*low)--;

	cpu->Z = (*low == 0);
	cpu->N = 1;
	cpu->HALF_CARRY = (*low & 0x0F);
}



int prefix_cb(CPU* cpu, unsigned char* memory) {
	u8 opcode = memory[cpu->PC++];

	switch (opcode) {
		/* RL C */
		case 0x11: {
			cpu->C <<= 1;

			cpu->Z = (cpu->C == 0);
			cpu->N = 0;
			cpu->HALF_CARRY = 0;
			cpu->CARRY = (cpu->C & 0x01) | (cpu->CARRY);

			/*printf("Warning: '0xCB' prefixed '0x11' opcode needs fixing.\n"); */
			break;
		}

		/* BIT 7,H */
		case 0x7C: {
			cpu->Z = ((cpu->H & (1 << 7)) == 0);
			cpu->N = 0;
			cpu->HALF_CARRY = 1;

			break;
		}

		/* ... other opcodes will be added here. */
		default:
			printf("Error: Invalid '0xCB' opcode: '0x%0X' at %i\n", opcode, cpu->PC);
			return -1;
	}

	return 0;
}