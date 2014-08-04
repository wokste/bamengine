#pragma once
#include <memory>
#include <string>

class Map;
class BlockList;

namespace MapGenerator{
	std::unique_ptr<Map> generate(const BlockList& blockList, const std::string& type, int seed);
};
