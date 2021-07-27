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
private:
	int x;
	int y;
	float mass;
	float heat; // kelvin
	//float electrons;
	CompoundType compound_type;

public:
	GridNode();

public:
	void generate(int x, int y, 
		float mass, float heat, CompoundType compound_type);
	
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
	void calc_new_state(GridNode* a, float delta);
};

#endif