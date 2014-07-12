#include "skybox.h"
#include "mathplus.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

Skybox::Skybox(){
	std::string filename = "data/skybox_plains.png";
	mTexture = std::make_shared<sf::Texture>();
	mTexture->loadFromFile(filename);
	mTexture->setRepeated(true);
	mPhaseColor[Day] = sf::Color(128,192,255);
	mPhaseColor[Night] = sf::Color(0,32,64);

	int ticksPerMinute = 25 * 30;
	mPhaseTime[Day] = 5 * ticksPerMinute;
	mPhaseTime[Night] = 6 * ticksPerMinute;
	mTicks = mPhaseTime[Day];
	mInterpolationTime = ticksPerMinute * 1;
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
	renderSky(window);

	if (getPhase() == Phase::Day){
		sf::IntRect rect(0,0,128,128);
		renderSun(window, rect, 0, mPhaseTime[Phase::Day] - mInterpolationTime);
	} else {
		sf::IntRect rect(128,0,128,128);
		renderSun(window, rect, mPhaseTime[Phase::Day] - mInterpolationTime, mPhaseTime[Phase::Night]);
	}

	renderTerrain(window, 128, 384, 0.3);
	renderTerrain(window, 128, 256, 0.6);
}

void Skybox::renderSky(sf::RenderTarget& window) const{
	Phase phase = getPhase();

	if (mTicks + mInterpolationTime >= mPhaseTime[phase]){
		auto& oldColor = mPhaseColor[phase];
		auto& newColor = mPhaseColor[(phase + 1) % Phase::Max];

		double interpolation = 1 - static_cast<double>(mPhaseTime[phase] - mTicks) / static_cast<double>(mInterpolationTime);

		window.clear(sf::Color(
			MathPlus::interpolate(oldColor.r, newColor.r, interpolation),
			MathPlus::interpolate(oldColor.g, newColor.g, interpolation),
			MathPlus::interpolate(oldColor.b, newColor.b, interpolation)));
	} else {
		window.clear(mPhaseColor[phase]);
	}
}

void Skybox::renderTerrain(sf::RenderTarget& window, int imgTop, int imgHeight, double scrollSpeed) const{
	// Draw the terain
	sf::Sprite sprite;
	sprite.setTexture(*mTexture);

	sf::Vector2u windowSize = window.getSize();

	int terrainHeight = std::min(imgHeight, static_cast<int>(windowSize.y / 2));

	sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(0,windowSize.y - terrainHeight ));

	sprite.setPosition(worldPos);
	sprite.setScale(sf::Vector2f(0.5f, 0.5f));

	int skyLeft = static_cast<int>(window.getView().getCenter().x * scrollSpeed);

	sprite.setTextureRect(sf::IntRect(skyLeft, imgTop, windowSize.x, imgHeight));

	window.draw(sprite);
}

void Skybox::renderSun(sf::RenderTarget& window, const sf::IntRect& rect, int tickStart, int tickEnd) const{
	if (mTicks < tickStart || mTicks > tickEnd)
		return;

	double time = static_cast<double>(mTicks - tickStart) / static_cast<double>(tickEnd - tickStart);

	sf::Sprite sprite;
	sprite.setTexture(*mTexture);
	sprite.setTextureRect(rect);
	sprite.setOrigin(64,64);
	float scale = 0.3f;
	sprite.setScale(scale, scale);

	constexpr double pi = 3.141592;

	sf::Vector2u windowSize = window.getSize();

	sf::Vector2i center(windowSize.x / 2, windowSize.y - 200);
	sf::Vector2f radius(windowSize.x * 0.4, windowSize.x * 0.2);
	double angle = pi * (time) + pi;
	sf::Vector2i translate(std::cos(angle) * radius.x, std::sin(angle) * radius.y);
	sf::Vector2f worldPos = window.mapPixelToCoords(center + translate);

	sprite.setPosition(worldPos);

	window.draw(sprite);
}

