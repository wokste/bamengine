#pragma once

#include <memory>

class Map;

namespace MapGenerator{
	std::unique_ptr<Map> generate(const std::string& type, int seed);
};
