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
	/*
	* getNode(..): Return the node located at (x, y)
	* 
	*/
	GridNode* getNode(unsigned x, unsigned y);
	GridNode *getNodeSafe(unsigned x, unsigned y);
	GridNode* getNodeWrapped(int x, int y);
	
	/*
	* set(...): Set node values in a region
	*/
	void set(CompoundType type, float mass, float heat);
	void set(CompoundType compound_type, float mass, float heat, 
		int x, int y, int w, int h);

	void doPatternTransfer(int wrap_count);

	void on_render();

private:
	void createNodes();

};

#endif