#pragma once
#include <vector>
#include <string>

typedef uint16_t BlockId;

class Block{
public:
	enum class Graphics{
		Solid, NonSolid, RepeatX, RepeatY
	};

	std::string mTag;
	int mFrameStart;
	int mFrameCount;
	Graphics mGraphics;
	struct Flags{
		int mBlockPlayer : 1;
		int mGravity : 1;
		int mTopLayer : 1;
	} mFlags;

	Block(){}

	void setGraphics(const std::string& source);

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
