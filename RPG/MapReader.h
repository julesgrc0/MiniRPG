#pragma once
#include <iostream>
#include "Chunk.h"

class MapReader
{
public:
	MapReader();
	~MapReader();

	std::vector<Chunk *> getFullMap(const char *);
	std::vector<Case *> getMapChunk(const char *, sf::Vector2f);
	int size = 50;
private:
	std::vector<Chunk *> map_chunks;
};
