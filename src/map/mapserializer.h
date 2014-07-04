#pragma once
#include <memory>
#include <string>
#include "map.h"

namespace MapSerializer{
	std::unique_ptr<Map> readMap(const BlockList& blockList, const std::string& filename);
	void writeMap(const std::string& filename, const Map& map);
};
