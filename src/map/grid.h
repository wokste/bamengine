#pragma once
class Block;
#include <vector>

struct Precondition{};
struct ComputationError{};

template <typename X, typename A>
inline void Assert(A assertion)
{
    if( !assertion ) throw X();
}

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

	int width;
	int height;

	std::vector<Chunk> chunks;
public:
	int getWidth() const {return width;}
	int getHeight() const {return height;}

	Grid(int width, int height){
		Assert<Precondition>(width > 0 && height > 0 && width % mChunkWidth == 0 && height % mChunkHeight == 0);
		this->width = width;
		this->height = height;

		for(int c = 0; c < (width / mChunkWidth) * (height / mChunkHeight); c++)
			chunks.push_back(Chunk());
	}

	constexpr bool inArea(int x, int y, int layer){
		return x >= 0 && y >= 0 && layer >= 0 && x < width && y < height && layer < mNumLayers;
	}

	T& at(int x, int y, int layer){
		Assert<Precondition>(inArea(x,y,layer));

		int chunkId = x / mChunkWidth + (width / mChunkWidth) * (y / mChunkHeight);
		return chunks[chunkId].at(x % mChunkWidth, y % mChunkHeight, layer);
	}

	const T& at(int x, int y, int layer) const{
		Assert<Precondition>(inArea(x,y,layer));

		int chunkId = (x / mChunkWidth) + (width / mChunkWidth) * (y / mChunkHeight);
		Assert<ComputationError>(chunkId >= 0 && chunkId < chunks.size());
		return chunks[chunkId].at(x % mChunkWidth, y % mChunkHeight, layer);
	}
};
