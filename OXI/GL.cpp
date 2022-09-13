#include "GL.h"

//#define LODEPNG_NO_COMPILE_ENCODER

#include "Util.h"

#include "lodepng.h"
#include <unordered_set>
#include <stdexcept>

#ifdef _TINSPIRE
	#include <libndls.h>

	static bool grayscale;
	static std::unique_ptr<color_t> screenInverted; // for flipped the screen on grayscale models
#else
	#include <SDL.h>
	static SDL_Window* window;
	static SDL_Surface* surface;
	static SDL_Surface* surface565; // drawn to surface
#endif

static std::unique_ptr<color_t> screen;

inline void pixel(int x, int y, color_t c) {
	if (x < 0 || y < 0 || x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT) {
		return;
	}

	int pitch = x + y * SCREEN_WIDTH;

	screen.get()[pitch] = c;
}

inline void normalize(int& pos, int& size, int bound) {
	// restrict within screen bounds
	if (pos < 0) {
		size += pos;
		pos = 0;
	}

	// now whether width exceeds bounds
	if (pos + size >= bound) {
		size -= ((pos + size) - bound);
	}
}

void DD_glInit() {
#ifdef _TINSPIRE
	// handle other nspires too

	// BOOL is_cm: since v3.1 r863. TRUE on TI-Nspire CM/CM-C.
	// cm seems to be a more limited variant of nspire,
	// can be in color too
	grayscale = lcd_type() == SCR_320x240_4;
	if (grayscale) {
		lcd_init(SCR_320x240_16);
	} else
		lcd_init(SCR_320x240_565);
#else
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error("Unable to init SDL");

	window = SDL_CreateWindow("DD", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240, SDL_WINDOW_SHOWN);

	if (!window)
		throw std::runtime_error("Unable to create SDL window");
#endif

	screen = std::unique_ptr<color_t>(new color_t[SCREEN_WIDTH * SCREEN_HEIGHT]);

#ifndef _TINSPIRE
	surface = SDL_GetWindowSurface(window);

	surface565 = SDL_CreateRGBSurfaceWithFormatFrom(screen.get(), SCREEN_WIDTH, SCREEN_HEIGHT, 16, 2 * SCREEN_WIDTH, SDL_PIXELFORMAT_RGB565);
#endif
}

void DD_glUnInit() {
#ifdef _TINSPIRE
	lcd_init(SCR_TYPE_INVALID);
	screenInverted.reset();
#else
	SDL_FreeSurface(surface565);
	SDL_Quit(); // this should free all windows and surfaces
#endif
	screen.reset();
}

void DD_glRender() {
#ifdef _TINSPIRE
	if (grayscale) {
		COLOR* ptr = screen + SCREEN_HEIGHT * SCREEN_WIDTH, * ptr_inv = screen_inverted + SCREEN_HEIGHT * SCREEN_WIDTH;
		while (--ptr >= screen)
			*--ptr_inv = ~*ptr;

		lcd_blit(screenInverted.get(), SCR_320x240_16);
	} else
		lcd_blit(screen.get(), SCR_320x240_565);
#else
	//surface->pixels
	SDL_BlitSurface(surface565, NULL, surface, NULL);
	SDL_UpdateWindowSurface(window);
#endif
}





// unoptimized currently
void DD_glDrawRect(int x, int y, int w, int h, color_t c) {
	for (int j = y; j < y + h; j++) {
		for (int i = x; i < x + w; i++) {
			pixel(i, j, c);
		}
	}
}

void DD_glDrawTexture(int x, int y, Texture* texture) {
	//OGL_drawTexture(x, y, 0, 0, texture->m_width, texture->m_height, texture);
	throw std::runtime_error("not implemented");
}

