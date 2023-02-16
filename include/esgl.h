#include <emulator.h>
typedef void* ESGL_Handle;


/* Creates a window with the name, width, height and RGB color as background. */
ESGL_Handle ESGL_window_create(const char* name, u16 width, u16 height, u8 red, u8 green, u8 blue);
/* Checks for any new events. */
u8 ESGL_window_check_events(ESGL_Handle* window);
/* Clears the visible conents of the window. */
void ESGL_window_clear(ESGL_Handle* window);
/* Closes the window cleanly.*/
void ESGL_window_close(ESGL_Handle* window);

/* Centers the window. */
void ESGL_window_center(ESGL_Handle* window);