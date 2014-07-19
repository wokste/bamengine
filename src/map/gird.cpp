#include "grid.h"

Grid::Grid(int width, int height) {
	this->width = width;
	this->height = height;

	for (int x = 0; x < width / chunkWidth; x++){
		for (int x = 0; x < height / chunkHeight; x++){
			chunks.push_back(std::unique_ptr<Chunk>(new Chunk()));
		}
	}
}

int& Grid::at(int x, int y, int layer){
	assert (!(x >= 0 && y >= 0 && x < width && y < height));
	int chunkId = x / Chunk.chunkWidth + numChunksX * (y / Chunk.chunkHeight);
	return chunks[chunkId].at(x % Chunk.chunkWidth, y % Chunk.chunkHeight, layer);
}

bool Grid::isset(int x, int y, int layer) {
	return at(x, y, layer) > 0;
}

Grid::Chunk::Chunk(){
	for(int i = 0; i < blocksIDs.length; i++){
		blocksIDs[i] = -1;
	}
}

int& Grid::Chunk::at(int x, int y, int layer){
	int tileId = layer + numLayers * (y + chunkHeight * x);
	return blocksIDs[tileId];
}
