#include "Play.hpp"
#include "Architecture\GameData.hpp"

//SELF
#include "Scene.hpp"



Play::Play(GameData* data): audience(game_data)
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

	//Set values and links of each scene
	scene1.name = "scene1";
	scene1.scene_description = "The Theater is full, Riggan's attempt to ";
	scene1.initSceneProps(Play::KITCHEN);
	scene1.initSceneCharacter(Play::SceneCharacters::RIGGAN);
	//TODO - Remove this
	scene1.character_pool.at(0).setPosition(100.0f, 100.0f);

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

void Play::update(float dt)
{
	scenes[current_scene].update(dt);
}

void Play::render() const
{
	//Render stage background
	game_data->getRenderer()->renderSprite(*stage);

	scenes[current_scene].render();
	//Render  VisualDialogue*
}
