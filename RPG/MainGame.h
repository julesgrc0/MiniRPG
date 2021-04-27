#pragma once
#include "Chunk.h"
#include "Player.h"
#include "Mouse.h"

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
};
