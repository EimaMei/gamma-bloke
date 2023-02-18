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


void ESGL_draw_rect(ESGL_Handle* window, int x, int y, int width, int height, u8 red, u8 green, u8 blue) {
    ESGL_Window* win = *window;

    float i = win->width * 0.5f;
    float new_x = (x / i) - 1.0f;
    float new_x2 = ((x + width) / i) - 1.0f;

    i = win->height * -0.5f;
    float new_y = (y / i) + 1.0f;
    float new_y2 = ((y + height) / i) + 1.0f;

    glColor4f(red / 255.0f, green / 255.0f, blue / 255.0f, 1.0f);

    glBegin(GL_QUADS);
        glVertex2f(new_x, new_y);
        glVertex2f(new_x2, new_y);
        glVertex2f(new_x2, new_y2);
        glVertex2f(new_x, new_y2);
    glEnd();
}