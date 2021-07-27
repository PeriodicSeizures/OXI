#include <assert.h>
#include "Chunk.h"

Chunk::Chunk(int x, int y) 
	: x(x), y(y), grid_nodes(SIZE*SIZE) 
{}

GridNode &Chunk::getNode(unsigned int x, unsigned int y) {
	assert(x < SIZE && y < SIZE);

	// coordinate to pitch trick
	return grid_nodes[y * SIZE + x];
}

void Chunk::fill(CompoundType compound_type, float mass, float heat) {
	std::fill(grid_nodes.begin(), grid_nodes.end(), GridNode());
}

//void Chunk::on_tick() {
//	for (unsigned y = 0; y < SIZE; y++) {
//		for (unsigned x = 0; x < SIZE; x++) {
//
//			GridNode node = grid_nodes
//
//		}
//	}
//}