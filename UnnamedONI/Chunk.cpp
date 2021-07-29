#include <assert.h>
#include <exception>
#include "Chunk.h"

Chunk::Chunk(int x, int y) 
	: x(x), y(y) //, grid_nodes(SIZE*SIZE) 
{
	this->createNodes();
}

GridNode *Chunk::getNode(unsigned x, unsigned y) {

	// coordinate to pitch trick
	return &grid_nodes[y * SIZE + x];
}

GridNode* Chunk::getNodeSafe(unsigned x, unsigned y) {

	if (x >= Chunk::SIZE || y >= Chunk::SIZE)
	{
		return nullptr;
	}

	// coordinate to pitch trick
	return &grid_nodes[y * SIZE + x];
}


GridNode* Chunk::getNodeWrapped(int x, int y) {
	x %= Chunk::SIZE;
	if (x < 0) x += Chunk::SIZE;

	y %= Chunk::SIZE;
	if (y < 0) y += Chunk::SIZE;

	// coordinate to pitch trick
	return &grid_nodes[y * SIZE + x];
}

void Chunk::set(CompoundType type, float mass, float heat) {
	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {

			grid_nodes[y * SIZE + x].set(type, mass, heat);
		}
	}
}

void Chunk::set(CompoundType compound_type, float mass, float heat, 
	int x, int y, int w, int h) 
{
	// Lower boundaries
	if (x + w < 0 || y + h < 0)
		return;

	// Dimension trimming
	if (x < 0) {
		w += x;
		x = 0;
	}
	else if (x > SIZE)
		return;

	if (y < 0) {
		h += y;
		y = 0;
	}
	else if (y > SIZE)
		return;

	if (w > SIZE)
		w = SIZE;

	if (h > SIZE)
		h = SIZE;

	for (int yy = y; yy < y + h; yy++) {
		for (int xx = x; xx < x + w; xx++) {
			grid_nodes[yy * SIZE + xx].set(compound_type, mass, heat);
		}
	}
}

void Chunk::doPatternTransfer(int wrap_count) {

	for (int y = wrap_count; y < Chunk::SIZE + wrap_count; y += 4) {
		for (int x = wrap_count; x < Chunk::SIZE + wrap_count; x += 4) {
			/* * * * * *
			* Top left
			** * * * * */
			auto a = getNodeWrapped(x, y);
			auto b = getNodeWrapped(x + 1, y);

			a->calcNewState(b, 1.f);

			/* * * * * *
			* Bottom right
			** * * * * */
			a = getNodeWrapped(x + 2, y + 3);
			b = getNodeWrapped(x + 3, y + 3);

			a->calcNewState(b, 1.f);

			/* * * * * *
			* Bottom left
			** * * * * */
			a = getNodeWrapped(x, y + 2);
			b = getNodeWrapped(x, y + 3);

			a->calcNewState(b, 1.f);

			/* * * * * *
			* Top right
			** * * * * */
			a = getNodeWrapped(x + 3, y);
			b = getNodeWrapped(x + 3, y + 1);

			a->calcNewState(b, 1.f);

		}
	}

}

void Chunk::on_render() {
	for (auto&& node : grid_nodes) {
		node.on_render();
	}
}

void Chunk::createNodes() {
	grid_nodes.reserve(SIZE * SIZE);

	for (int y = 0; y < SIZE; y++) {
		for (int x = 0; x < SIZE; x++) {
			//grid_nodes[y * SIZE + x] = GridNode(x, y);
			grid_nodes.push_back(GridNode(x, y));
		}
	}
}