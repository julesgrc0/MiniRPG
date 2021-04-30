#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "MainGame.h"


int main(int argc, const char *argv[])
{
	MainGame mainGame = MainGame();
	mainGame.Run();

	// _CrtDumpMemoryLeaks();
	return 0;
}