#include "block.h"
#include "../util/assert.h"
#include "../util/csv.h"
#include <algorithm>

BlockList::BlockList(const std::string& fileName){
	io::CSVReader<3> in("data/blocks.csv");
	in.read_header(io::ignore_extra_column, "tag", "frame_start", "frame_count");

	Block block;

	while(in.read_row(block.mTag, block.mFrameStart, block.mFrameCount)){

		block.mFlags.mBlockPlayer = true;
		block.mFlags.mSolid = true;
		block.mFlags.mGravity = true;
		mBlocks.push_back(block);
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
