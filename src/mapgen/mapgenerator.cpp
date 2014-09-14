#include "mapgenerator.h"
#include "mapstructure.h"
#include "dungeon.h"
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
		void placeHeightMap(Map& map, std::mt19937& random) const;
		int chooseStoneBlock(double value) const;
	};

	int getHeight(const Map& map, int x);

	// PUBLIC
	unique_ptr<Map> generate(const BlockList& blockList, const std::string& type, int seed){
		std::mt19937 rnd;
		rnd.seed(seed);

		unique_ptr<Map> map;
		map.reset(new Map{blockList, 2048, 512});
		const Biome biome(blockList);
		biome.placeBasicTerrain(*map, rnd);
		biome.placeHeightMap(*map, rnd);

		Dungeon dungeon;
		dungeon.addRooms(*map, rnd, 4096);
		dungeon.placeCorridors();
		dungeon.place(*map, rnd);

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

	void placeFineGrainTerrain(Map& map, std::mt19937& random, int scale);

	void Biome::placeBasicTerrain(Map& map, std::mt19937& random) const{
		std::uniform_real_distribution<double> dist(0, 1);

		for(int x = 0; x < map.getWidth(); x+= 16){
			for(int y = 0; y < map.getHeight(); y+= 16){
				int blockId = chooseStoneBlock(dist(random));

				for(int layer = 0; layer < 2; layer++){
					map.idAt(x,y,layer) = blockId;
				}
			}
		}

		placeFineGrainTerrain(map, random, 16);
		placeFineGrainTerrain(map, random, 8);
		placeFineGrainTerrain(map, random, 4);
		placeFineGrainTerrain(map, random, 2);
	}

	void placeFineGrainTerrain(Map& map, std::mt19937& random, int scale){
		int half = scale / 2;
		for (int x = 0; x < map.getWidth(); x+=scale){
			for (int y = 0; y < map.getHeight(); y+=scale){
				int xs = x;
				int ys = y;

				xs += ((random() & 1) == 1) ? scale : 0;
				ys += ((random() & 1) == 1) ? scale : 0;

				xs %= map.getWidth();
				ys %= map.getHeight();

				map.idAt(x + half,y + half,0) = map.idAt(xs,ys,0);
				map.idAt(x + half,y + half,1) = map.idAt(xs,ys,1);
			}
		}

		for (int x = 0; x < map.getWidth(); x+=half){
			for (int y = 0; y < map.getHeight(); y+=half){
				if ((x & half) == (y & half))
					continue;
				int xs = x;
				int ys = y;
				if ((random() & 1) == 1)
					xs += ((random() & 1) == 1) ? half : -half;
				else
					ys += ((random() & 1) == 1) ? half : -half;

				xs = (xs + map.getWidth()) % map.getWidth();
				ys = (ys + map.getHeight()) % map.getHeight();

				map.idAt(x,y,0) = map.idAt(xs,ys,0);
				map.idAt(x,y,1) = map.idAt(xs,ys,1);
			}
		}
	}

	void Biome::placeHeightMap(Map& map, std::mt19937& random) const{
		std::uniform_int_distribution<int> seedDist(0, (1 << 16) - 1);
		noise::module::Perlin heightMap;
		heightMap.SetSeed(seedDist(random));
		heightMap.SetFrequency(1.0 / mMountainWidth);

		for(int x = 0; x < map.getWidth(); x++){
			double alpha = x / static_cast<double>(map.getWidth()) * 2 * 3.141592;
			double r = map.getWidth() / 2 / 3.141592;

			int groundLevel = map.getHeight();
			for(int layer = 0; layer < 2; layer++){
				groundLevel = std::min(groundLevel, (int) (heightMap.GetValue(r * std::sin(alpha), r * std::cos(alpha),layer) * mMountainHeight) + mGroundLevel);
				int dirtHeight = (int) mSoilHeight;
				for(int y = 0; y < groundLevel + dirtHeight + 1; y++){
					if (y < groundLevel) {
						map.idAt(x,y,layer) = Block::air;
					} else if (y == groundLevel) {
						map.idAt(x,y,layer) = mGrassBlock;
					} else if (y <= groundLevel + dirtHeight){
						map.idAt(x,y,layer) = mSoilBlock;
					}
				}
			}
		}
	}


	int Biome::chooseStoneBlock(double value) const{
		int maxValue = 0;
		for (auto& pair : mStoneBlocks){
			maxValue += pair.first;
		}

		int valueAsInt = (value) * maxValue;

		for (auto& pair : mStoneBlocks){
			valueAsInt -= pair.first;
			if (valueAsInt < 0)
				return pair.second;
		}
		return Block::air;
	}
}
