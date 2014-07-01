#pragma once
#include <vector>
#include <string>

class Block{
public:
	std::string tag;
	int frameStart;
	int frameCount;
	bool solid;

	Block(){}
};

class BlockList{
	std::vector<Block> blocks;
public:
	BlockList(const std::string& fileName);
	const Block& operator[](unsigned i) const;
};
