#include "maprenderer.h"
#include "block.h"
#include "map.h"
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/OpenGL.hpp>

class MapRenderer : public IMapRenderer {
	constexpr static int mTileSize = 12;
	int mFramesPerRow;
	sf::Texture mTileSet;

public:

	MapRenderer(){
		mTileSet.loadFromFile("data/tileset2.png");
		mFramesPerRow = (int)(mTileSet.getSize().x / mTileSize);
	}

	void render(const Map& map, sf::RenderTarget& renderTarget) override{
		const sf::Vector2f screenCenter = renderTarget.getView().getCenter();
		const sf::Vector2f screenSize = renderTarget.getView().getSize();

		int minX = std::max(0, (int)(screenCenter.x - screenSize.x / 2) / mTileSize);
		int maxX = std::min(map.getWidth(), (int)(screenCenter.x + screenSize.x / 2) / mTileSize + 1);
		int minY = std::max(0, (int)(screenCenter.y - screenSize.y / 2) / mTileSize);
		int maxY = std::min(map.getHeight(), (int)(screenCenter.y + screenSize.y / 2) / mTileSize + 1);

		sf::Sprite sprite(mTileSet);

		for (int layer = 0; layer < 3; layer++){
			if (layer == 0)
				sprite.setColor(sf::Color(192,192,192));
			else
				sprite.setColor(sf::Color(255,255,255));

			for(int x = minX; x < maxX; x++){
				for(int y = minY; y < maxY; y++){
					// Don't draw blocks that are obscured by other blocks.
					if (layer < 1){
						const Block* frontBlock = map.getBlock(x,y,1);
						if (frontBlock != nullptr && frontBlock->mGraphics == Block::Graphics::Solid)
							continue;
					}

					// Draw block
					renderBlock(renderTarget, sprite, map, x, y, layer);
				}
			}
		}
	}

	void renderTile(sf::RenderTarget& renderTarget, sf::Sprite& sprite, int baseFrame, int frameAdds[], int tileX, int tileY){
		int x=tileX * mTileSize;
		int y=tileY * mTileSize;

		for (int i = 0; i < 4; i++){
			int dx = mTileSize / 2 * ((i & 1) != 0);
			int dy = mTileSize / 2 * ((i & 2) != 0);

			int frame = baseFrame + frameAdds[i];
			sf::IntRect rect{
				(frame % mFramesPerRow) * mTileSize + dx,
				(frame / mFramesPerRow) * mTileSize + dy, 6, 6
			};

			sprite.setTextureRect(rect);
			sprite.setPosition(x + dx,y + dy);
			renderTarget.draw(sprite);
		}
	}

	void renderBlock(sf::RenderTarget& renderTarget, sf::Sprite& sprite, const Map& map, int x, int y, int layer){
		const Block* block = map.getBlock(x, y, layer);
		if (block == nullptr)
			return;

		int baseframe = block->mFrameStart;
		int frameAdds[] = {0,0,0,0};

		if (map.getBlock(x - 1, y, layer) != block){
			frameAdds[0] |= 1;
			frameAdds[2] |= 1;
		}

		if (map.getBlock(x + 1, y, layer) != block){
			frameAdds[1] |= 1;
			frameAdds[3] |= 1;
		}

		if (map.getBlock(x, y - 1, layer) != block){
			frameAdds[0] |= 2;
			frameAdds[1] |= 2;
		}

		if (map.getBlock(x, y + 1, layer) != block){
			frameAdds[2] |= 2;
			frameAdds[3] |= 2;
		}

		renderTile(renderTarget, sprite, baseframe, frameAdds, x, y);
	}
};

std::unique_ptr<IMapRenderer> IMapRenderer::factory(){
	return std::unique_ptr<IMapRenderer>(new MapRenderer());
}

