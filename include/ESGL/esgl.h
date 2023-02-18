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
#pragma once

#include "defines.hpp"

/* ====== Window related functions ====== */

/* Creates a window with the name, width, height and RGB color as background. */
ESGL_Handle ESGL_window_create(const char* name, int width, int height, u8 red, u8 green, u8 blue);
/* Checks for any new events. */
u8 ESGL_window_check_events(ESGL_Handle* window);
/* Clears the visible conents of the window. */
void ESGL_window_clear(ESGL_Handle* window);
/* Closes the window cleanly.*/
void ESGL_window_close(ESGL_Handle* window);

/* Centers the window. */
void ESGL_window_center(ESGL_Handle* window);

/* Key is pressed. */
int ESGL_window_key_is_pressed(ESGL_Handle* window, int key);


/* ====== Drawing related functions ====== */

/* Draws a basic rectangle. */
void ESGL_draw_rect(ESGL_Handle* window, int x, int y, int width, int height, u8 red, u8 green, u8 blue);