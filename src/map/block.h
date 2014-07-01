#pragma once

class Block{
public:
	int frameStart;
	int frameCount;
	bool solid = true;

	Block(int start, int count){
		frameStart = start;
		frameCount = count;
	}
};
