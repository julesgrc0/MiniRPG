#include <iostream>
#include <SFML/Graphics.hpp>
#include "MainGame.h"

int main(int argc, const char *argv[])
{
	MainGame mainGame = MainGame();

	if (!mainGame.Init(argv[0]))
	{
		return 1;
	}
	mainGame.Run();

	return 0;
}