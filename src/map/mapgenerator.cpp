#include "mapgenerator.h"
#include "map.h"
#include "block.h"

#include <noise/noise.h>

using namespace std;

namespace MapGenerator{
	struct Biome{
		int mSoilHeight = 4;
		int mMountainHeight = 50;
		int mMountainWidth = 100;
		int mGroundLevel = 80;

		int mSoilBlock = 0;
		int mStoneBlock = 1;

		void placeBasicTerrain(Map& map, int layer) const;
	};

	int getHeight(const Map& map, int x);

	// PUBLIC
	unique_ptr<Map> generate(const BlockList& blockList, const std::string& type, int seed){
		unique_ptr<Map> map;
		map.reset(new Map{blockList, 1024, 256});
		Biome biome;
		biome.placeBasicTerrain(*map, 0);
		biome.placeBasicTerrain(*map, 1);

		return map;
	}

	// PRIVATE
	int getHeight(const Map& map, int x){
		for(int y = 0; y < map.getHeight(); y++){
			if (map.solid(x, y, 1))
				return y;
		}
		return -1;
	}

	void Biome::placeBasicTerrain(Map& map, int layer) const{
		for(int x = 0; x < map.getWidth(); x++){
			noise::module::Perlin heightMap;
			int groundLevel = (int) (heightMap.GetValue(x / (float)mMountainWidth,0,0) * mMountainHeight) + mGroundLevel;
			int dirtHeight = (int) mSoilHeight;
			for(int y = 0; y < map.getHeight(); y++){
				int blockId = -1;

				if (y > groundLevel + dirtHeight) {
					// stone layer
					blockId = mStoneBlock;
				} else if (y > groundLevel) {
					// dirt layer
					blockId = mSoilBlock;
				}
				map.idAt(x,y,layer) = blockId;
			}
		}
	}
}
