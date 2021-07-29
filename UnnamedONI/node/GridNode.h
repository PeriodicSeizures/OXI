#ifndef TILE_H
#define TILE_H

// world holds tiles and entities

#include <stdint.h>
#include "../compound/Compound.h"
//#include "Layer.h"

/*
* A "tile" space in the world
* Will contain different layers of whatever (buildings, fluids, conduits...)
*/
class GridNode {
public:
	int x;
	int y;

	CompoundType compound_type;
	float mass;
	float heat; // kelvin
	//float electrons;

public:
	GridNode(int x, int y);

public:
	void set(CompoundType compound_type, float mass, float heat);
	
	/*
	* calc_new_state(...): Perform heat transfer across 
	* immediate neighboring nodes
	* 
	* an efficiency can be to identify primary nodes which cause the
	* entire change in heat
	* 
	* a new state change will need to be calculated only if 
	* any given temperatures of neighboring nodes are different
	*/
	void calcNewState(GridNode* a, float delta);

	void on_render();
};

#endif