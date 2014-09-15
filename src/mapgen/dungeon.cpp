#include "dungeon.h"
#include "../map/map.h"
#include "../map/block.h"
#include "../util/vectormath.h"
#include <random>
#include <iostream>

using namespace std;

namespace MapGenerator{
	Room::Room(int backBlockId, int frontBlockId, sf::IntRect rect)
		: mBackBlockId(backBlockId), mFrontBlockId(frontBlockId), mRect(rect), mConnected(false), mDistance(999999), mParent(0)
	{}

	Room::~Room() = default;

	void Room::place(Map& map, mt19937& rand){
		int wall = 2;

		for (int x = mRect.left; x < mRect.left + mRect.width; x++){
			for (int y = mRect.top; y < mRect.top + mRect.height; y++){
				map.setBlock(x, y, 0, mBackBlockId);
				if (
					x >= mRect.left + wall && x < mRect.left + mRect.width - wall &&
					y >= mRect.top + wall && y < mRect.top + mRect.height - wall)
				{
					map.setBlock(x, y, 1, Block::air);
				} else {
					map.setBlock(x, y, 1, mFrontBlockId);
				}
			}
		}
	}

	sf::Vector2i Room::getCenter() const{
		return sf::Vector2i{mRect.left + mRect.width / 2, mRect.top + mRect.height / 2};
	}

	Corridor::Corridor(int roomId1, int roomId2){
		mRoomId1 = roomId1;
		mRoomId2 = roomId2;
	}

	Corridor::~Corridor() = default;

	void Corridor::place(Dungeon& dungeon, Map& map, mt19937& rand){
		Room* room1 = &dungeon.mRooms[mRoomId1];
		Room* room2 = &dungeon.mRooms[mRoomId2];

		if (room1->getCenter().x > room2->getCenter().x){
			swap(room1, room2);
		}

		auto point1 = sf::Vector2i{room1->mRect.left + room1->mRect.width - 2, room1->mRect.top + room1->mRect.height - 2};
		auto point2 = sf::Vector2i{room2->mRect.left + 2, room2->mRect.top + room2->mRect.height - 2};

		auto delta = point2 - point1;
		float deltaerr = delta.x != 0 ? ((float)delta.y / (float)delta.x) : 1000;
		float y = point1.y;

		for (int x = point1.x; x < point2.x; x++){

			y += deltaerr ;
			int yFloor = (int)(y);

			int yCeil = yFloor - 4;
			for (int y = yCeil - 2; y < yFloor + 2; y++){
				if (map.blockPlayer(x, y, 1)){
					map.setBlock(x, y, 0, room1->mBackBlockId);
					if (y >= yCeil && y < yFloor)
						map.setBlock(x, y, 1, Block::air);
					else
						map.setBlock(x, y, 1, room1->mFrontBlockId);

				}
			}
		}
	}

	Dungeon::Dungeon() = default;
	Dungeon::~Dungeon() = default;

	int Dungeon::addRooms(const Map& map, mt19937& rand, int rooms){
		int backBlockId = 4;
		int frontBlockId = 6;
		//TODO: Load this correctly

		uniform_int_distribution<> roomWidthDist(12, 24);
		uniform_int_distribution<> roomHeightDist(10, 18);

		for (int i = 0; i < rooms; i++){
			int roomWidth = roomWidthDist(rand);
			int roomHeight = roomHeightDist(rand);

			uniform_int_distribution<> roomXDist(0,map.getWidth() - roomWidth);
			uniform_int_distribution<> roomYDist(0,map.getHeight() - roomHeight);

			auto area = sf::IntRect(roomXDist(rand), roomYDist(rand), roomWidth, roomHeight);

			if (validRoomPos(area, map)){
				mRooms.push_back(Room{backBlockId, frontBlockId, area});
			}
		}

		return mRooms.size();
	}

	bool Dungeon::validRoomPos(const sf::IntRect& area, const Map& map) const{
		for(auto x = area.left; x < area.left + area.width; x++){
			if(!map.blockPlayer(x, area.top + area.height, 1))
				return false;
		}

		for( auto& room : mRooms){
			if(area.intersects(room.mRect))
				return false;
		}
		return true;
	}

	void Dungeon::place(Map& map, mt19937& rand){
		for( auto& room : mRooms){
			room.place(map, rand);
		}
		for( auto& corridor : mCorridors){
			corridor.place(*this, map, rand);
		}
	}

	void Dungeon::prim(){
		primConnect(0);

		while(true){
			float len = 99999;
			int roomId;
			for(unsigned int i = 0; i < mRooms.size(); i++){
				if (mRooms[i].mConnected)
					continue;

				if (len > mRooms[i].mDistance){
					roomId = i;
					len = mRooms[i].mDistance;
				}
			}
			if (len >= 99999)
				return;

			primConnect(roomId);
		}

		//TODO: implement Prims algorithm
		//Source: Wikipedia
		//Input: A non-empty connected weighted graph with vertices V and edges E (the weights can be negative).
		//Initialize: Vnew = {x}, where x is an arbitrary node (starting point) from V, Enew = {}
		//Repeat until Vnew = V:
		//    Choose an edge {u, v} with minimal weight such that u is in Vnew and v is not (if there are multiple edges with the same weight, any of them may be picked)
		//    Add v to Vnew, and {u, v} to Enew
		//Output: Vnew and Enew describe a minimal spanning tree
	}

	void Dungeon::primConnect(int roomId){
		if (roomId != mRooms[roomId].mParent){
			mCorridors.push_back(Corridor(roomId, mRooms[roomId].mParent));
		}

		mRooms[roomId].mConnected = true;

		for(auto& room : mRooms){
			if (room.mConnected)
				continue;

			float newLen = VectorMath::length(room.getCenter() - mRooms[roomId].getCenter());
			if (newLen < room.mDistance){
				room.mDistance = newLen;
				room.mParent = roomId;
			}
		}
	}
}

