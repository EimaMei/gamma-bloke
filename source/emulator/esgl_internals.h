#include <windows.h>
#include <shellscalingapi.h>
#include <GL/gl.h>

typedef struct ESGL_Window {
	const char* name;
	u8 r, g, b;
	u16 width, height;

	HWND hwnd;
	HDC hdc;
    HGLRC glrc;
} ESGL_Window;


MSG curEvent;
u8 required_events[3] = {WM_SIZE, WM_MOVE, WM_CLOSE};

LRESULT CALLBACK WindowProc(HWND h, UINT msg, WPARAM param, LPARAM lparam);
