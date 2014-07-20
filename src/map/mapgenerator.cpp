#include "mapgenerator.h"
#include "map.h"
#include "block.h"

#include <noise/noise.h>
#include <random>

using namespace std;

namespace MapGenerator{
	class IMapStructure {
	public:
		double mRarity = 0.01;

		virtual void place(Map& map, std::mt19937& rand, int x, int y) = 0;

		void placeMany(Map& map, std::mt19937& rand){
			int count = map.getWidth() * map.getHeight() * mRarity * mRarity;

			std::uniform_int_distribution<> xDist(0, map.getWidth());
			std::uniform_int_distribution<> yDist(0, map.getHeight());

			for (int i = 0; i < count; i++){
				auto x = xDist(rand);
				auto y = yDist(rand);
				place(map, rand, x, y);
			}
		}
		virtual ~IMapStructure(){}
	};

	class VeinStructure : public IMapStructure {
	public:
		int mOreId;
		int mLength;
		double mTwisty;

		VeinStructure(double rarity, int oreId, int length, double twisty) {
			mRarity = rarity;
			mOreId = oreId;
			mLength = length;
			mTwisty = twisty;
		}

		void place(Map& map, std::mt19937& rand, int x, int y) override{
			std::normal_distribution<> rotationChange(0,mTwisty);
			std::normal_distribution<> positionChange(0,1);
			std::uniform_real_distribution<> rotationDist(0, 6.283);
			double rotation = rotationDist(rand);

			double curX = x, curY = y;

			for (int i = 0; i < mLength; i++){
				curX += std::sin(rotation);
				curY += std::cos(rotation);
				rotation += rotationChange(rand);

				x = (int) (curX + positionChange(rand));
				y = (int) (curY + positionChange(rand));

				if (map.blockPlayer(x, y, 1))
					map.setBlock(x, y, 2, mOreId);
			}
		}
	};

	struct Biome{
		int mSoilHeight, mMountainHeight, mMountainWidth, mGroundLevel = 80;

		int mSoilBlock;
		int mStoneBlock;
		int mGrassBlock;
		std::vector<std::unique_ptr<IMapStructure>> mapStructures;

		Biome(const BlockList& blockList){
			mSoilBlock = blockList.getId("dirt");
			mStoneBlock = blockList.getId("cobble");
			mGrassBlock = blockList.getId("grass");

			mSoilHeight = 4;
			mMountainHeight = 50;
			mMountainWidth = 100;
			mGroundLevel = 80;

			mapStructures.push_back(std::unique_ptr<IMapStructure>(new VeinStructure(0.003, blockList.getId("gem1"), 10, 0.1)));
			mapStructures.push_back(std::unique_ptr<IMapStructure>(new VeinStructure(0.01, blockList.getId("gem2"), 10, 0.2)));
			mapStructures.push_back(std::unique_ptr<IMapStructure>(new VeinStructure(0.02, blockList.getId("gem3"), 10, 0.5)));
		}

		void placeBasicTerrain(Map& map, int layer) const;
	};

	int getHeight(const Map& map, int x);

	// PUBLIC
	unique_ptr<Map> generate(const BlockList& blockList, const std::string& type, int seed){
		std::mt19937 gen;
		gen.seed(seed);

		unique_ptr<Map> map;
		map.reset(new Map{blockList, 1024, 256});
		Biome biome(blockList);
		biome.placeBasicTerrain(*map, 0);
		biome.placeBasicTerrain(*map, 1);

		for (auto& mapStructure : biome.mapStructures){
			mapStructure->placeMany(*map, gen);
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

	void Biome::placeBasicTerrain(Map& map, int layer) const{
		for(int x = 0; x < map.getWidth(); x++){
			noise::module::Perlin heightMap;
			int groundLevel = (int) (heightMap.GetValue(x / (float)mMountainWidth,0,0) * mMountainHeight) + mGroundLevel;
			if (layer == 0)
				groundLevel = std::min(groundLevel, (int) (heightMap.GetValue(x / (float)mMountainWidth,0,-0.03) * mMountainHeight) + mGroundLevel);

			int dirtHeight = (int) mSoilHeight;
			for(int y = 0; y < map.getHeight(); y++){
				int blockId = -1;

				if (y > groundLevel + dirtHeight) {
					// stone layer
					blockId = mStoneBlock;
				} else if (y > groundLevel) {
					// dirt layer
					blockId = mSoilBlock;
				} else if (y == groundLevel){
					blockId = mGrassBlock;
				}
				map.idAt(x,y,layer) = blockId;
			}
		}
	}
}
