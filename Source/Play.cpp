#include "Play.hpp"

//SELF
#include "Scene.hpp"

Play::~Play()
{
	//it's better to avoid manual memory management, see if you can use std::vector<Scene>> instead.
	delete[] scenes;
	scenes = nullptr;
}

void Play::create()
{
	//TODO implement scene linked list creation here
	//Call functions here based on the play to instanciate

	scenes = new Scene[9];

	Scene& scene1 = scenes[0];
	Scene& scene2 = scenes[1];
	Scene& scene3 = scenes[2];
	Scene& scene4 = scenes[3];
	Scene& scene5 = scenes[4];
	Scene& scene6 = scenes[5];
	Scene& scene7 = scenes[6];
	Scene& scene8 = scenes[7];
	Scene& scene9 = scenes[8];
	Scene& scene10 = scenes[9];

	scene1.name = "scene1";
	scene1.stage_description = "A rundown stage.. poorly made props... Player actor is stood on the left, his head lowered...";
	scene1.dark.stage_description = "A room north of you";
	scene1.dark.scene = &scene2;
	scene1.light.stage_description = "blah hbalshflagf";
	scene1.light.scene = &scene3;



	scene2.name = "scene2";
	scene2.stage_description = "A lifeless room.. Except for the firey redhead in lingerie... your wife... in bed with another man!";
}

Scene* Play::getScene(int idx)
{
	return &scenes[idx];
}
