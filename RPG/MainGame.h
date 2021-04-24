#pragma once
#include"Chunk.h"


class MainGame
{
public:
	MainGame();
	~MainGame();
	bool Init(const char*);
	void Run();
private:
	sf::Vector2f playerPos;
};

