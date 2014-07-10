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
};
