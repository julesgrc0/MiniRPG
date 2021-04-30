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
	void Run();

private:

	void DrawText(sf::RenderWindow&, sf::String, sf::Vector2f);

	Player player;
	Mouse mouse;
	JsonMapReader reader;

	sf::Font font;
	std::string errorValue = "";
	
	bool HasError = false;
	bool isFontReady = false;
	
	
};
