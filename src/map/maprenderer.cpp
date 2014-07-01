#include "maprenderer.h"
#include "block.h"
#include "map.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/OpenGL.hpp>

MapRenderer::MapRenderer(){
	mTileSet.loadFromFile("data/tileset.png");
	mFramesPerRow = (int)(mTileSet.getSize().x / mOuterTileSize);
}

void MapRenderer::render(const Map& map, sf::RenderTarget& renderTarget, int layer) {
	const sf::Vector2f screenCenter = renderTarget.getView().getCenter();
	const sf::Vector2f screenSize = renderTarget.getView().getSize();

	int minX = std::max(0, (int)(screenCenter.x - screenSize.x / 2) / mInnerTileSize);
	int maxX = std::min(map.getWidth(), (int)(screenCenter.x + screenSize.x / 2) / mInnerTileSize + 1);
	int minY = std::max(0, (int)(screenCenter.y - screenSize.y / 2) / mInnerTileSize);
	int maxY = std::min(map.getHeight(), (int)(screenCenter.y + screenSize.y / 2) / mInnerTileSize + 1);

	sf::Sprite sprite(mTileSet);

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
	int x=tileX * mInnerTileSize;
	int y=tileY * mInnerTileSize;

	sf::IntRect rect{
		(frame % mFramesPerRow) * mOuterTileSize + mBorder,
		(frame / mFramesPerRow) * mOuterTileSize + mBorder, mInnerTileSize, mInnerTileSize
	};

	if (borderLeft){
		rect.left -= mBorder;
		x -= mBorder;
		rect.width += mBorder;
	}
	if (borderRight){
		rect.width += mBorder;
	}
	if (borderTop){
		rect.top -= mBorder;
		y -= mBorder;
		rect.height += mBorder;
	}
	if (borderBottom){
		rect.height += mBorder;
	}

	sprite.setTextureRect(rect);
	sprite.setPosition(x,y);
}

int MapRenderer::chooseFrame(const Block* type, int x, int y) const{
	int seed = x + y * 57;
	seed ^= seed<<13;
	  seed=(seed*(seed*60493+19990303)+1376312589)&0x7fffffff;

	return type->mFrameStart + ((seed >> 24) % type->mFrameCount);
}

