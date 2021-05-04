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
	bool isNight = true;

	const float chunkAnimationDuration = 20.0f;
	const int gameTimeSwitch = 60 * 2;
	
	void DrawNightFilter(sf::RenderTexture&,sf::Vector2u, int gameTime);
	int getChunkId(std::vector<Chunk*>& map, Chunk*& chunk);
};
