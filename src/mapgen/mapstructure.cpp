#include "mapstructure.h"
#include "../map/map.h"

namespace MapGenerator{
	void IMapStructure::placeMany(Map& map, std::mt19937& rand){
		int count = map.getWidth() * map.getHeight() * mRarity * mRarity;

		std::uniform_int_distribution<> xDist(0, map.getWidth());
		std::uniform_int_distribution<> yDist(0, map.getHeight());

		for (int i = 0; i < count; i++){
			auto x = xDist(rand);
			auto y = yDist(rand);
			place(map, rand, x, y);
		}
	}

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
}
