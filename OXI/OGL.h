#pragma once

#include <stdint.h>
#include <memory>
#include <bitset>
#include <vector>
#include "Fix.h"

// 11111000 00011111
//#define COLOR_565_888(rgb) ((((rgb >> 11) + 1) * 8) - 1) \
//	| (((((rgb >> 5) & 0b111111) + 1) * 8) - 1) \
//	| (((((rgb) & 0b11111) + 1) * 8) - 1)

// 111111110000000011111111
#define COLOR_888_565(rgb) \
	  (COLOR_RGB888_TO_R5(rgb) << 11) \
	| (COLOR_RGB888_TO_G6(rgb) << 5) \
	| (COLOR_RGB888_TO_B5(rgb))

#define COLOR_RGB888_TO_R5(rgb)  \
	   ((rgb >> 16) / 8)

#define COLOR_RGB888_TO_G6(rgb) \
	((((rgb >> 8) & 0xFF) / 4) & 0b111111)

#define COLOR_RGB888_TO_B5(rgb) \
	((((rgb) & 0xFF) / 8) & 0b11111)

#ifndef _TINSPIRE
	#define SCREEN_WIDTH 320
	#define SCREEN_HEIGHT 240
#endif

using color_t = uint16_t;

struct Texture {
	uint8_t m_width;				    // 1 byte
	uint8_t m_height;				    // 1 byte
	std::unique_ptr<color_t> m_pixels;   // 8 bytes
	bool m_useTransparency;
	color_t m_transparent;              // transparent color

	~Texture() {
		printf("~Texture()\n");
	}
};

struct Rect {
	int x;
	int y;
	int w;
	int h;
};

// Primary functions
void OGL_init();
void OGL_uninit();
void OGL_render();

// Utility functions
std::unique_ptr<Texture> OGL_loadTexture(const char* name);

// Basic draw functions
void OGL_drawRect(int x, int y, int w, int h, color_t c);

// Texture draw functions
void OGL_drawTexture(int x, int y, Texture* tex);
//void OGL_drawTexture(int x, int y, int tx, int ty, int tw, int th, Texture* tex);
void OGL_drawTexture(Rect* scr_rect, Rect* tex_rect, Texture* tex);

//void OGL_drawTextureFast(Rect* screen_rect, Rect* texture_rect, Texture* texture);

//void OGL_drawTextureFast(int x, int y, int tx, int ty, int tw, int th, Texture* texture);



enum class Dither {
	FULL = 0,
	P9375,
	P8750,
	P7500,
	P6250,
	HALF,
	P3750,
	P2500,
	P1250,
	P0625,
	EMPTY,
	COUNT,
};

// Post processing
// dither will contain a baked texture which will be overlayed
void OGL_dither(Dither d, int chunk_x, int chunk_y);

void OGL_ditherRect(Dither d, int chunk_x, int chunk_y, int chunk_w, int chunk_h);

// another effect to create radial dither
void OGL_ditherGradRadial(int chunk_x, int chunk_y, int r);

//void GL_effectLighting()
