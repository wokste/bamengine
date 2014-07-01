#pragma once
#include <vector>
#include <string>

class Block{
public:
	std::string mTag;
	int mFrameStart;
	int mFrameCount;
	bool mSolid;

	Block(){}
};

class BlockList{
	std::vector<Block> blocks;
public:
	BlockList(const std::string& fileName);
	const Block& operator[](unsigned i) const;
};
