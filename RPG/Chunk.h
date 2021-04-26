#pragma once
#include "Case.h"
#include <SFML/Graphics.hpp>

class Chunk
{
public:
	Chunk();
	~Chunk();
	std::vector<Case*> chunk;
	sf::Vector2f position;
	void Draw(sf::RenderWindow&);
};

