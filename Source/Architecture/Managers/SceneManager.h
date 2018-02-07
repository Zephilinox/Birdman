#pragma once
#include "..\\..\\Play.hpp"

class GameData;

class SceneManager
{
public:
	int getCurrentScene() { return play_id; }
	void setCurrentScene(int new_play);

	void render();

	//TODO used in testing, REFACTOR
	Play my_play;

private:
	int play_id = 0;
};
