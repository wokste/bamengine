#pragma once
#include <vector>
#include <string>
#include <cstdint>

typedef uint16_t BlockId;

class Block{
public:
	std::string mTag = "";
	int mFrame = 0;

	struct Flags{
		int mDrawBack : 1;
		int mFrameBorders : 1;
		int mBlockPlayer : 1;
		int mGravity : 1;
		int mTopLayer : 1;
	} mFlags;

	Block() = default;

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
