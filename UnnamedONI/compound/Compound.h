#pragma once

#include <string>
#include <vector>
#include <SDL.h>
#include "CompoundType.h"

enum class StateOfMatter : uint16_t {
	Gas,
	Liquid,
	Solid
};

class Compound {
public:
	/*
	* enum-like
	*/
	static const Compound VOID;
	static const Compound LIQUID_HYDROGEN;
	static const Compound GASEOUS_HYDROGEN;
	static const Compound LIQUID_HELIUM;
	static const Compound GASEOUS_HELIUM;
	static const Compound SOLID_LITHIUM;

private:
	static std::vector<Compound*> compounds;

public:
	const CompoundType compound_type; // enum for material
	const std::string name;

	/*
	* Substance chem stuff
	*/
	const float viscosity;
	const float thermal_conductivity;
	const float specific_heat;
	const float max_electrons;
	const float low_state_change;
	const float high_state_change;
	const float density;
	/*
	* Density determines a fluids spaciousness
	*	- The higher the density, the higher
	*		the maximum mass per m^3 
	*	- Think of "compacting slime" 
	*		with densities
	*/

	const StateOfMatter state_of_matter;


	const SDL_Color color;

private:
	Compound(CompoundType compound_type, std::string name);

	Compound(CompoundType compound_type, 
		std::string name,
		float viscosity,
		float thermal_conductivity,
		float specific_heat,
		float max_electrons,
		float low_state_change,
		float high_state_change,
		float density,
		StateOfMatter state,
		SDL_Color color);

public:
	static Compound *get(CompoundType type);

};