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
	constexpr static int numLayers = 3;
private:
	constexpr static int chunkWidth = 64;
	constexpr static int chunkHeight = 64;

	/// A piece of the map for faster access.
	struct Chunk {
		T cells[chunkWidth*chunkHeight*numLayers];

		Chunk(){
			for(auto& cel : cells){
				cel = -1;
			}
		}

		T& at(int x, int y, int layer){
			return cells[layer + numLayers * (y + chunkHeight * x)];
		}

		const T& at(int x, int y, int layer) const{
			return cells[layer + numLayers * (y + chunkHeight * x)];
		}
	};

	int width;
	int height;

	std::vector<Chunk> chunks;
public:
	int getWidth() const {return width;}
	int getHeight() const {return height;}

	Grid(int width, int height){
		Assert<Precondition>(width > 0 && height > 0 && width % chunkWidth == 0 && height % chunkHeight == 0);
		this->width = width;
		this->height = height;

		for(int c = 0; c < (width / chunkWidth) * (height / chunkHeight); c++)
			chunks.push_back(Chunk());
	}

	constexpr bool inArea(int x, int y, int layer){
		return x >= 0 && y >= 0 && layer >= 0 && x < width && y < height && layer < numLayers;
	}

	T& at(int x, int y, int layer){
		Assert<Precondition>(inArea(x,y,layer));

		int chunkId = x / chunkWidth + (width / chunkWidth) * (y / chunkHeight);
		return chunks[chunkId].at(x % chunkWidth, y % chunkHeight, layer);
	}

	const T& at(int x, int y, int layer) const{
		Assert<Precondition>(inArea(x,y,layer));

		int chunkId = (x / chunkWidth) + (width / chunkWidth) * (y / chunkHeight);
		Assert<ComputationError>(chunkId >= 0 && chunkId < chunks.size());
		return chunks[chunkId].at(x % chunkWidth, y % chunkHeight, layer);
	}
};
