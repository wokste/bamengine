#pragma once
#include <vector>
#include <string>

typedef uint16_t BlockId;

class Block{
public:
	std::string mTag;
	int mFrameStart;
	int mFrameCount;
	bool mSolid;

	Block(){}

	constexpr static BlockId air = 0xffff;
};

class BlockList{
	std::vector<Block> mBlocks;
public:
	BlockList(const std::string& fileName);
	const Block& operator[](unsigned i) const;
	int getId(const std::string& tag) const;
	int getId(const char* tag) const;
};
