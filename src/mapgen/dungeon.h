#pragma once
#include <random>
#include <SFML/Graphics/Rect.hpp>

class Map;

namespace MapGenerator{
	class Room {
	public:
		int mBackBlockId, mFrontBlockId;
		sf::IntRect mRect;

		Room(int backBlockId, int frontBlockId, sf::IntRect rect);
		~Room();

		void place(Map& map, std::mt19937& rand);
		sf::Vector2i getCenter() const;
	};

	class Dungeon;

	class Corridor{
	public:
		int mRoomId1, mRoomId2;
		Corridor(int, int);
		~Corridor();
		void place(Dungeon& dungeon, Map& map, std::mt19937& rand);
	};

	class Dungeon {
		std::vector<Room> mRooms;
		std::vector<Corridor> mCorridors;
	public:
		Dungeon();
		~Dungeon();

		int addRooms(const Map& map, std::mt19937& rand, int rooms);
		void placeCorridors();
		void place(Map& map, std::mt19937& rand);
	private:
		bool validRoomPos(const sf::IntRect& r, const Map& map) const;

		friend class Corridor;
	};
}
