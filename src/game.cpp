#include <SFML/Graphics.hpp>
#include "game.h"
#include "map/map.h"
#include "map/maprenderer.h"
#include "map/mapgenerator.h"
#include "util/vectormath.h"

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
