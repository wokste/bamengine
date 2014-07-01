#pragma once

#include "grid.h"
#include "block.h"
#include <cmath>

class Block;

class Map{
	const BlockList& list;

public:
	Grid<int> grid;
	Map(const BlockList& list, int w, int h);
	bool solid(int x, int y, int layer) const;
	const Block* getBlock(int x, int y, int layer) const;
	int& idAt(int x, int y, int layer);
	bool placeBlock(int x, int y, int layer, const int blockId);
	bool setBlock(int x, int y, int layer, const int blockId);
};
