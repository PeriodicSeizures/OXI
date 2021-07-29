#include <chrono>
#include <thread>
#include <iostream>

#include "engine/Engine.h"
#include "world.h"

#define WIDTH 800
#define HEIGHT 600

int main(int argc, char **args) {
	Engine::init(WIDTH, HEIGHT);

	//GridNode node1(0, 0);
	//node1.set(CompoundType::GASEOUS_HYDROGEN, 1, 150);
	//
	//GridNode node2(1, 0);
	//node2.set(CompoundType::GASEOUS_HYDROGEN, 1, 400);

	//// temperature transfer test
	////printf("%.03f\t%.03f\n", this->heat, a->heat);
	//for (int i = 0; i < 300; i++) {
	//	std::cout << i << "s, " << node1.heat << ", " << node2.heat << "\n";
	//	node1.calcNewState(&node2, 1.f);
	//	//std::this_thread::sleep_for(std::chrono::seconds(1));
	//}
	//
	////node1.calc_new_state(&node2, 1.f);
	//node2.calcNewState(&node1, 1.f);

	//node1.on_render();

	World world;

	bool run = true;
	while (run) {
		world.on_update();
		//Engine::fill(Engine::DARK_PURPLE);
		
		//Engine::drawString("Hello, World!", 5, 5, Engine::BLACK, 3, false);

		world.on_render();

		//node1.calcNewState(&node2, 1.f);
		//
		//std::cout << node1.heat << ", " << node2.heat << "\n";
		//
		//node1.on_render();
		//node2.on_render();

		Engine::doRender();
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}

	Engine::uninit();

	return 0;
}