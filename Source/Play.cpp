#include "Play.hpp"

//SELF
#include "Architecture\GameData.hpp"
#include "Scene.hpp"
#include "States/FadeOutState.hpp"
#include "States/GameState.hpp"
#include "States/PlayEndState.h"
#include "Messages/AudioChangeMessage.hpp"

#include "Constants.hpp"

Play::Play(GameData* data)
	: audience(data)
{
	scenes.reserve(number_of_scenes);
	game_data = data;

	stage = data->getRenderer()->createUniqueSprite();
	stage->loadTexture("../../Resources/Textures/stage.png");
	stage->width((float)data->getWindowWidth());
	stage->height((float)data->getWindowHeight() * 0.65f);
	stage->xPos(0.0f);
	stage->yPos(0.0f);

	top_left_panel = std::move(game_data->getRenderer()->createUniqueSprite());
	top_left_panel->loadTexture("../../Resources/Textures/UI/TopLeftPanel.png");

	underline = std::move(game_data->getRenderer()->createUniqueSprite());
	underline->loadTexture("../../Resources/Textures/UI/SpeakerUnderline.png");
	underline->width(210);
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
	scene9.scene_id = 8;
	Scene& scene10 = scenes[9];

	//Set values and links of each scene
	scene1.name = "Kitchen";
	scene1.scene_description = "*The Theater is full. The curtains open...*";

	//Set props to scene type
	scene1.initSceneProps(Play::KITCHEN);

	//Set up the characters for the scene
	scene1.initSceneCharacter(Play::SceneCharacters::RIGGAN);
	scene1.character_pool.at(0)->setIsActive(false);
	scene1.initSceneCharacter(Play::SceneCharacters::MIKE);
	scene1.initSceneCharacter(Play::SceneCharacters::LESLIE);
	scene1.initSceneCharacter(Play::SceneCharacters::LAURA);

	//Set each character's position
	scene1.character_pool.at(RIGGAN)->setPosition(100.0f, 100.0f);
	scene1.character_pool.at(RIGGAN)->setFacing(Character::CharacterFacing::SOUTH);

	scene1.character_pool.at(MIKE)->setPosition(550.0f, 320.0f);
	scene1.character_pool.at(MIKE)->setFacing(Character::CharacterFacing::EAST);

	scene1.character_pool.at(LESLIE)->setPosition(614.0f, 270.0f);
	scene1.character_pool.at(LESLIE)->setFacing(Character::CharacterFacing::SOUTH);

	scene1.character_pool.at(LAURA)->setPosition(680.0f, 325.0f);
	scene1.character_pool.at(LAURA)->setFacing(Character::CharacterFacing::WEST);


	scene1.comedy.stage_description = "comedy path 1";
	scene1.comedy.scene = &scene2;
	scene1.light.stage_description = "light path 1";
	scene1.light.scene = &scene2;

	scene1.sad.stage_description = "sad path 1";
	scene1.sad.scene = &scene2;

	scene1.dark.stage_description = "dark path 1";
	scene1.dark.scene = &scene2;

	scene2.name = "Apartment";
	scene2.scene_description = "*Flashback to a dimly lit apartment*";
	//Set props to scene type
	scene2.initSceneProps(Play::APARTMENT_BEDROOM);

	//Set up the characters for the scene
	scene2.initSceneCharacter(Play::SceneCharacters::RIGGAN);
	scene2.initSceneCharacter(Play::SceneCharacters::MIKE);
	scene2.initSceneCharacter(Play::SceneCharacters::LESLIE);

	scene2.character_pool.at(RIGGAN)->setIsActive(false);
	scene2.character_pool.at(RIGGAN)->setPosition(170, 280);
	scene2.character_pool.at(RIGGAN)->setFacing(Character::CharacterFacing::EAST);

	scene2.character_pool.at(MIKE)->setPosition(580.0f, 240.0f);
	scene2.character_pool.at(MIKE)->setFacing(Character::CharacterFacing::EAST);

	scene2.character_pool.at(LESLIE)->setPosition(650.0f, 240.0f);
	scene2.character_pool.at(LESLIE)->setFacing(Character::CharacterFacing::WEST);

	//So AnimatedSprites get drawn while the curtain is closed
	scene3.name = "Apartment";
	scene3.scene_description = "*Enter apartment description here*";
	scene4.name = "Apartment";
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
	game_data->getRenderer()->renderSprite(*stage, Z_ORDER_LAYER::STAGE);
	//render scene
	scenes[current_scene].render();
	//render audience
	audience.render();

	game_data->getRenderer()->renderSprite(*top_left_panel, Z_ORDER_LAYER::PANELS);

	switch (night)
	{
	case 1:
		game_data->getRenderer()->renderText("First Preview Night", 20, 37, 1.0f, dark, Z_ORDER_LAYER::PANELS_TEXT);
		break;
	case 2:
		game_data->getRenderer()->renderText("Second Preview Night", 20, 37, 1.0f, dark, Z_ORDER_LAYER::PANELS_TEXT);
		break;
	case 3:
		game_data->getRenderer()->renderText("Opening Night", 20, 37, 1.0f, dark, Z_ORDER_LAYER::PANELS_TEXT);
		break;
	default:
		std::cout << "ERROR: NIGHTS IS UNDER 1 OR ABOVE 3\n";
	}
	
	game_data->getRenderer()->renderText(scenes[current_scene].name, 20, 75, 1.0f, dark, Z_ORDER_LAYER::PANELS_TEXT);

	underline->xPos(20);
	underline->yPos(45);
	game_data->getRenderer()->renderSprite(*underline, Z_ORDER_LAYER::PANELS_TEXT);

	underline->yPos(83);
	game_data->getRenderer()->renderSprite(*underline, Z_ORDER_LAYER::PANELS_TEXT);
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
		game_data->getAudioManager()->reset();
		game_data->getStateManager()->push<FadeOutState>(
		[&]()
		{
			night++;
			audience.varyApprovalsBetweenNights();
			current_scene = 0;
			next_scene = 0;
			game_data->getAudioManager()->play("FF7.wav", true);
			game_data->getMessageQueue()->sendMessage<AudioChangeMessage>("FF7.wav");

			//reset scenes
			scenes.clear();
			create();
		});
	}
	else
	{
		//TODO - play finished, output approval to screen?
		game_data->getStateManager()->push<PlayEndState>(audience.getOverallApproval());
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