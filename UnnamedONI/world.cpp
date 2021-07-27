#include "world.h"

World::World() {

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
		return &find->second->getNode(rx, ry);
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
