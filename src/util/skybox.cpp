#include "skybox.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

Skybox::Skybox(){
	std::string filename = "data/skybox_plains.png";
	mTexture = std::make_shared<sf::Texture>();
	mTexture->loadFromFile(filename);
	mTexture->setRepeated(true);
	mPhaseColor[Day] = sf::Color(128,192,255);
	mPhaseColor[Night] = sf::Color(0,32,64);

	int ticksPerMinute = 25 * 6;
	mPhaseTime[Day] = 4 * ticksPerMinute;
	mPhaseTime[Night] = 5 * ticksPerMinute;
}

void Skybox::tick(){
	mTicks++;
	if (mTicks >= mPhaseTime[Phase::Max - 1])
		mTicks = 0;
}

Skybox::Phase Skybox::getPhase() const{
	for(int i = 0; i < Phase::Max; i++){
		if (mTicks < mPhaseTime[i])
			return static_cast<Skybox::Phase>(i);
	}
	return Night;
}

void Skybox::render(sf::RenderTarget& window) const{
	Phase phase = getPhase();

	window.clear(mPhaseColor[phase]);

	// Draw the terain
	sf::Sprite sprite;
	sprite.setTexture(*mTexture);

	sf::Vector2u windowSize = window.getSize();

	int terrainHeight = std::min(384, static_cast<int>(windowSize.y / 2));

	sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(0,windowSize.y - terrainHeight ));

	sprite.setPosition(worldPos);
	sprite.setScale(sf::Vector2f(0.5f, 0.5f));

	int skyLeft = static_cast<int>(window.getView().getCenter().x * 0.5);

	sprite.setTextureRect(sf::IntRect(skyLeft, 128, windowSize.x, 384));

	window.draw(sprite);
}
