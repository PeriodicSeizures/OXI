#include <iostream>
#include "GridNode.h"

GridNode::GridNode() {}

void GridNode::generate(int x, int y, 
	float mass, float heat, CompoundType compound_type) 
{
	this->x = x;
	this->y = y;
	this->mass = mass;
	this->heat = heat;
	this->compound_type = compound_type;
}

void GridNode::calc_new_state(GridNode* a, float delta) {
	/*
	* thermal transfer is dependent upon:
	*	- the object with the least conductivity
	*	- the thermal capacities
	* transfering will equalize among the 2 objects over time
	* more conductivity will increase the delta transfer
	* thermal capacity acts as a buffer which soaks up heat
	*/

	Compound *compound_this = Compound::get(this->compound_type);
	Compound *compound_a = Compound::get(this->compound_type);

	//diff = node2.heat - node1.heat
	float diff = a->heat - this->heat;

	//conduct = min(node1.compound.conduct, node2.compound.conduct)
	float conduct = std::min(
		compound_a->thermal_conductivity,
		compound_this->thermal_conductivity);

	//change = conduct * diff * .5
	float change = conduct * diff * delta * .5f;

	//print(f'change: {change}')
	//print(f'before node1: {node1.heat}, node2: {node2.heat}')
	std::cout << "before this->heat: " << this->heat << ", a->heat: " << a->heat << "\n";

	//node1.heat += change / node1.compound.specific_heat
	//node2.heat -= change / node2.compound.specific_heat
	this->heat += change / compound_this->specific_heat;
	a->heat -= change / compound_a->specific_heat;

	//print(f'after node1: {node1.heat}, node2: {node2.heat}')
	std::cout << "after this->heat: " << this->heat << ", a->heat: " << a->heat << "\n";

	// thermal capacity is assumed to be 1 for simplicity
}