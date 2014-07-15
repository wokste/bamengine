#include <SFML/Graphics.hpp>
#include "game.h"
#include "map/map.h"
#include "map/block.h"
#include "map/maprenderer.h"
#include "map/mapgenerator.h"
#include "map/mapserializer.h"
#include "util/vectormath.h"
#include "util/skybox.h"

#include <iostream>

class Game : public IGame{
	std::unique_ptr<Map> mMap;
	std::unique_ptr<Skybox> mSkybox;
	sf::Vector2f mCenterPos;
	const BlockList mBlockList;

public:
	Game() : mBlockList("data/blocks.csv"){
		mMap = MapGenerator::generate(mBlockList, "plains", 1337);
		mSkybox.reset( new Skybox() );
	}

	void display(sf::RenderTarget& target, float interpolation) override{
		sf::View v = target.getView();
		v.setCenter(mCenterPos + interpolation * getCameraVelocity());
		target.setView(v);

		mSkybox->render(target);
		// Render the map
		MapRenderer mapRenderer;
		mapRenderer.render(*mMap, target);
	}

	void logic() override{
		mSkybox->tick();
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

	void load(const std::string& mapname) override{
		const std::string totalMapName = "data/maps/" + mapname + ".map";
		try {
			mMap = MapSerializer::readMap(mBlockList, totalMapName);
		} catch (FileException ex) {
			std::cout << ex.describe();
		}
	}

	void save(const std::string& mapname) override{
		const std::string totalMapName = "data/maps/" + mapname + ".map";
		try {
			MapSerializer::writeMap(totalMapName, *mMap);
		} catch (FileException ex) {
			std::cout << ex.describe();
		}
	}

	void generate(const std::string& biome, int seed) override{
		mMap = MapGenerator::generate(mBlockList, biome, seed);
	}
};

std::unique_ptr<IGame> IGame::factory(){
	return std::unique_ptr<IGame>(new Game());
}
