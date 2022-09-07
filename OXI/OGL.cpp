#include "OGL.h"

//#define LODEPNG_NO_COMPILE_ENCODER

#include "OXI.h"

#include "lodepng.h"
#include <unordered_set>
#include <stdexcept>

#ifdef _TINSPIRE
	#include <libndls.h>
#else
	#include <SDL.h>
	static SDL_Window* window;
	static SDL_Surface* surface;
	static SDL_Surface* surface565; // drawn to surface
#endif

static std::unique_ptr<color_t> screen;
static std::unique_ptr<Texture> texDither;

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

void OGL_init() {
#ifdef _TINSPIRE
	lcd_init(SCR_320x240_565);
#else
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
		throw std::runtime_error("Unable to init SDL");

	window = SDL_CreateWindow("OGL", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 320, 240, SDL_WINDOW_SHOWN);

	if (!window)
		throw std::runtime_error("Unable to create SDL window");
#endif

	screen = std::unique_ptr<color_t>(new color_t[SCREEN_WIDTH * SCREEN_HEIGHT]);

	texDither = OGL_loadTexture("dither.png");

#ifndef _TINSPIRE
	surface = SDL_GetWindowSurface(window);

	surface565 = SDL_CreateRGBSurfaceWithFormatFrom(screen.get(), SCREEN_WIDTH, SCREEN_HEIGHT, 16, 2 * SCREEN_WIDTH, SDL_PIXELFORMAT_RGB565);
#endif
}

void OGL_uninit() {
#ifdef _TINSPIRE
	lcd_init(SCR_TYPE_INVALID);
#else
	SDL_Quit();
#endif
}

void OGL_render() {
#ifdef _TINSPIRE
	lcd_blit(screen.get(), SCR_320x240_565);
#else
	//surface->pixels
	SDL_BlitSurface(surface565, NULL, surface, NULL);
	SDL_UpdateWindowSurface(window);
#endif
}


// transparency/alpha is not supported
std::unique_ptr<Texture> OGL_loadTexture(const char* name) {
	std::vector<byte_t> buffer;

	OXI_loadFile(name, buffer);

	std::vector<unsigned char> pixels;
	unsigned int w, h;
	lodepng::State state;
	if (lodepng::decode(pixels, w, h, state, buffer))
		throw std::runtime_error("Failed to decode texture");

	if (w > UCHAR_MAX || h > UCHAR_MAX)
		throw std::runtime_error("Texture dimesions are too large");



	auto tex = std::unique_ptr<Texture>(new Texture{
		static_cast<byte_t>(w),
		static_cast<byte_t>(h),
		std::unique_ptr<color_t>(new color_t[static_cast<uint64_t>(w * h)]),
		false,
		0 });

	// transparency detection
	{
		std::unordered_set<color_t> colors;
		for (int i = 0; i < pixels.size() / 4; i++) {
			
			// if transparent
			if (pixels[i * 4 + 3] == 0) {
				tex->m_useTransparency = true;
			}
			else {
				uint64_t rgb888 = (pixels[i * 4] << 16) | (pixels[i * 4 + 1] << 8) | (pixels[i * 4 + 2]);
				auto rgb565 = COLOR_888_565(rgb888);

				colors.insert(rgb565);
			}
		}

		// iterate unused for transparent candidate
		if (tex->m_useTransparency) {
			for (color_t c = 0; c < std::numeric_limits<color_t>::max(); c++) {
				if (colors.find(c) == colors.end()) {
					tex->m_transparent = c;
					break;
				}
			}
		}
	}



	for (int i = 0; i < pixels.size() / 4; i++) {
		// adds used colors for later transparent decision
		if (pixels[i * 4 + 3] == 0) {
			tex->m_pixels.get()[i] = tex->m_transparent;
		}
		else {
			uint64_t rgb888 = (pixels[i * 4] << 16) | (pixels[i * 4 + 1] << 8) | (pixels[i * 4 + 2]);
			auto rgb565 = COLOR_888_565(rgb888);

			tex->m_pixels.get()[i] = rgb565;
		}
	}

	return tex;

	//return std::unique_ptr<Texture>(
//		new Texture{static_cast<byte_t>(w), static_cast<byte_t>(h), std::move(px), false, 0});
	
}



// unoptimized currently
void OGL_drawRect(int x, int y, int w, int h, color_t c) {
	for (int j = y; j < y + h; j++) {
		for (int i = x; i < x + w; i++) {
			pixel(i, j, c);
		}
	}
}

void OGL_drawTexture(int x, int y, Texture* texture) {
	//OGL_drawTexture(x, y, 0, 0, texture->m_width, texture->m_height, texture);
	throw std::runtime_error("not implemented");
}

void OGL_drawTexture(int x, int y, int tx, int ty, int tw, int th, Texture* texture) {
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


void OGL_drawTexture(Rect* screen_rect, Rect* texture_rect, Texture* texture) {
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




void OGL_dither(Dither d, int chunk_x, int chunk_y) {
	// draw it
	Rect scr_rect{ chunk_x * 4, chunk_y * 4, 4, 4 };
	Rect tex_rect{ ((int) d) * 4, 0, 4, 4 };
	//OGL_drawTexture(&scr_rect, &tex_rect, texDither.get());
	OGL_drawTexture(scr_rect.x, scr_rect.y, tex_rect.x, tex_rect.y, tex_rect.w, tex_rect.h, texDither.get());
}

void OGL_ditherRect(Dither d, int chunk_x, int chunk_y, int chunk_w, int chunk_h) {
	for (int y = chunk_y; y < chunk_y + chunk_h; y++) {
		for (int x = chunk_x; x < chunk_x + chunk_w; x++) {
			OGL_dither(d, x, y);
		}
	}
}

void OGL_ditherGradRadial(int chunk_x, int chunk_y, int r) {
	throw std::runtime_error("not implemented");
	for (int y = -r; y < r; y++) {
		for (int x = -r; x < r; x++) {
			//int dist = x * x + y * y;
			int dist = std::sqrt(x * x + y * y);
			if (dist < (int)Dither::COUNT) {
				Dither d = Dither(dist);
				if (x * x + y * y <= r * r) {
					//OGL_effectDither(d, x + chunk_x, y + chunk_y);
				}
			}
		}
	}
}


void OGL_drawTextureFast(int x, int y, int tx, int ty, int tw, int th, Texture* texture) {

	throw std::runtime_error("Do not use");

	if (x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT)
		return;

	//if (tx)

	// clip image within screen
	//int draw_x = std::max(0, std::min(SCREEN_WIDTH - 1, x));
	//int draw_y = std::max(0, std::min(SCREEN_HEIGHT - 1, y));

	const int draw_x = std::max(0, x);
	const int draw_y = std::max(0, y);

	// cap lower bounds
	//x = std::max(0, x);
	//y = std::max(0, y);

	int draw_w = tw;
	int draw_h = th;
	if (x < 0)
		draw_w = tw + x;
	else if (x + tw >= SCREEN_WIDTH)
		draw_w = SCREEN_WIDTH - x;

	if (y < 0)
		draw_h = th + y;
	else if (y + th >= SCREEN_HEIGHT)
		draw_h = SCREEN_HEIGHT - y;

	for (; y < SCREEN_HEIGHT; y++) {
		int pitch = draw_x + draw_y * draw_w;
		//std::copy(texture->m_pixels.get() + pitch, 
		//	texture->m_pixels.get() + pitch + draw_w, 
		//	screen + tx + ty*(int)texture->m_width);
	}

}
