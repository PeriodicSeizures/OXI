#include "engine/Engine.h"
#include "world.h"

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char **args) {
	Engine::init(WIDTH, HEIGHT);

	bool run = true;

	while (run) {
		Engine::fill(Engine::DARK_PURPLE);
		
		Engine::drawString("Hello, World!", 5, 5, Engine::BLACK, 3, false);

		Engine::doRender();
	}

	Engine::uninit();

	return 0;
}