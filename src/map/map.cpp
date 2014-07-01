#include "map.h"
#include "block.h"

#include <algorithm>

Map::Map(const BlockList& list,int w, int h) : list(list), grid(w,h){
}

bool Map::solid(int x, int y, int layer) const{
	if (!grid.inArea(x,y,layer))
		return true;
	int id = grid.at(x,y,layer);
	if (id < 0)
		return false;
	return list[id].solid;
}

const Block* Map::getBlock(int x, int y, int layer) const{
	if (!grid.inArea(x,y,layer))
		return nullptr;

	int id = grid.at(x,y,layer);
	if (id < 0)
		return nullptr;
	return &(list[id]);
}

int& Map::idAt(int x, int y, int layer){
	return grid.at(x,y,layer);
}

bool Map::placeBlock(int x, int y, int layer, int block){
	if (!grid.inArea(x,y,layer))
		return false;

	if(grid.at(x,y,layer) < 0){
		setBlock(x, y, layer, block);
		return true;
	}
	return false;
}

bool Map::setBlock(int x, int y, int layer, int blockId){
	if (!grid.inArea(x,y,layer))
		return false;

	grid.at(x,y,layer) = blockId;
	return true;
}
