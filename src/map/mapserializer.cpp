#include "mapserializer.h"
#include <fstream>
#include <stdint.h>

namespace MapSerializer{
	std::unique_ptr<Map> readMap(const BlockList& blockList, std::ifstream& is);
	void writeMap(std::ofstream& os, const Map& map);

	template <typename T>
	T readInt(std::ifstream& is){
		char c0, c1;
		is.read (&c0,1);
		is.read (&c1,1);
		return (c0<<8) | (c1<<0);
	}

	template <typename T>
	void writeInt(std::ofstream& os, const T cel){
		char c0 = (char)(cel >> 8);
		char c1 = (char)(cel);
		os.write(&c0,1);
		os.write(&c1,1);
	}

	std::unique_ptr<Map> readMap(const BlockList& blockList, const std::string& filename){
		std::ifstream is(filename);
		Assert<FileException>(is.good(), filename);
		return readMap(blockList, is);
	}

	void writeMap(const std::string& filename, const Map& map){
		std::ofstream os(filename);
		Assert<FileException>(os.good(), filename);
		writeMap(os, map);
	}

	std::unique_ptr<Map> readMap(const BlockList& blockList, std::ifstream& is){
		int width = readInt<uint16_t>(is);
		int height = readInt<uint16_t>(is);
		std::unique_ptr<Map> map;
		map.reset(new Map(blockList, width, height));

		for(int l = 0; l < 3; l++){
			for(int y = 0; y < height; y++){
				for(int x = 0; x < width; x++){
					map->idAt(x,y,l) = readInt<BlockId>(is);
				}
			}
		}
		return std::move(map);
	}

	void writeMap(std::ofstream& os, const Map& map){
		int width = map.getWidth();
		int height = map.getHeight();
		writeInt(os, (uint16_t)width);
		writeInt(os, (uint16_t)height);

		for(int l = 0; l < 3; l++){
			for(int y = 0; y < height; y++){
				for(int x = 0; x < width; x++){
					writeInt(os, map.idAt(x,y,l));
				}
			}
		}
	}
}
