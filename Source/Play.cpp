#include "Play.hpp"
#include "Architecture\GameData.hpp"

//SELF
#include "Scene.hpp"

Play::Play(GameData* data)
{
	scenes.reserve(number_of_scenes);
	game_data = data;
}

Play::~Play()
{
}

void Play::create()
{
	//TODO implement scene linked list creation here
	//Call functions here based on the play to instanciate
	for (int i = 0; i < number_of_scenes; i++)
	{
		Scene s(game_data);
		scenes.push_back(std::move(s));
	}

	//TODO replace with non shit code
	Scene& scene1 = scenes[0];
	scene1.setSceneID(0);
	Scene& scene2 = scenes[1];
	scene2.setSceneID(1);
	Scene& scene3 = scenes[2];
	scene3.setSceneID(2);
	Scene& scene4 = scenes[3];
	scene4.setSceneID(3);
	Scene& scene5 = scenes[4];
	scene5.setSceneID(4);
	Scene& scene6 = scenes[5];
	scene6.setSceneID(5);
	Scene& scene7 = scenes[6];
	scene7.setSceneID(6);
	Scene& scene8 = scenes[7];
	scene8.setSceneID(7);
	Scene& scene9 = scenes[8];
	scene9.setSceneID(8);
	Scene& scene10 = scenes[9];
	scene10.setSceneID(9);

	//Set values and links of each scene
	scene1.name = "scene1";
	scene1.scene_description = "The Theater is full, Riggan's attempt to ";
	scene1.initSceneProps(Play::KITCHEN);
	scene1.initSceneCharacter(Play::SceneCharacters::RIGGAN);
	scene1.initSceneCharacter(Play::SceneCharacters::JOHN);

	scene1.dark.stage_description = "The play is going down a dark path...";
	scene1.dark.scene = &scene2;

	scene1.light.stage_description = "The play is going down a light hearted route...";
	scene1.light.scene = &scene3;

	scene2.name = "scene2";
	scene2.scene_description = "A dark apartment bedroom, two bodies move under the sheets...";

	//TODO implement next scenes
	scene2.initSceneProps(Play::APARTMENT_BEDROOM);
	scene2.initSceneCharacter(Play::SceneCharacters::RIGGAN);
	scene2.initSceneCharacter(Play::SceneCharacters::JOHN);
}

void Play::update()
{
	scenes[current_scene].update();
}

void Play::render()
{
	scenes[current_scene].render();
	//Render  VisualDialogue*
}

void Play::moveToNextScene()
{
	//TODO - redo this, it's ugly as shitfuck
	if (scenes[current_scene].light_value > scenes[current_scene].dark_value &&
		scenes[current_scene].light_value > scenes[current_scene].serious_value &&
		scenes[current_scene].light_value > scenes[current_scene].slapstick_value)
	{
		current_scene = scenes[current_scene].light.scene->scene_id;
	}
	else if (scenes[current_scene].dark_value > scenes[current_scene].serious_value&&
		scenes[current_scene].dark_value > scenes[current_scene].slapstick_value)
	{
		current_scene = scenes[current_scene].dark.scene->scene_id;
	}
	else if (scenes[current_scene].serious_value > scenes[current_scene].slapstick_value)
	{
		current_scene = scenes[current_scene].serious.scene->scene_id;
	}
	else
	{
		current_scene = scenes[current_scene].slapstick.scene->scene_id;
		std::cout << "Make this better!";
	}
}

void Play::moveToNextNight()
{
	if (night < 3)
	{
		night++;
		audience.varyApprovalsBetweenNights();
	}
	else
	{
		//TODO - play finished, next play pls
	}

}
