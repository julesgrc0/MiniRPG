#pragma once
#include<SFML/Graphics.hpp>
#include "MapReader.h"

class Player
{
public:
	Player();
	~Player();
	bool KeyBoardUpdate(float deltatime);
	bool MapUpdate(float deltatime, MapReader& reader, Chunk*& activeChunk);
	bool isNewChunk();
	void addVisitedChunk(sf::Vector2f);
	sf::Vector2f playerPos;
private:
	bool isNew = false;
	int playerSeep = 10;
	std::vector<sf::Vector2f> visitedChunks;
};

