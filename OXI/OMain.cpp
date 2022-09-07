#ifdef _TINSPIRE
#include <libndls.h>
#endif

#include "OGL.h"
#include <thread>

void run() {
	OGL_init();

	//OGL_drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0b1111100000011111);

	auto c = COLOR_888_565(0xFF0000);
	auto r565 = COLOR_RGB888_TO_R5(0xFF0000);
	auto g565 = COLOR_RGB888_TO_G6(0xFF0000);

	//TextureMask mask(std::unique_ptr<byte_t>(new byte_t[1]{0}), 8, 1);
	//auto v = mask[0];
	//mask[0] = 1;

	auto tex = OGL_loadTexture("rocket.png");

	Rect screen_rect{ 0, 0, tex->m_width * 3, tex->m_height * 3 };
	Rect texture_rect{ 0, 0, tex->m_width, tex->m_height };

	//OGL_drawTexture(&screen_rect, &texture_rect, tex.get());

	//OGL_ditherRect(Dither::HALF, 0, 0, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);

	//OGL_dither(Dither::P1250, 1, 1);

	//OGL_drawTexture(0, 0, tex.get());
	//OGL_drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0b0000000000011111);

	bool dir = false;
	Dither d = Dither::EMPTY;
#ifdef _TINSPIRE
	while (!isKeyPressed(KEY_NSPIRE_ESC)) {
#else
	for (;;) {
#endif
		OGL_drawTexture(&screen_rect, &texture_rect, tex.get());

		OGL_ditherRect(d, 0, 0, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4);

		if (dir) {
			d = Dither(((int)d) + 1);
			if (d == Dither::EMPTY)
				dir = false;
		}
		else {
			d = Dither(((int)d) - 1);
			if (d == Dither::FULL)
				dir = true;
		}

		// delay
#ifdef _TINSPIRE
		msleep(100);
#else
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
#endif
		OGL_render();
	}

	OGL_uninit();
}

int main(void)
{
	// exceptions end up not getting caught gracefully when not using try/catch,
	// which wont allow allocated memory to be freed (like in smart ptrs)
	try {
		run();
	}
	catch (const std::exception& e) {
		printf("Error: %s\n", e.what());
#ifdef _TINSPIRE
		show_msgbox("OXI Error", e.what());
#endif
	}

	return 0;
}