void DD_glDrawTexture(int x, int y, int tx, int ty, int tw, int th, Texture* texture) {
	//throw std::runtime_error("not implemented");

	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
		return;

	if (tx < 0 || ty < 0 || tx >= texture->m_width || ty >= texture->m_height)
		return;

	for (int j = 0; j < th; j++) {
		for (int i = 0; i < tw; i++) {
			int ix = tx + i;
			int iy = ty + j;

			int dx = x + i;
			int dy = y + j;

			auto c = texture->m_pixels.get()[ix + iy * (int)texture->m_width];

			if (!(texture->m_useTransparency && texture->m_transparent == c))
				pixel(dx, dy, c);
		}
	}
}


void DD_glDrawTexture(Rect* screen_rect, Rect* texture_rect, Texture* texture) {
	// draw image with scaling
	// impose texture_rect upon screen_rect with stretching/scaling
	const FFix tex_x_inc = FFix(texture_rect->w) / FFix(screen_rect->w);
	const FFix tex_y_inc = FFix(texture_rect->h) / FFix(screen_rect->h);

	FFix tex_x = texture_rect->x;
	FFix tex_y = texture_rect->y;

	for (FFix screen_y = screen_rect->y;
		screen_y < FFix(screen_rect->y + screen_rect->h);
		screen_y+=1) {

		for (FFix screen_x = screen_rect->x;
			screen_x < FFix(screen_rect->x + screen_rect->w);
			screen_x+=1) {

			if (tex_x >= 0 && tex_y >= 0
				&& tex_x < FFix(texture->m_width) && tex_y < FFix(texture->m_height)) {
				auto c = texture->m_pixels.get()[(int)tex_x + (int)tex_y * texture->m_width];
				if (!(texture->m_useTransparency && texture->m_transparent == c))
					pixel(screen_x, screen_y, c);
			}

			tex_x += tex_x_inc;
		}

		tex_x = texture_rect->x;
		tex_y += tex_y_inc;
	}
}

void OGL_drawTextureFast(Rect* scr_rect, Rect* tex_rect, Texture* tex) {
	throw std::runtime_error("not implemented");

	// draw image with scaling
	// impose texture_rect upon screen_rect with stretching/scaling
	const FFix tex_x_rate = FFix(tex_rect->w) / FFix(scr_rect->w);
	const FFix tex_y_rate = FFix(tex_rect->h) / FFix(scr_rect->h);

	FFix scr_x = scr_rect->x, scr_y = scr_rect->y, scr_w = scr_rect->w, scr_h = scr_rect->h,
		tex_x = tex_rect->x, tex_y = tex_rect->y, tex_w = tex_rect->w, tex_h = tex_rect->h;

	// normalize(...) wont work here because texture also needs normalizing if screen coords changed
	//normalize(screen_rect->x, screen_rect->w, SCREEN_WIDTH);
	if (scr_x < FFix(0)) {
		//size += pos;
		scr_w += scr_x;
		
		//pos = 0;
		scr_x = 0;
		
		// normalize texture now
		//tex_w += ((FFix(scr_rect->w) - scr_w) / FFix(scr_rect->w));
		tex_w *= (FFix(1) - ((FFix(scr_rect->w) - scr_w) / FFix(scr_rect->w)));

		//tex_x += 
	}

	// now whether width exceeds bounds
	//if (pos + size >= bound) {
	//	size -= ((pos + size) - bound);
	//}

	if (true)
		return;

	//for (FFix screen_y = screen_rect->y;
	//	screen_y < FFix(screen_rect->y + screen_rect->h);
	//	screen_y += 1) {
	//
	//	for (FFix screen_x = screen_rect->x;
	//		screen_x < FFix(screen_rect->x + screen_rect->w);
	//		screen_x += 1) {
	//
	//		if (tex_x >= 0 && tex_y >= 0
	//			&& tex_x < FFix(texture->m_width) && tex_y < FFix(texture->m_height))
	//			pixel(screen_x, screen_y, texture->m_pixels.get()[(int)tex_x + (int)tex_y * texture->m_width]);
	//
	//		tex_x += tex_x_inc;
	//
	//	}
	//
	//	tex_x = texture_rect->x;
	//	tex_y += tex_y_inc;
	//}
}
