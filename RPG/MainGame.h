#pragma once
#include "Chunk.h"
#include "Player.h"
#include "Mouse.h"
#include "JsonMapReader.h"

class MainGame
{
public:
	MainGame();
	~MainGame();
	bool Init(const char *);
	void Run();

private:
	Player player;
	Mouse mouse;
	sf::Font font;
	bool isFontReady = false;
	JsonMapReader reader;
	bool HasError = false;
	std::string errorValue = "";
};
