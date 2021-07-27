#ifndef BUILDING_H
#define BUILDING_H

//#include <string_view>
#include "../lib/gl.h"

/*
* Piping with input and outputs
* (source and destination)
* Power is transferred along an entire path
* (power conduits have a producer and consumer)
*/
enum class ConduitType : uint16_t
{
	None,
	Gas,
	Liquid,
	Solid
};

class BuildingDef {
private:
	std::string name;
	unsigned int elementReferredID; // material it is constructed from
	unsigned int id;

	const GLFix energyConsumptionWhenActive;
	const GLFix generatorWattageRating;
	const GLFix GeneratorBaseCapacity;
	// batteries?
	const GLFix exhaustKilowattsWhenActive;
	// heating amount when active
	const GLFix selfHeatKilowattsWhenActive;
	const GLFix baseMeltingPoint;
	const GLFix constructionTime;
	const GLFix workTime;
	const GLFix thermalConductivity;
	const int widthInCells;
	const int heightInCells;
	const int hitPoints;
	const bool requiresPowerInput;
	const bool requiresPowerOutput;
	// colors: wihte, green
	const bool useWhitePowerOutputConnectorColour;
	const ConduitType inputConduitType;
	const ConduitType outputConduitType;
	const bool floodable;
	const bool disinfectable;
	const bool entombable;
	const bool overheatable;
	const bool repairable;
	const GLFix overheatTemperature = 348.15f;
	const bool breakable;
	// any actual applicable use?
	//bool ContinuouslyCheckFoundation;
	// like for tiles?
	const bool isFoundation;
	const GLFix baseTimeUntilRepair = 600.f;
	const bool showInBuildMenu;
	const PermittedRotations permittedRotations;
	const CellOffset powerInputOffset;
	const CellOffset powerOutputOffset;
	const CellOffset utilityInputOffset; // x,y of input
	const CellOffset utilityOutputOffset; // x,y of output
	const bool isSolidTile;
	//KAnimFile[] AnimFiles;

	//Building(const char *name, const unsigned int elementReferredID);

};



//struct BuildingTile {
//	// buildings with solid colliders, prevent dupe movement across them
//	const bool allowGasFlow;
//	const bool allowLiquidFlow;
//
//};
//
//struct Generator {
//	const unsigned int power;
//	// not all buildings take the same type of input
//};

#endif