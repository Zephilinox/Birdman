#pragma once
#include "Audience.hpp"

class Scene;

//if I've understood this right, the structure should look like this:
//	States
//		Game State
//			Play
//				Dialogue Tree (each play has it's own tree)
//				Scenes (each play has multiple scenes, dialogue system can use this to vary dialogue based on scene)
//				Visual Dialogue (should it be here or part of game state?, here might be easier)
//				Audience
//what is game states purpose aside from containing the play? audience reaction? controlling the play? selecting the play?

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