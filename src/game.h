#pragma once
#include <memory>

namespace sf {class RenderTarget;}

class IGame{
public:
	static std::unique_ptr<IGame> factory();
	virtual void display(sf::RenderTarget&, float interpolation) = 0;
	virtual void logic() = 0;
};
