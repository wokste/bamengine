#pragma once
#include <memory>
#include <string>

namespace sf {class RenderTarget;}

class IGame{
public:
	static std::unique_ptr<IGame> factory();
	virtual ~IGame(){}

	virtual void display(sf::RenderTarget&, float interpolation) = 0;
	virtual void logic(bool hasFocus) = 0;

	virtual void load(const std::string& mapname) = 0;
	virtual void save(const std::string& mapname) = 0;

	virtual void generate(const std::string& biome, int seed) = 0;
};
