#include <SFML/Graphics.hpp>
#include "game.h"
#include "map/map.h"
#include "map/maprenderer.h"
#include "map/mapgenerator.h"

namespace VectorMath
{
	template <typename T>
	constexpr T dot(const sf::Vector2<T>& v1, const sf::Vector2<T>& v2){
		return v1.x * v2.x + v1.y * v2.y;
	}

	template <typename T>
	constexpr T length(const sf::Vector2<T>& self){
		return std::sqrt(dot(self,self));
	}

	template <typename T>
	constexpr sf::Vector2<T> normalize(const sf::Vector2<T>& self){
		return self / length(self);
	}
}

class Game : public IGame{
	std::unique_ptr<Map> mMap;
	sf::Vector2f mCenterPos;
	const BlockList mBlockList;

public:
	Game() : mBlockList("data/blocks.csv"){
		mMap = MapGenerator::generate(mBlockList, "plains", 1337);
	}

	void display(sf::RenderTarget& target, float interpolation) override{
		sf::View v = target.getView();
		v.setCenter(mCenterPos + interpolation * getCameraVelocity());
		target.setView(v);

		// Render the map
		MapRenderer mapRenderer;
		mapRenderer.render(*mMap, target, 0);
	}

	void logic() override{
		mCenterPos += getCameraVelocity();
	}

private:
	sf::Vector2f getCameraVelocity(){
		constexpr float camSpeed = 20;

		sf::Vector2f v;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			v.x += 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			v.x -= 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			v.y += 1;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
			v.y -= 1;
		if (VectorMath::length(v) > 1)
			v = VectorMath::normalize(v);
		v *= camSpeed;
		return v;
	}
};

std::unique_ptr<IGame> IGame::factory(){
	return std::unique_ptr<IGame>(new Game());
}
