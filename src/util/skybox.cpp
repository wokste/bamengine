#include "skybox.h"
#include "mathplus.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Sprite.hpp>

Skybox::Skybox(){
	std::string filename = "data/skybox_plains.png";
	mTexture = std::make_shared<sf::Texture>();
	mTexture->loadFromFile(filename);
	mTexture->setRepeated(true);
	mTexture->setSmooth(true);
	mPhaseColor[Day] = sf::Color(128,192,255);
	mPhaseColor[Night] = sf::Color(0,32,64);

	int ticksPerMinute = 25 * 60;
	mPhaseTime[Day] = 10 * ticksPerMinute;
	mPhaseTime[Night] = 12 * ticksPerMinute;
	mTicks = 0;
	mInterpolationTime = ticksPerMinute * 0.5;
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

	renderSun(window, sf::IntRect(0,0,64,64), 0, mPhaseTime[Phase::Day] - mInterpolationTime);
	renderSun(window, sf::IntRect(64,0,64,64), mPhaseTime[Phase::Day] - mInterpolationTime, mPhaseTime[Phase::Night]);

	renderTerrain(window, 64, 192, 0.5, 0.25);
	renderTerrain(window, 64, 192, 1, 0.5);
	renderTerrain(window, 64, 192, 1.5, 0.75);
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

void Skybox::renderTerrain(sf::RenderTarget& window, int imgTop, int imgHeight, double scale, double zIndex) const{
	// Draw the terain
	sf::Sprite sprite;
	sprite.setTexture(*mTexture);

	sf::Vector2u windowSize = window.getSize();

	sf::Vector2f worldPos = window.mapPixelToCoords(sf::Vector2i(0,static_cast<int>((windowSize.y / 2))));

	sprite.setPosition(worldPos);
	sprite.setScale(sf::Vector2f(scale, scale));

	int skyLeft = static_cast<int>(window.getView().getCenter().x * zIndex);

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
	sprite.setOrigin(32,32);
	sprite.setRotation(mTicks * 0.1f);
	float scale = 0.5f;
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

