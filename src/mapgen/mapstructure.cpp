#include "mapstructure.h"
#include "../map/map.h"
#include "../map/block.h"
#include "../util/tokenstream.h"
#include <SFML/Graphics/Rect.hpp>

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

	class DungeonStructure : public IMapStructure {
	public:
		int mBlockId;
		int mRooms;

		DungeonStructure(double rarity, int blockId, int rooms) {
			mRarity = rarity;
			mBlockId = blockId;
			mRooms = rooms;
		}

		void place(Map& map, mt19937& rand, int x, int y) override{
			uniform_real_distribution<> horChange(15,20);
			uniform_real_distribution<> vertChange(5,9);
			bernoulli_distribution goRight(0.5);

			int curX = x, curY = y;
			int roomX = 9, roomY = 6;

			for (int i = 0; i < mRooms; i++){
				placeRoom(map, sf::IntRect(curX, curY, roomX, roomY));

				if (i + 1 < mRooms){
					// Connect rooms with corridor if it is not the last room.
					int dx = horChange(rand);
					int dy = vertChange(rand);

					if (!goRight(rand))
						dx *= -1;

					if (dx > 0){
						placeCorridor(map, sf::Vector2i(curX + roomX, curY + roomY), sf::Vector2i(curX + dx, curY + dy + roomY));
					} else {
						placeCorridor(map, sf::Vector2i(curX, curY + roomY), sf::Vector2i(curX + dx + roomX, curY + dy + roomY));
					}

					curX += dx;
					curY += dy;
				}
			}
		}

		void placeCorridor(Map& map, sf::Vector2i point1, sf::Vector2i point2){
			if (point1.x > point2.x)
				std::swap(point1, point2);

			auto delta = point2 - point1;

			int yFloor = point1.y;
			for (int x = point1.x; x < point2.x; x++){
				if (yFloor < point2.y)
					yFloor++;
				else if (yFloor > point2.y)
					yFloor--;

				int yCeil = yFloor - 4;
				for (int y = yCeil - 2; y < yFloor + 2; y++){
					if (map.blockPlayer(x, y, 1)){
						map.setBlock(x, y, 0, mBlockId);
						if (y >= yCeil && y < yFloor)
							map.setBlock(x, y, 1, Block::air);
						else
							map.setBlock(x, y, 1, mBlockId);

					}
				}
			}
		}

		void placeRoom(Map& map, sf::IntRect rect){
			int wall = 2;

			for (int x = rect.left - wall; x < rect.left + rect.width + wall; x++){
				for (int y = rect.top - wall; y < rect.top + rect.height + wall; y++){
					if (map.blockPlayer(x, y, 1)){
						map.setBlock(x, y, 0, mBlockId);
						if (rect.contains(x,y))
							map.setBlock(x, y, 1, Block::air);
						else
							map.setBlock(x, y, 1, mBlockId);

					}
				}
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
		} else if (type == "dungeon"){
			TokenStreamPusher push(stream);

			double rarity = stream.readFloat();
			int blockId = blockList.getId(stream.readString());
			int rooms = stream.readInt();
			return unique_ptr<IMapStructure>(new DungeonStructure(rarity, blockId, rooms));
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
