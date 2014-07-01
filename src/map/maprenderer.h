#pragma once
#include <SFML/Graphics/Texture.hpp>
class Map;
class RenderTarget;
class Block;

namespace sf{ class Sprite;}

class MapRenderer {
	constexpr static int innerTileSize = 12;
	constexpr static int border = 2;
	constexpr static int outerTileSize = innerTileSize + 2 * border;
	int framesPerRow;
	sf::Texture tileSet;

public:
	MapRenderer();
	void render(const Map& map, sf::RenderTarget& renderTarget, int layer);

private:
	void updateSprite(sf::Sprite& sprite, int frame, float tileX, float tileY, bool borderLeft, bool borderTop, bool borderRight, bool borderBottom);
	int chooseFrame(const Block* type, int x, int y) const;
};
