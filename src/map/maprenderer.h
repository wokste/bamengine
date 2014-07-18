#pragma once
#include <memory>
class Map;
namespace sf{
	class RenderTarget;
}

class IMapRenderer {
public:
	static std::unique_ptr<IMapRenderer> factory();

	virtual void render(const Map& map, sf::RenderTarget& renderTarget) = 0;
};
