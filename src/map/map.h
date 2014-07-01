#pragma once
#include "grid.h"
#include <cmath>

class Block;
class BlockList;

class Map{
	const BlockList& mList;
	Grid<int> mGrid;

public:
	Map(const BlockList& list, int w, int h);
	bool solid(int x, int y, int layer) const;
	const Block* getBlock(int x, int y, int layer) const;
	int& idAt(int x, int y, int layer);
	bool placeBlock(int x, int y, int layer, const int blockId);
	bool setBlock(int x, int y, int layer, const int blockId);

	int getWidth() const { return mGrid.getWidth(); }
	int getHeight() const { return mGrid.getHeight(); }
};
