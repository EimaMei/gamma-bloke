#include <emulator.h>


void cpu_flag_set(CPU* cpu, u8 flag, u8 boolean_result) {
	if (boolean_result == 1)
		cpu->F |= flag;
	else
		cpu->F &= ~flag;
}


void cpu_load_u16_to_r16(CPU* cpu, u8* r8_high_bit, u8* r8_low_bit, u8 value_high_bit, u8 value_low_bit) {
	*r8_high_bit = value_high_bit;
	*r8_low_bit = value_low_bit;
}


void cpu_load_u16_to_sp(CPU* cpu, u8 value_high_bit, u8 value_low_bit) {
	cpu->SP = get_u16_from_two_u8(value_high_bit, value_low_bit);
}


void cpu_load_u8_to_memory(CPU* cpu, u8 value, u8 high, u8 low) {
	cpu->memory[(high << 8) + low] = value;
}


void cpu_load_u8_to_r8(CPU* cpu, u8* r8, u8 value) {
	*r8 = value;
}


void cpu_load_u8_to_address_ff00_plus_u8(CPU* cpu, u8 u8_low_bit_adress, u8 value) {
	cpu->memory[0xFF00 + u8_low_bit_adress] = value;
}

void cpu_inc_r16(u8* high, u8* low) {
	(*low)++;

	if (*low == 0)
		(*high)++;
}


void cpu_dec_r16(u8* high, u8* low) {
	(*low)--;

	if (*low == 0xFF)
		(*high)--;
}


void cpu_inc_r8(CPU* cpu, u8* low) {
	(*low)++;

	cpu_flag_set(cpu, FLAG_ZERO, (*low == 0));
	cpu_flag_set(cpu, FLAG_NEGATIVE, 0);
	cpu_flag_set(cpu, FLAG_HALF_CARRY, (*low & 0x0F));
}


void cpu_dec_r8(CPU* cpu, u8* low) {
	(*low)--;

	cpu_flag_set(cpu, FLAG_ZERO, (*low == 0));
	cpu_flag_set(cpu, FLAG_NEGATIVE, 1);
	cpu_flag_set(cpu, FLAG_HALF_CARRY, (*low & 0x0F));
}



int prefix_cb(CPU* cpu, unsigned char* memory) {
	u8 opcode = memory[cpu->PC++];

	switch (opcode) {
		/* RL C */
		case 0x11: {
			u8 last_bit = (cpu->C & 0b10000000) >> 7;
			cpu->C = (cpu->C << 1) | last_bit;

			cpu_flag_set(cpu, FLAG_ZERO, (cpu->C == 0));
			cpu_flag_set(cpu, FLAG_NEGATIVE, 0);
			cpu_flag_set(cpu, FLAG_HALF_CARRY, (last_bit != (cpu->C & 0x08)));
			cpu_flag_set(cpu, FLAG_CARRY, (last_bit == 1));

			break;
		}

		/* BIT 7,H */
		case 0x7C: {
			cpu_flag_set(cpu, FLAG_ZERO, ((cpu->H & (1 << 7)) == 0));
			cpu_flag_set(cpu, FLAG_NEGATIVE, 0);
			cpu_flag_set(cpu, FLAG_HALF_CARRY, 1);

			break;
		}

		/* ... other opcodes will be added here. */
		default:
			printf("Error: Invalid '0xCB' opcode: '0x%0X' at %i\n", opcode, cpu->PC);
			return -1;
	}

	return 0;
}