#pragma once
#include "Case.h"
#include <SFML/Graphics.hpp>
#include "GameTexture.h"

class Chunk
{
public:
	Chunk();
	~Chunk();
	std::vector<Case *> chunk;
	sf::Vector2f position;
	int  size = 50;
	void Draw(sf::RenderWindow&, GameTexture&);
	void Draw(sf::RenderTexture&, GameTexture&);
};
