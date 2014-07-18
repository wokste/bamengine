#include "block.h"
#include "../util/assert.h"
#include "../util/csv.h"
#include <algorithm>

BlockList::BlockList(const std::string& fileName){
	io::CSVReader<7> in("data/blocks.csv");
	in.read_header(io::ignore_extra_column, "tag", "frame_start", "frame_count", "graphics", "block_player", "gravity", "top_layer");

	Block block;

	std::string graphics;
	char block_player;
	char gravity;
	char top_layer;

	while(in.read_row(block.mTag, block.mFrameStart, block.mFrameCount, graphics, block_player, gravity, top_layer)){
		block.setGraphics(graphics);
		block.mFlags.mBlockPlayer = (block_player == 'y');
		block.mFlags.mGravity = (gravity == 'y');
		block.mFlags.mTopLayer = (top_layer == 'y');
		mBlocks.push_back(block);
	}
}

void Block::setGraphics(const std::string& source){
	if (source == "nonsolid"){
		mGraphics = Graphics::NonSolid;
	} else if (source == "solid"){
		mGraphics = Graphics::Solid;
	} else if (source == "repeat_x"){
		mGraphics = Graphics::RepeatX;
	} else if (source == "repeat_y"){
		mGraphics = Graphics::RepeatY;
	} else {
		Assert<EnumException>(false, source);
	}
}

const Block& BlockList::operator[](unsigned i) const{
	return mBlocks[i];
}

int BlockList::getId(const std::string& tag) const{
	auto iter = std::find_if(begin(mBlocks), end(mBlocks), [&](Block b){return tag == b.mTag;});
	Assert<UnknownBlock>(iter != end(mBlocks), tag);
	return std::distance(begin(mBlocks), iter);
}

int BlockList::getId(const char* tag) const{
	const std::string str(tag);
	return getId(str);
}
