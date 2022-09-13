#pragma once

#include <stdint.h>
#include <memory>
#include <bitset>
#include <vector>
#include "Fix.h"
#include "Util.h"

// 11111000 00011111
//#define COLOR_565_888(rgb) ((((rgb >> 11) + 1) * 8) - 1) \
//	| (((((rgb >> 5) & 0b111111) + 1) * 8) - 1) \
//	| (((((rgb) & 0b11111) + 1) * 8) - 1)

// 111111110000000011111111


#ifndef _TINSPIRE
	#define SCREEN_WIDTH 320
	#define SCREEN_HEIGHT 240
#endif


// Primary functions
void DD_glInit();
void DD_glUnInit();
void DD_glRender();

// Utility functions


// Basic draw functions
void DD_glDrawRect(int x, int y, int w, int h, color_t c);

// Texture draw functions
void DD_glDrawTexture(int x, int y, Texture* tex);
//void OGL_drawTexture(int x, int y, int tx, int ty, int tw, int th, Texture* tex);
void DD_glDrawTexture(Rect* scr_rect, Rect* tex_rect, Texture* tex);

//void OGL_drawTextureFast(Rect* screen_rect, Rect* texture_rect, Texture* texture);

//void OGL_drawTextureFast(int x, int y, int tx, int ty, int tw, int th, Texture* texture);


