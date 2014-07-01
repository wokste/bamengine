#include "block.h"
#include "../util/csv.h"
BlockList::BlockList(const std::string& fileName){
	io::CSVReader<3> in("data/blocks.csv");
	in.read_header(io::ignore_extra_column, "tag", "frame_start", "frame_count");

	Block block;

	while(in.read_row(block.tag, block.frameStart, block.frameCount)){
		block.solid = true;
		blocks.push_back(block);
	}
}

const Block& BlockList::operator[](unsigned i) const{
	return blocks[i];
}
