#pragma once
#include "Case.h"

class Chunk
{
public:
	Chunk();
	~Chunk();
	std::vector<Case*> chunk;
	sf::Vector2f position;
};

