#pragma once
#include "Case.h"
#include <SFML/Graphics.hpp>

class Chunk
{
public:
	Chunk();
	~Chunk();
	std::vector<Case *> chunk;
	sf::Vector2f position;
	int  size = 50;
	void Draw(sf::RenderWindow &);
	void Draw(sf::RenderTexture &);
};
