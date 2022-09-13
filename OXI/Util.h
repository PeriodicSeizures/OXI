#pragma once

#include <vector>
#include "fix.h"
#include <memory>

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

using byte_t = uint8_t;
using FFix = Fix<8>;
using DFix = Fix<12>;
using color_t = uint16_t;
struct Rect {
	int x;
	int y;
	int w;
	int h;
};
struct Texture {
	uint8_t m_width;				    // 1 byte
	uint8_t m_height;				    // 1 byte
	std::unique_ptr<color_t> m_pixels;   // 8 bytes
	bool m_useTransparency;
	color_t m_transparent;              // transparent color
};

void DD_loadFile(const char* name, std::vector<byte_t>& out);
std::unique_ptr<Texture> DD_loadTexture(const char* name);