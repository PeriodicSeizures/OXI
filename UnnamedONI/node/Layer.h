#pragma once

#include <stdint.h>
#include <vector>

typedef unsigned LAYER_MASK;

struct LayerType {
	constexpr static LAYER_MASK BUILDING =		0b1;
	constexpr static LAYER_MASK MESH_TILE =		0b10;
	constexpr static LAYER_MASK SOLID_TILE =	0b100;
	constexpr static LAYER_MASK NATURAL_TILE =	0b1000;
	constexpr static LAYER_MASK FLUID =			0b10000;
};

class Layer
{
public:
	/*
	* enum-like
	*/
	static const Layer BUILDING;
	static const Layer MESH_TILE;
	static const Layer SOLID_TILE;
	static const Layer NATURAL_TILE;
	static const Layer FLUID;

private:
	LAYER_MASK layer;
	LAYER_MASK collide_mask;
	//std::vector<LayerType> collides;


private:
	///Layer(LayerType layer, std::vector<LayerType> collides);
	Layer(LAYER_MASK layer, LAYER_MASK collide_mask);
};

