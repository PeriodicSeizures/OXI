#include <thread>
#include <chrono>
#include "world.h"

World::World() : wrap_count(0) {
	auto chunk = createChunk(0, 0);
	chunk->set(CompoundType::GASEOUS_HYDROGEN, 1, 200, 0, 0, 8, 16);

	chunk->set(CompoundType::GASEOUS_HYDROGEN, 1, 430, 8, 0, 8, 16);
}

World::~World() {

}

GridNode *World::getNode(int x, int y) {
	auto&& find = chunks.find({ x / Chunk::SIZE, y / Chunk::SIZE });
	//find->second
	if (find != chunks.end()) {
		int rx = x % Chunk::SIZE;
		int ry = y % Chunk::SIZE;
		if (rx < 0) rx += Chunk::SIZE;
		if (ry < 0) ry += Chunk::SIZE;
		return find->second->getNode(rx, ry);
	}
	return nullptr;
}

std::shared_ptr<Chunk> World::getChunk(int cx, int cy) {
	auto&& find = chunks.find({ cx, cy });
	//find->second
	if (find != chunks.end()) {
		return find->second;
	}
	return nullptr;
}

std::shared_ptr<Chunk> World::createChunk(int cx, int cy) {
	std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>(cx, cy);
	chunks.insert({ std::tuple<int, int>(cx, cy),
		 chunk});
	return chunk;
}

//void World::calc_change_states_a(int x, int y) {
//	// bot-right cross pattern thermal transfer
//	// state change from [adj to center] && [adj to center]
//	//auto&& chunk = getChunk(x / Chunk::SIZE, y / Chunk::SIZE);
//	//chunks[y*Chunk::SIZE + x]
//	auto&& center = getNode(x, y);
//	auto&& a = getNode(x+1, y);
//	auto&& b = getNode(x, y-1);
//
//	// transfer [a and center], [b and center]
//
//}

void World::on_update() {
	// bot-right cross pattern thermal transfer
	// state change from [adj to center] && [adj to center]
	auto chunk = getChunk(0, 0);

	chunk->doPatternTransfer(wrap_count--);

	if (wrap_count <= -4)
		wrap_count = 0;

	std::this_thread::sleep_for(std::chrono::milliseconds(500));
}

void World::on_render() {
	auto chunk = getChunk(0, 0);

	chunk->on_render();
}