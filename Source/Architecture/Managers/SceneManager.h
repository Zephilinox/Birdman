#pragma once
#include "..\\..\\Play.h"

class SceneManager
{
	SceneManager() = default;

public:
	int getCurrentScene() { return play_id; }
	void setCurrentScene(int new_play);

	//TODO used in testing, REFACTOR
	Play myPlay;

private:
	int play_id = 0;
};
