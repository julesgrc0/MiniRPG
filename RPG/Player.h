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
	void Draw(sf::RenderWindow&);
	sf::Vector2f playerPos;
private:
	bool isNew = false;
	const int Max_player_speed = 6;
	int playerSeep = Max_player_speed;
	std::vector<sf::Vector2f> visitedChunks;
};

