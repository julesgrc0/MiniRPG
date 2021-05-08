#include <iostream>
#include <SFML/Graphics.hpp>
#include <Windows.h>
#include "MainGame.h"

int main(int argc, const char *argv[])
{
	char *mapName;
	if (argc > 1)
	{
		mapName = (char *)argv[1];
	}
	else
	{
		mapName = (char *)"map.json";
	}

	HWND console = GetForegroundWindow();
#ifndef _DEBUG
	ShowWindow(console, SW_HIDE);
#else
	ShowWindow(console, SW_SHOW);
#endif

	while (true)
	{
		MainGame mainGame = MainGame(mapName);
		if (!mainGame.Run())
		{
			break;
		}
	}

	// _CrtDumpMemoryLeaks();
	return 0;
}