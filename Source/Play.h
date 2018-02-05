#pragma once
#include "Audience.h"

class Scene;

class Play
{
public:
	Play() = default;
	~Play();

	void create();
	Scene* getScene(int idx);

private:
	Scene* scenes = nullptr;

	Audience audience;

	//TODO see this
	//Chris - each night 1 = preview1, 2 = preview 2, 3 = opening night
	int night = 1;
};