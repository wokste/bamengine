#pragma once

#include "grid.h"
#include "block.h"
#include <cmath>

class Block;
typedef Block* BlockRef;

class Map{
	std::vector<const Block*> blockTypes;

public:
	Grid<int> grid;
	Map(int w, int h);
	bool solid(int x, int y, int layer) const;
	const Block* getBlock(int x, int y, int layer) const;
	bool placeBlock(int x, int y, int layer, const Block* block);
	bool setBlock(int x, int y, int layer, const Block* block);
};
