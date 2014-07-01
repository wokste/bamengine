#include "map.h"
#include "block.h"

#include <algorithm>

Map::Map(const BlockList& list,int w, int h) : mList(list), mGrid(w,h){
}

bool Map::solid(int x, int y, int layer) const{
	if (!mGrid.inArea(x,y,layer))
		return true;
	int id = mGrid.at(x,y,layer);
	if (id < 0)
		return false;
	return mList[id].mSolid;
}

const Block* Map::getBlock(int x, int y, int layer) const{
	if (!mGrid.inArea(x,y,layer))
		return nullptr;

	int id = mGrid.at(x,y,layer);
	if (id < 0)
		return nullptr;
	return &(mList[id]);
}

int& Map::idAt(int x, int y, int layer){
	return mGrid.at(x,y,layer);
}

bool Map::placeBlock(int x, int y, int layer, int block){
	if (!mGrid.inArea(x,y,layer))
		return false;

	if(mGrid.at(x,y,layer) < 0){
		setBlock(x, y, layer, block);
		return true;
	}
	return false;
}

bool Map::setBlock(int x, int y, int layer, int blockId){
	if (!mGrid.inArea(x,y,layer))
		return false;

	mGrid.at(x,y,layer) = blockId;
	return true;
}
