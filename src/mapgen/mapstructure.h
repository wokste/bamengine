#pragma once
#include <random>

class Map;

namespace MapGenerator{
	class IMapStructure {
	public:
		double mRarity;
		virtual void place(Map& map, std::mt19937& rand, int x, int y) = 0;
		void placeMany(Map& map, std::mt19937& rand);
	};
}
