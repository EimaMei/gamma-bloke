/*
* Copyright (C) 2023 EimaMei/Sacode
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
*
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
*
* 1. The origin of this software must not be misrepresented; you must not
*    claim that you wrote the original software. If you use this software
*    in a product, an acknowledgment in the product documentation would be
*    appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
*    misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*
*
*/
#include <ESGL/esgl.h>
#include "esgl_internals.h"

#include <windows.h>
#include <shellscalingapi.h>


MSG curEvent;
u8 required_events[3] = {WM_SIZE, WM_MOVE, WM_CLOSE};


ESGL_Handle ESGL_window_create(const char* name, int width, int height, u8 red, u8 green, u8 blue) {
	SetProcessDpiAwareness(PROCESS_PER_MONITOR_DPI_AWARE); /* dpi scaling baed :trolleybus: */
	HINSTANCE inh = GetModuleHandle(NULL);

	WNDCLASSA Class = {0}; /* Setup the Window class. */
	Class.lpszClassName = "Edgy Simple GUI Library";
	Class.hInstance = inh;
	Class.hCursor = LoadCursor(NULL, IDC_ARROW);
	Class.lpfnWndProc = WindowProc;
	RegisterClassA(&Class);

	DWORD window_style = WS_CAPTION | WS_SYSMENU | WS_BORDER | WS_SIZEBOX | WS_MAXIMIZEBOX | WS_MINIMIZEBOX;

	RECT adjusted_size = {0, 0, width, height};
	AdjustWindowRect(&adjusted_size, window_style, 0);

	HWND hwnd = CreateWindowA("Edgy's Simple GUI Library", name, window_style, 0, 0, width, height, NULL, NULL, inh, NULL);
	HDC hdc = GetDC(hwnd);

	PIXELFORMATDESCRIPTOR pfd = {0}; /* Setup OpenGL. */
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

    int format = ChoosePixelFormat(hdc, &pfd); /* More OpenGL shenanigans. */
	SetPixelFormat(hdc, format, &pfd);
	HGLRC glrc = wglCreateContext(hdc);
	wglMakeCurrent(hdc, glrc);

	ESGL_Window* win = malloc(sizeof(ESGL_Window)); /* Setup the ESGL_Window var return. */
	win->name = name;

	win->r = red;
	win->g = green;
	win->b = blue;

	win->width = width;
	win->height = height;

	win->hwnd = hwnd;
	win->hdc = hdc;
	win->glrc = glrc;

	ESGL_window_center((ESGL_Handle*)&win); /* Center the window to make it look pretty. */
	ShowWindow(hwnd, SW_SHOWNORMAL);
	UpdateWindow(hwnd);

	return win;
}


void ESGL_window_center(ESGL_Handle* window) {
	ESGL_Window* win = *window;

	int new_x = (GetSystemMetrics(SM_CXSCREEN) - win->width) * 0.5f,
		new_y = (GetSystemMetrics(SM_CYSCREEN) - win->height) * 0.5f;

	MoveWindow(win->hwnd, new_x, new_y, win->width, win->height, 1);
}


u8 ESGL_window_check_events(ESGL_Handle* window) {
	ESGL_Window* win = *window;
	MSG msg = {};
	u8 event_type = 0;

	while (PeekMessage(&msg, win->hwnd, 0u, 0u, PM_REMOVE)) {
		switch (msg.message) {
			case WM_CLOSE:
			case WM_QUIT: event_type = 33; break;
			default: break;
		}
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (curEvent.hwnd == win->hwnd) {
		switch (curEvent.message) {
			case WM_CLOSE: {
				event_type = 33;
				PostQuitMessage(0);

				break;
			}
			case WM_SIZE: {
				/*r.w = LOWORD(curEvent.lParam);
				r.h = HIWORD(curEvent.lParam);*/

				glViewport(0, 0, LOWORD(curEvent.lParam), HIWORD(curEvent.lParam));
				break;
			}
			default: break;
		}
	}
	curEvent.hwnd = NULL;

	return event_type;
}


void ESGL_window_clear(ESGL_Handle* window) {
	ESGL_Window* win = *window;

	glFlush();
	/*SwapBuffers(win->hdc);
	wglMakeCurrent(NULL,NULL);
	wglMakeCurrent(win->hdc, win->glrc);*/

	glClearColor(win->r / 255.0f, win->g / 255.0f, win->b / 255.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}


void ESGL_window_close(ESGL_Handle* window) {
	ESGL_Window* win = *window;

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(win->glrc);
	DestroyWindow(win->hwnd);
	DeleteDC(win->hdc);
}


int ESGL_window_key_is_pressed(ESGL_Handle* window, int key) {
	return (( 1 << 15 ) & GetAsyncKeyState(key)); /* Note that as of now, this checks globally for any key presses. Will need to be fixed later on. */
}


LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	u8 i;
    for (i = 0; i < sizeof(required_events) / sizeof(*required_events); i++) {
		if (msg == required_events[i]) {
			curEvent.hwnd = hwnd;
            curEvent.message = msg;
            curEvent.lParam = lparam;
			curEvent.wParam = wparam;
			break;
		}
	}

	return DefWindowProc(hwnd, msg, wparam, lparam);
}