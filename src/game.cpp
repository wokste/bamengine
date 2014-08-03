#include <SFML/Graphics.hpp>
#include "game.h"
#include "map/map.h"
#include "map/block.h"
#include "map/maprenderer.h"
#include "map/mapserializer.h"
#include "mapgen/mapgenerator.h"
#include "util/vectormath.h"
#include "util/skybox.h"

#include <iostream>

class Game : public IGame{
	std::unique_ptr<Map> mMap;
	std::unique_ptr<Skybox> mSkybox;
	sf::Vector2f mCenterPos{200.0f,200.0f};
	const BlockList mBlockList;
	std::unique_ptr<IMapRenderer> mMapRenderer;
	std::shared_ptr<sf::Texture> mTexture;

public:
	Game() : mBlockList("data/blocks.csv"){
		mMap = MapGenerator::generate(mBlockList, "plains", 1337);
		mSkybox.reset( new Skybox() );
		mMapRenderer = IMapRenderer::factory();
	}

	~Game(){

	}

	void display(sf::RenderTarget& target, float interpolation) override{
		sf::View v = target.getView();
		v.setCenter(mCenterPos + interpolation * getCameraVelocity());
		target.setView(v);

		mSkybox->render(target);
		mMapRenderer->render(*mMap, target);
	}

	void logic(bool hasFocus) override{
		mSkybox->tick();
		if (hasFocus){
			mCenterPos += getCameraVelocity();
		}
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
		} catch (FileException& ex) {
			std::cout << ex.describe();
		}
	}

	void save(const std::string& mapname) override{
		const std::string totalMapName = "data/maps/" + mapname + ".map";
		try {
			MapSerializer::writeMap(totalMapName, *mMap);
		} catch (FileException& ex) {
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
