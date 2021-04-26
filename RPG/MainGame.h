#pragma once
#include"Chunk.h"
#include "Player.h"

class MainGame
{
public:
	MainGame();
	~MainGame();
	bool Init(const char*);
	void Run();
private:
	Player player;
	sf::Font font;
	bool isFontReady = false;
};

