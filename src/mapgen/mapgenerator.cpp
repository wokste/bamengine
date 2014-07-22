#include "mapgenerator.h"
#include "mapstructure.h"
#include "../map/map.h"
#include "../map/block.h"
#include "../util/tokenstream.h"

#include <noise/noise.h>
#include <random>

using namespace std;
namespace MapGenerator{
	struct Biome{
		int mSoilHeight, mMountainHeight, mMountainWidth, mGroundLevel = 80;

		int mSoilBlock;
		std::vector<std::pair<int,int>> mStoneBlocks;
		int mGrassBlock;
		std::vector<std::unique_ptr<IMapStructure>> mapStructures;

		Biome(const BlockList& blockList){
			mSoilBlock = blockList.getId("dirt");

			mStoneBlocks.emplace_back(20,blockList.getId("cobble"));
			mStoneBlocks.emplace_back(5,blockList.getId("dirt"));
			mStoneBlocks.emplace_back(15,blockList.getId("gravel"));
			mStoneBlocks.emplace_back(5,blockList.getId("sand"));

			mGrassBlock = blockList.getId("grass");

			mSoilHeight = 4;
			mMountainHeight = 50;
			mMountainWidth = 100;
			mGroundLevel = 80;

			std::string data("array(vein(0.003,gem1,10,0.1),vein(0.01,gem2,10,0.2),vein(0.015,gem3,10,0.5))");
			TokenStream stream(data);
			mapStructures = makeStructureVector(stream, blockList);
		}

		void placeBasicTerrain(Map& map, std::mt19937& random) const;
		int chooseStoneBlock(double value) const;
	};

	int getHeight(const Map& map, int x);

	// PUBLIC
	unique_ptr<Map> generate(const BlockList& blockList, const std::string& type, int seed){
		std::mt19937 rnd;
		rnd.seed(seed);

		unique_ptr<Map> map;
		map.reset(new Map{blockList, 1024, 256});
		const Biome biome(blockList);
		biome.placeBasicTerrain(*map, rnd);

		for (auto& mapStructure : biome.mapStructures){
			mapStructure->placeMany(*map, rnd);
		}

		return map;
	}

	// PRIVATE
	int getHeight(const Map& map, int x){
		for(int y = 0; y < map.getHeight(); y++){
			if (map.blockPlayer(x, y, 1))
				return y;
		}
		return -1;
	}

	void Biome::placeBasicTerrain(Map& map, std::mt19937& random) const{
		std::uniform_int_distribution<int> seedDist(0, (1 << 16) - 1);
		noise::module::Perlin heightMap;
		noise::module::Voronoi voronoi;
		noise::module::Turbulence areas;
		heightMap.SetSeed(seedDist(random));
		heightMap.SetFrequency(1.0 / mMountainWidth);
		voronoi.SetSeed(seedDist(random));
		voronoi.SetFrequency(0.1);
		areas.SetSeed(seedDist(random));
		areas.SetSourceModule(0,voronoi);
		areas.SetFrequency(0.2);

		for(int x = 0; x < map.getWidth(); x++){
			int groundLevel = (int) (heightMap.GetValue(x,0,0) * mMountainHeight) + mGroundLevel;

			int dirtHeight = (int) mSoilHeight;
			for(int y = 0; y < map.getHeight(); y++){
				int blockId = Block::air;

				if (y > groundLevel + dirtHeight) {
					// stone layer
					blockId = chooseStoneBlock(areas.GetValue(x,y,0));
				} else if (y > groundLevel) {
					// dirt layer
					blockId = mSoilBlock;
				} else if (y == groundLevel){
					blockId = mGrassBlock;
				}

				for(int layer = 0; layer < 2; layer++){
					map.idAt(x,y,layer) = blockId;
				}
			}
		}

		/*for(int x = 0; x < map.getWidth(); x++){
			for(int y = 0; y < map.getHeight(); y++){
				if ((x & 3) || (y & 3)){
					for(int layer = 0; layer < 2; layer++)
						map.idAt(x,y,layer) = map.idAt(x & ~3,y & ~3,layer);
				}
			}
		}*/
	}

	int Biome::chooseStoneBlock(double value) const{
		int maxValue = 0;
		for (auto& pair : mStoneBlocks){
			maxValue += pair.first;
		}

		int valueAsInt = (value + 1) / 2 * maxValue;

		for (auto& pair : mStoneBlocks){
			valueAsInt -= pair.first;
			if (valueAsInt < 0)
				return pair.second;
		}
		return Block::air;
	}
}
