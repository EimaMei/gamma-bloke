#include <emulator.h>
#include <ESGL/esgl.h>


void draw_gameboy_tile_i8(ESGL_Handle* window, unsigned char* buffer, u8 SCX, i8* x, i8* y, u8 scanline) {
    u8 i;
	i8 og_x = *x;
    i8 og_y = *x;

    for (i = 0; i < 16; i += 2) { /* Y row. */
        u8 msb = buffer[i + 1];
        u8 lsb = buffer[i];

        i8 bit;
        for (bit = 7; bit >= -1; bit--) { /* X row. */
			u8 value = 255;

            /*
                00 - 255, 255, 255
                01 - 170, 170, 170
                10 -  85, 85, 85
                11 -   0, 0, 0
            */
			if (get_bit(msb, bit) == 1)
				value -= 170;
			if (get_bit(lsb, bit) == 1)
				value -= 85;

			ESGL_draw_rect(window, *x, ((scanline + *y)) & 255, 1, 1, value, value, value);
            *x += 1;
        }
		*y += 1;
		*x = og_x;
    }
    *x += 8;
    *y = og_y;
}