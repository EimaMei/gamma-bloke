#include <emulator.h>


void cpu_load_u16_to_r16(CPU* cpu, unsigned char* memory, uint8* high, uint8* low) {
	*high = memory[cpu->PC++];
	*low = memory[cpu->PC++];
}



void cpu_load_u16_to_s16(CPU* cpu, unsigned char* memory, uint16* special) {
	*special = get_u16_from_two_u8(memory[cpu->PC + 2], memory[cpu->PC + 1]);
	cpu->PC += 2;
}


void cpu_load_u8_to_memory(unsigned char* memory, uint32 value, uint8 high, uint8 low) {
	memory[(high << 8) + low] = value;
}


void cpu_inc_r16(uint8* high, uint8* low) {
	(*low)++;

	if (low == 0)
		(*high)++;
}


void cpu_dec_r16(uint8* high, uint8* low) {
	(*low)--;

	if (low == 0)
		(*high)--;
}


void cpu_inc_r8(CPU* cpu, uint8* low) {
	(*low)++;

	cpu->Z = (*low == 0);
	cpu->N = 0;
	cpu->H = (*low & 0x0F);
}


void cpu_dec_r8(CPU* cpu, uint8* low) {
	(*low)--;

	cpu->Z = (*low == 0);
	cpu->N = 1;
	cpu->H = (*low & 0x0F);
}