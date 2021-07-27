#include "Compound.h"

std::vector<Compound*> Compound::compounds(static_cast<unsigned>(CompoundType::size));

Compound::Compound(CompoundType compound_type,
	std::string name)
	: compound_type(compound_type),
	name(name),
	viscosity(0),
	thermal_conductivity(0),
	specific_heat(0),
	max_electrons(0),
	low_state_change(0),
	high_state_change(0),
	density(0),
	state_of_matter(StateOfMatter::Gas),
	color({ 0, 0, 0, 0 }) {}

Compound::Compound(CompoundType compound_type,
	std::string name,
	float viscosity,
	float thermal_conductivity,
	float specific_heat,
	float max_electrons,
	float low_state_change,
	float high_state_change,
	float density,
	StateOfMatter state,
	SDL_Color color)
	: compound_type(compound_type), 
	name(name),
	viscosity(viscosity),
	thermal_conductivity(thermal_conductivity),
	specific_heat(specific_heat),
	max_electrons(max_electrons),
	low_state_change(low_state_change),
	high_state_change(high_state_change),
	density(density),
	state_of_matter(state),
	color(color)
{
	compounds.insert(compounds.begin() + static_cast<unsigned>(compound_type), this);
}

Compound *Compound::get(CompoundType type) {
	//assert(static_cast<unsigned>(type) < CompoundType::size)
	//auto *t = &compounds[0];
	return compounds[static_cast<unsigned>(type)];
}

const Compound Compound::VOID(CompoundType::VOID, "Void");
const Compound Compound::LIQUID_HYDROGEN(CompoundType::LIQUID_HYDROGEN, "Liquid Hydrogen");
const Compound Compound::GASEOUS_HYDROGEN(CompoundType::GASEOUS_HYDROGEN, "Hydrogen Gas");
const Compound Compound::LIQUID_HELIUM(CompoundType::LIQUID_HELIUM, "Liquid Helium");
const Compound Compound::GASEOUS_HELIUM(CompoundType::GASEOUS_HELIUM, "Helium Gas");
const Compound Compound::SOLID_LITHIUM(CompoundType::SOLID_LITHIUM, "Lithium");

