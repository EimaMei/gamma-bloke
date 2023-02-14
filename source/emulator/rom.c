#include <emulator.h>

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

	fread_s((uint8*)output->data, output->size, sizeof(unsigned char), output->size, file);
	fclose(file);
}


void rom_read_metadata(ROM* rom) {
	if (rom->size < 0x0143)
		return ;

	char* buffer = malloc(16 + 1);

	int j = 0;
	const uint8* start = &rom->data[0x0134];
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