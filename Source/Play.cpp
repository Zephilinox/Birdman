#include "Play.hpp"
#include "Architecture\GameData.hpp"
#include "States\FadeOutState.hpp"

//SELF
#include "Scene.hpp"



Play::Play(GameData* data): audience(data)
{
	scenes.reserve(number_of_scenes);
	game_data = data;

	stage = data->getRenderer()->createUniqueSprite();
	stage->loadTexture("../../Resources/Textures/stage.png");
	stage->width((float)data->getWindowWidth());
	stage->height((float)data->getWindowHeight() * 0.65f);
	stage->xPos(0.0f);
	stage->yPos(0.0f);
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

	Scene& scene1 = scenes[0];
	scene1.scene_id = 0;
	Scene& scene2 = scenes[1];
	scene2.scene_id = 1;
	Scene& scene3 = scenes[2];
	scene3.scene_id = 2;
	Scene& scene4 = scenes[3];
	scene4.scene_id = 3;
	Scene& scene5 = scenes[4];
	scene5.scene_id = 4;
	Scene& scene6 = scenes[5];
	scene6.scene_id = 5;
	Scene& scene7 = scenes[6];
	scene7.scene_id = 6;
	Scene& scene8 = scenes[7];
	scene8.scene_id = 7;
	Scene& scene9 = scenes[8];
	Scene& scene10 = scenes[9];

	//Set values and links of each scene
	scene1.name = "scene1";
	scene1.scene_description = "*The Theater is full. The curtains open...*";

	//Set props to scene type
	scene1.initSceneProps(Play::KITCHEN);

	//Set up the characters for the scene
	scene1.initSceneCharacter(Play::SceneCharacters::RIGGAN);
	scene1.initSceneCharacter(Play::SceneCharacters::MIKE);
	scene1.initSceneCharacter(Play::SceneCharacters::LESLIE);
	scene1.initSceneCharacter(Play::SceneCharacters::LAURA);

	//Set each character's position
	scene1.character_pool.at(0)->setPosition(100.0f, 100.0f);
	scene1.character_pool.at(0)->setFacing(Character::CharacterFacing::SOUTH);
	scene1.character_pool.at(1)->setPosition(300.0f, 100.0f);
	scene1.character_pool.at(1)->setFacing(Character::CharacterFacing::SOUTH);
	scene1.character_pool.at(2)->setPosition(400.0f, 200.0f);
	scene1.character_pool.at(2)->setFacing(Character::CharacterFacing::WEST);
	scene1.character_pool.at(3)->setPosition(200.0f, 300.0f);
	scene1.character_pool.at(3)->setFacing(Character::CharacterFacing::NORTH);


	scene1.character_pool.at(3)->slowMoveToPosition(1000.0f, 700.0f);

	scene1.comedy.stage_description = "comedy path 1";
	scene1.comedy.scene = &scene2;

	scene1.light.stage_description = "light path 1";
	scene1.light.scene = &scene3;

	scene1.sad.stage_description = "sad path 1";
	scene1.sad.scene = &scene4;

	scene1.dark.stage_description = "dark path 1";
	scene1.dark.scene = &scene5;

	scene2.name = "scene2";
	scene2.scene_description = "*Enter apartment description here*";
	//Set props to scene type
	scene2.initSceneProps(Play::APARTMENT_BEDROOM);

	//Set up the characters for the scene
	scene2.initSceneCharacter(Play::SceneCharacters::RIGGAN);

	//TODO implement next scenes
	//scene2.initSceneCharacter(Play::SceneCharacters::RIGGAN);
	//scene2.initSceneCharacter(Play::SceneCharacters::MIKE);

	//So AnimatedSprites get drawn while the curtain is closed
	scene3.scene_description = "*Enter apartment description here*";
	scene4.scene_description = "*Enter apartment description here*";
	update(1.0f / 60.0f);
}

void Play::update(float dt)
{
	scenes[current_scene].update(dt);
	audience.update(dt);
}

void Play::render() const
{
	//Render stage background
	game_data->getRenderer()->renderSprite(*stage);
	scenes[current_scene].render();
	audience.render();
	//Render  VisualDialogue*
}

void Play::moveToNextScene()
{
	//Determine the highest value and set the current scene as the one it points to
	if (scenes[current_scene].light_value > scenes[current_scene].dark_value &&
		scenes[current_scene].light_value > scenes[current_scene].sad_value &&
		scenes[current_scene].light_value > scenes[current_scene].comedy_value)
	{
		next_scene = scenes[current_scene].light.scene->scene_id;
	}
	else if (scenes[current_scene].dark_value > scenes[current_scene].sad_value&&
		scenes[current_scene].dark_value > scenes[current_scene].comedy_value)
	{
		next_scene = scenes[current_scene].dark.scene->scene_id;
	}
	else if (scenes[current_scene].sad_value > scenes[current_scene].comedy_value)
	{
		next_scene = scenes[current_scene].sad.scene->scene_id;
	}
	else
	{
		next_scene = scenes[current_scene].comedy.scene->scene_id;
	}

	game_data->getStateManager()->push<FadeOutState>(
	[&]()
	{
		current_scene = next_scene;
	});
}

void Play::moveToNextNight()
{
	if (night < 3)
	{
		night++;
		audience.varyApprovalsBetweenNights();
		current_scene = 0;
	}
	else
	{
		//TODO - play finished, output approval to screen?
	}

}

Scene* Play::getScene()
{
	return &scenes[current_scene];
}

Scene* Play::getNextScene()
{
	return &scenes[next_scene];
}

Audience* Play::getAudience()
{
	return &audience;
}