#pragma once
#include <memory>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>

class Skybox{
	enum Phase{
		Day,
		Night,
		Max
	};
	std::shared_ptr<sf::Texture> mTexture;
	sf::Color mPhaseColor[Phase::Max];
	int mPhaseTime[Phase::Max];
	int mInterpolationTime;
	int mTicks;

public:
	Skybox();

	/// Renders the whole skybox
	void render(sf::RenderTarget& target) const;
	Phase getPhase() const;

	void tick();
private:
	void renderSky(sf::RenderTarget& window) const;
	void renderTerrain(sf::RenderTarget& window, int top, int height, double speed) const;
	void renderSun(sf::RenderTarget& window, const sf::IntRect& rect, int tickStart, int tickEnd) const;
};
