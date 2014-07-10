#include "skybox.h"
#include <SFML/Graphics/RenderTarget.hpp>

Skybox::Skybox(){
	std::string filename = "data/skybox_plains.png";
	mTexture = std::make_shared<sf::Texture>();
	mTexture->loadFromFile(filename);
	mPhaseColor[Day] = sf::Color(128,192,255);
	mPhaseColor[Night] = sf::Color(0,32,64);

	int ticksPerMinute = 25;
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

void Skybox::render(sf::RenderTarget& target) const{
	Phase phase = getPhase();

	target.clear(mPhaseColor[phase]);
}
