#include <iostream>
#include <assert.h>
#include "engine/Engine.h"
#include "GridNode.h"

GridNode::GridNode(int x, int y) : x(x), y(y) {}

void GridNode::set(CompoundType compound_type, float mass, float heat)
{
	this->compound_type = compound_type;
	this->mass = mass;
	this->heat = heat;
}

void GridNode::calcNewState(GridNode* a, float delta) {
	/*
	* thermal transfer is dependent upon:
	*	- the object with the least conductivity
	*	- the thermal capacities
	* transfering will equalize among the 2 objects over time
	* more conductivity will increase the delta transfer
	* thermal capacity acts as a buffer which soaks up heat
	*/

	static constexpr float K_NAT = 1.f;

	Compound *compound_this = Compound::get(this->compound_type);
	Compound *compound_a = Compound::get(this->compound_type);

	//diff = node2.heat - node1.heat
	float diff = a->heat - this->heat;

	//conduct = min(node1.compound.conduct, node2.compound.conduct)
	float conduct = std::min(
		compound_a->thermal_conductivity,
		compound_this->thermal_conductivity);

	//change = conduct * diff * .5
	float change = conduct * diff * delta * .5f * K_NAT;

	//node1.heat += change / node1.compound.specific_heat
	//node2.heat -= change / node2.compound.specific_heat
	this->heat += change / (compound_this->specific_heat * this->mass);
	a->heat -= change / (compound_a->specific_heat * a->mass);

	//print(f'after node1: {node1.heat}, node2: {node2.heat}')
	//printf("%.03f\t%.03f\n", this->heat, a->heat);
	//std::cout << this->heat << ", " << a->heat << "\n";
}

void GridNode::on_render() {
	// draw this tile
	SDL_Color c;
	if (heat < 273.f) // 0 C
		c = Engine::DARK_AQUA;
	else if (heat < 291.f) // 18 C
		c = Engine::AQUA;
	else if (heat < 304.f) // 31 C
		c = Engine::GREEN;
	else if (heat < 312.f) // 39 C
		c = Engine::YELLOW;
	else //if (heat < 345.f) // 72 C
		c = Engine::RED;


	Engine::fillRect(c, x * 16, y * 16, 16, 16);
}
