#ifndef CHUNK_H
#define CHUNK_H

#include <memory>
#include <vector>
#include "node/GridNode.h"

class Chunk {
private:
	int x, y;
	std::vector<GridNode> grid_nodes; // SIZE * SIZE
	
public:
	static constexpr int SIZE = 16;

public:
	Chunk(int x, int y);

public:
	GridNode &getNode(unsigned int x, unsigned int y);

	void fill(CompoundType compound_type, float mass, float heat);

	// update all
	//void on_tick();

};

#endif