#include "Layer.h"

//Layer::Layer(LayerType layer, std::vector<LayerType> collides)
//	: layer(layer), collides(collides) {}

Layer::Layer(LAYER_MASK layer, LAYER_MASK collide_mask)
	: layer(layer), collide_mask(collide_mask) {}

//const Layer Layer::BUILDING(LayerType::BUILDING, { LayerType::NATURAL_TILE, LayerType::FLUID });
//const Layer Layer::MESH_TILE(LayerType::MESH_TILE, { LayerType::FLUID });
//const Layer Layer::SOLID_TILE(LayerType::SOLID_TILE, { });
//const Layer Layer::NATURAL_TILE(LayerType::NATURAL_TILE, { LayerType::BUILDING });
//const Layer Layer::FLUID(LayerType::FLUID, { LayerType::BUILDING, LayerType::MESH_TILE });

const Layer Layer::BUILDING(LayerType::BUILDING, LayerType::NATURAL_TILE | LayerType::FLUID);
const Layer Layer::MESH_TILE(LayerType::MESH_TILE, LayerType::FLUID);
const Layer Layer::SOLID_TILE(LayerType::SOLID_TILE, 0);
const Layer Layer::NATURAL_TILE(LayerType::NATURAL_TILE, LayerType::BUILDING);
const Layer Layer::FLUID(LayerType::FLUID, LayerType::BUILDING | LayerType::MESH_TILE);