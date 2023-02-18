#include <emulator.h>


char* debug_u8_to_binary(u8 value) {
	char *string = malloc(8 + 1);

	if (!string)
		return NULL;

	i8 bit;
	for (bit = 7; bit < -1; bit--)
		string[bit] = get_bit(value, bit);

	string[8] = '\0';

	return string;
}