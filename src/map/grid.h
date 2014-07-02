#pragma once
#include <vector>
#include "../util/assert.h"

template <typename T>
class Grid {
public:
	constexpr static int mNumLayers = 3;
private:
	constexpr static int mChunkWidth = 64;
	constexpr static int mChunkHeight = 64;

	/// A piece of the map for faster access.
	struct Chunk {
		T cells[mChunkWidth*mChunkHeight*mNumLayers];

		Chunk(){
			for(auto& cel : cells){
				cel = -1;
			}
		}

		T& at(int x, int y, int layer){
			return cells[layer + mNumLayers * (y + mChunkHeight * x)];
		}

		const T& at(int x, int y, int layer) const{
			return cells[layer + mNumLayers * (y + mChunkHeight * x)];
		}
	};

	int mWidth;
	int mHeight;

	std::vector<Chunk> chunks;
public:
	int getWidth() const {return mWidth;}
	int getHeight() const {return mHeight;}

	Grid(int width, int height){
		Assert<Precondition>(width > 0 && height > 0 && width % mChunkWidth == 0 && height % mChunkHeight == 0);
		mWidth = width;
		mHeight = height;

		for(int c = 0; c < (width / mChunkWidth) * (height / mChunkHeight); c++)
			chunks.push_back(Chunk());
	}

	constexpr bool inArea(int x, int y, int layer){
		return x >= 0 && y >= 0 && layer >= 0 && x < mWidth && y < mHeight && layer < mNumLayers;
	}

	T& at(int x, int y, int layer){
		Assert<Precondition>(inArea(x,y,layer));

		int chunkId = x / mChunkWidth + (mWidth / mChunkWidth) * (y / mChunkHeight);
		return chunks[chunkId].at(x % mChunkWidth, y % mChunkHeight, layer);
	}

	const T& at(int x, int y, int layer) const{
		Assert<Precondition>(inArea(x,y,layer));

		int chunkId = (x / mChunkWidth) + (mWidth / mChunkWidth) * (y / mChunkHeight);
		return chunks[chunkId].at(x % mChunkWidth, y % mChunkHeight, layer);
	}
};
