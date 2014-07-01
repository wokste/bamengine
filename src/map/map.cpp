#include "map.h"
#include "block.h"

#include <algorithm>

Map::Map(int w, int h) : grid(w,h){
}

bool Map::solid(int x, int y, int layer) const{
	if (!grid.inArea(x,y,layer))
		return true;
	int id = grid.at(x,y,layer);
	if (id < 0)
		return false;
	return blockTypes[id]->solid;
}

const Block* Map::getBlock(int x, int y, int layer) const{
	if (!grid.inArea(x,y,layer))
		return nullptr;

	int id = grid.at(x,y,layer);
	if (id < 0)
		return nullptr;
	return blockTypes[id];
}

bool Map::placeBlock(int x, int y, int layer, const Block* block){
	if (!grid.inArea(x,y,layer))
		return false;

	if(grid.at(x,y,layer) < 0){
		setBlock(x, y, layer, block);
		return true;
	}
	return false;
}

bool Map::setBlock(int x, int y, int layer, const Block* block){
	if (!grid.inArea(x,y,layer))
		return false;

	if (block == nullptr)
		return false;

	auto it = std::find(blockTypes.begin(), blockTypes.end(), block);
	int blockId = std::distance(blockTypes.begin(), it);

	if (it == blockTypes.end()){
		// block not in vector
		blockTypes.push_back(block);
	}

	grid.at(x,y,layer) = blockId;
	return true;
}
