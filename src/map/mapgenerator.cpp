#include "mapgenerator.h"
#include "map.h"
#include "block.h"

#include <noise/noise.h>

using namespace std;

namespace MapGenerator{
	struct Biome{
		int m_soilHeight = 4;
		int m_mountainHeight = 50;
		int m_mountainWidth = 100;
		int m_groundLevel = 80;

		int m_soilBlock = 0;
		int m_stoneBlock = 1;

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
		for(int y = 0; y < map.grid.getHeight(); y++){
			if (map.solid(x,y, 1))
				return y;
		}
		return -1;
	}

	void Biome::placeBasicTerrain(Map& map, int layer) const{
		for(int x = 0; x < map.grid.getWidth(); x++){
			noise::module::Perlin heightMap;
			int groundLevel = (int) (heightMap.GetValue(x / (float)m_mountainWidth,0,0) * m_mountainHeight) + m_groundLevel;
			int dirtHeight = (int) m_soilHeight;// (dirtHeightMap.noise2d(x, 0) * 4) + 4;
			for(int y = 0; y < map.grid.getHeight() ; y++){
				int blockId = -1;

				if (y > groundLevel + dirtHeight) {
					// stone layer
					blockId = m_stoneBlock;
				} else if (y > groundLevel) {
					// dirt layer
					blockId = m_soilBlock;
				}
				map.idAt(x,y,layer) = blockId;
			}
		}
	}
}
