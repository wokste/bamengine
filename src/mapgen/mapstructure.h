#pragma once
#include <random>
#include <memory>
#include <vector>

class Map;
class BlockList;
class TokenStream;

namespace MapGenerator{
	class IMapStructure {
	public:
		double mRarity;
		virtual void place(Map& map, std::mt19937& rand, int x, int y) = 0;
		void placeMany(Map& map, std::mt19937& rand);
	};

	std::vector<std::unique_ptr<IMapStructure>> makeStructureVector(TokenStream& stream, const BlockList& blockList);
	std::unique_ptr<IMapStructure> makeStructure(TokenStream& stream, const BlockList& blockList);
}
