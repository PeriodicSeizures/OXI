#ifndef WORLD_H
#define WORLD_H

#include <unordered_map>
#include <map>

#include "Chunk.h"

// world holds tiles and entities

class World {
private:
	std::map<std::tuple<int, int>, std::shared_ptr<Chunk>> chunks;

	int wrap_count;

public:
	World();
	~World();

public:
	GridNode *getNode(int x, int y);	// x,y : block coords
	std::shared_ptr<Chunk> getChunk(int cx, int cy);	// cx,cy : chunk coords
	
	std::shared_ptr<Chunk> createChunk(int cx, int cy);

	/*
	* bottom-right cross pattern, non-overlap state change algorithm 
	* (with shifting grid to account for prev unmodified nodes)
	*/
	//void calc_change_states_a(int x, int y);

	/*
	* Transfer states between these 2 nodes
	*/
	//void calc_change_states_b(GridNode *a, GridNode *b);

	void on_update();

	void on_render();

};

#endif