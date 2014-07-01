#include "maprenderer.h"
#include "block.h"
#include "map.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/OpenGL.hpp>

MapRenderer::MapRenderer(){
	tileSet.loadFromFile("data/tileset.png");
	framesPerRow = (int)(tileSet.getSize().x / outerTileSize);
}

void MapRenderer::render(const Map& map, sf::RenderTarget& renderTarget, int layer) {
	const sf::Vector2f screenCenter = renderTarget.getView().getCenter();
	const sf::Vector2f screenSize = renderTarget.getView().getSize();

	int minX = std::max(0, (int)(screenCenter.x - screenSize.x / 2) / innerTileSize);
	int maxX = std::min(map.grid.getWidth(), (int)(screenCenter.x + screenSize.x / 2) / innerTileSize + 1);
	int minY = std::max(0, (int)(screenCenter.y - screenSize.y / 2) / innerTileSize);
	int maxY = std::min(map.grid.getHeight(), (int)(screenCenter.y + screenSize.y / 2) / innerTileSize + 1);

	sf::Sprite sprite(tileSet);

	//glBegin(GL_QUADS);
	for(int x = minX; x < maxX; x++){
		for(int y = minY; y < maxY; y++){
			const Block* block = map.getBlock(x, y, layer);
			if (block != nullptr){
				updateSprite(sprite, chooseFrame(block, x, y), x, y,
					(map.getBlock(x - 1, y, layer) != block),
					(map.getBlock(x, y - 1, layer) != block),
					(map.getBlock(x + 1, y, layer) != block),
					(map.getBlock(x, y + 1, layer) != block));
				renderTarget.draw(sprite);
			}
		}
	}



	//glEnd();
}

void MapRenderer::updateSprite(sf::Sprite& sprite, int frame, float tileX, float tileY, bool borderLeft, bool borderTop, bool borderRight, bool borderBottom){
	int x=tileX * innerTileSize;
	int y=tileY * innerTileSize;

	sf::IntRect rect((frame % framesPerRow) * outerTileSize + border, (frame / framesPerRow) * outerTileSize + border, innerTileSize, innerTileSize);

	if (borderLeft){
		rect.left -= border;
		x -= border;
		rect.width += border;
	}
	if (borderRight){
		rect.width += border;
	}
	if (borderTop){
		rect.top -= border;
		y -= border;
		rect.height += border;
	}
	if (borderBottom){
		rect.height += border;
	}

	sprite.setTextureRect(rect);
	sprite.setPosition(x,y);
}

int MapRenderer::chooseFrame(const Block* type, int x, int y) const{
	int seed = x + y * 57;
	seed ^= seed<<13;
	  seed=(seed*(seed*60493+19990303)+1376312589)&0x7fffffff;

	return type->frameStart + ((seed >> 24) % type->frameCount);
}

