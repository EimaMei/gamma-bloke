#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <emulator.h>


void cpu_init(CPU* cpu) {
	cpu->A = 0x00;
	cpu->B = 0x00;
	cpu->C = 0x00;
	cpu->D = 0x00;
	cpu->E = 0x00;
	cpu->F = 0x00;
	cpu->H = 0x00;
	cpu->L = 0x00;

	cpu->PC = 0x0000;
	cpu->SP = 0x0000;

	cpu->Z = 0x00;
	cpu->N = 0x00;
}


void rom_init(const char* filename, ROM* output) {
	FILE* file;
	errno_t error;

	if ((error = fopen_s(&file, filename, "rb")) != 0) {
		printf("Error: Unable to open ROM file: %s.\n", strerror(error));
		return ;
	}

	fseek(file, 0, SEEK_END);
	output->size = ftell(file);
	rewind(file);

	output->data = (uint8*)malloc(output->size * sizeof(uint8));

	if (!output->data) {
		printf("Error: Failed to allocate memory for 'output->data'.\n");
		fclose(file);
		return ;
	}

	fread_s(output->data, output->size, sizeof(unsigned char), output->size, file);
	fclose(file);
}


void rom_read_metadata(ROM* rom) {
	if (rom->size < 0x0143)
		return ;

	char* buffer = malloc(16 + 1);

	int j = 0;
	uint8* start = &rom->data[0x0134];
	for (start; start != &rom->data[0x0143]; start++) {
		unsigned char character = *start;

		if (character == '\0')
			break;

		buffer[j++] = character;
	}
	buffer[j] = '\0';

	rom->name = buffer;
}


void rom_load_into_gameboy_memory(ROM* rom, Gameboy* gb) {
	errno_t error;

	if (!rom->data) {
		printf("Error: Cannot load invalid read-only memory into the GameBoy.\n");
		return ;
	}

	size_t copy_size = (rom->size < sizeof gb->memory - 1) ? rom->size : sizeof gb->memory - 1;

	if ((error = memcpy_s(gb->memory, sizeof gb->memory, rom->data, copy_size) != 0))
		printf("Error: Failed to load ROM into memory: %s.\n", strerror(error));

    gb->memory[rom->size] = '\0';
}


int main(void) {
	CPU cpu;
	ROM rom;
	Gameboy gb;

	cpu_init(&cpu);
	rom_init("res/DMG_ROM.bin", &rom);

	rom_read_metadata(&rom);
	rom_load_into_gameboy_memory(&rom, &gb);

	printf("Starting the GameBoy emulator...\nRunning '%s' now (ROM is %i bytes large)\n====== LOG ======\n", rom.name, rom.size);

	/* cpu.PC = 0x0150; */

	while (cpu.PC < rom.size) {
		uint8 opcode = gb.memory[cpu.PC++];

		switch (opcode) {
			case 0x00: /* NOP */
				break;
			case 0x01: /* LD BC, nn */
				cpu.C = gb.memory[cpu.PC++];
				cpu.B = gb.memory[cpu.PC++];
				break;
			case 0x02: /* LD (BC), A */
				gb.memory[(cpu.B << 8) + cpu.C] = cpu.A;
				break;
			case 0x03: /* INC BC */
				cpu.C++;
				break;
			case 0x04: /* INC B */
				cpu.B++;
				cpu.Z = (cpu.B == 0);
				cpu.N = 0;
				cpu.H = (cpu.B & 0x0F);
				break;
			case 0x05: /* DEC B */
				cpu.B--;
				cpu.Z = (cpu.B == 0);
				cpu.N = 1;
				cpu.H = (cpu.B & 0x0F);
				break;
			/* ... other opcodes will be added here. */
			default:
				printf("Error: Invalid opcode: '0x%0X' at %i\n", opcode, cpu.PC);
				return 1;
		}
	}


	return 0;
}