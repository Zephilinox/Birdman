//LIB
#include <Engine/Platform.h>

//SELF
#include "BirdmanTheGame.hpp"

#define ENABLE_CONSOLE true

#if ENABLE_CONSOLE
#pragma comment(linker, "/SUBSYSTEM:console /ENTRY:mainCRTStartup")
int main()
#else
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:WinMainCRTStartup")
int WINAPI WinMain(
	HINSTANCE hInstance, HINSTANCE hPrevInstance,
	PSTR pScmdline, int iCmdshow)
#endif
{
	BirdmanTheGame game;
	if (game.init())
	{
		return game.run();
	}
	else //game failed to initialise
	{
		return -1;
	}
}