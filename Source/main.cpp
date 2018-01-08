#include "BirdmanTheGame.h"

int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance, 
	PSTR pScmdline, int iCmdshow)
{
	BirdmanTheGame game;
	if (game.init())
	{
		return game.run();
	}

	// game failed to initialise
	else
	{
		return -1;
	}
}