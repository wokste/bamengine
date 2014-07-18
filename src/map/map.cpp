#include "map.h"
#include "block.h"

#include <algorithm>

Map::Map(const BlockList& list,int w, int h) : mList(list), mGrid(w,h){
}

bool Map::blockPlayer(int x, int y, int layer) const{
	if (!mGrid.inArea(x,y,layer))
		return true;
	int id = mGrid.at(x,y,layer);
	if (id == Block::air)
		return false;
	return mList[id].mFlags.mBlockPlayer;
}

const Block* Map::getBlock(int x, int y, int layer) const{
	if (!mGrid.inArea(x,y,layer))
		return nullptr;

	int id = mGrid.at(x,y,layer);
	if (id == Block::air)
		return nullptr;
	return &(mList[id]);
}

BlockId& Map::idAt(int x, int y, int layer){
	return mGrid.at(x,y,layer);
}

const BlockId& Map::idAt(int x, int y, int layer) const{
	return mGrid.at(x,y,layer);
}

bool Map::placeBlock(int x, int y, int layer, BlockId blockId){
	if (!mGrid.inArea(x,y,layer))
		return false;

	if(mGrid.at(x,y,layer) == Block::air){
		setBlock(x, y, layer, blockId);
		return true;
	}
	return false;
}

bool Map::setBlock(int x, int y, int layer, BlockId blockId){
	if (!mGrid.inArea(x,y,layer))
		return false;

	mGrid.at(x,y,layer) = blockId;
	return true;
}
