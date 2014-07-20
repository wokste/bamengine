#include "mapstructure.h"
#include "../map/map.h"
#include "../map/block.h"
#include "../util/tokenstream.h"

using namespace std;

namespace MapGenerator{
	void IMapStructure::placeMany(Map& map, mt19937& rand){
		int count = map.getWidth() * map.getHeight() * mRarity * mRarity;

		uniform_int_distribution<> xDist(0, map.getWidth());
		uniform_int_distribution<> yDist(0, map.getHeight());

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

		void place(Map& map, mt19937& rand, int x, int y) override{
			normal_distribution<> rotationChange(0,mTwisty);
			normal_distribution<> positionChange(0,1);
			uniform_real_distribution<> rotationDist(0, 6.283);
			double rotation = rotationDist(rand);

			double curX = x, curY = y;

			for (int i = 0; i < mLength; i++){
				curX += sin(rotation);
				curY += cos(rotation);
				rotation += rotationChange(rand);

				x = (int) (curX + positionChange(rand));
				y = (int) (curY + positionChange(rand));

				if (map.blockPlayer(x, y, 1))
					map.setBlock(x, y, 2, mOreId);
			}
		}
	};

	unique_ptr<IMapStructure> makeStructure(TokenStream& stream, const BlockList& blockList){
		string type = stream.readString();
		if (type == "vein"){
			TokenStreamPusher push(stream);

			double rarity = stream.readFloat();
			int oreId = blockList.getId(stream.readString());
			int length = stream.readInt();
			double twisty = stream.readFloat();
			return unique_ptr<IMapStructure>(new VeinStructure(rarity, oreId, length, twisty));
		} else {
			return nullptr;
		}
	}

	std::vector<std::unique_ptr<IMapStructure>> makeStructureVector(TokenStream& stream, const BlockList& blockList){
		std::vector<std::unique_ptr<IMapStructure>> vec;
		string type = stream.readString();
		if (type == "array"){
			TokenStreamPusher push(stream);

			while (stream.hasArguments()){
				auto elem = makeStructure(stream, blockList);
				if (elem)
					vec.push_back(std::move(elem));
			}
		}
		return vec;
	}
}
