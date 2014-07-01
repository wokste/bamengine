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

		Block* m_soilBlock = new Block{0,2};
		Block* m_stoneBlock = new Block{2,4};

		void placeBasicTerrain(Map& map, int layer) const;
	};

	int getHeight(const Map& map, int x);

	// PUBLIC
	unique_ptr<Map> generate(const std::string& type, int seed){
		unique_ptr<Map> map;
		map.reset(new Map{1024, 256});
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
				Block* block = nullptr;

				if (y > groundLevel + dirtHeight) {
					// stone layer
					block = m_stoneBlock;
				} else if (y > groundLevel) {
					// dirt layer
					block = m_soilBlock;
				}
				//if (block != nullptr)
					map.setBlock(x,y,layer,block);
			}
		}
	}
}
