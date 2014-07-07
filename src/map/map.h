#pragma once
#include "grid.h"
#include <cmath>
#include <stdint.h>

class Block;
class BlockList;

typedef uint16_t BlockId;

class Map{
	const BlockList& mList;
	Grid<BlockId> mGrid;

public:
	Map(const BlockList& list, int w, int h);
	bool solid(int x, int y, int layer) const;
	bool blockPlayer(int x, int y, int layer) const;
	const Block* getBlock(int x, int y, int layer) const;
	BlockId& idAt(int x, int y, int layer);
	const BlockId& idAt(int x, int y, int layer) const;
	bool placeBlock(int x, int y, int layer, const BlockId blockId);
	bool setBlock(int x, int y, int layer, const BlockId blockId);

	int getWidth() const { return mGrid.getWidth(); }
	int getHeight() const { return mGrid.getHeight(); }
};
