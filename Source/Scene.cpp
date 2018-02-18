#include "Scene.hpp"
#include "Play.hpp"
#include "Prop.h"
#include "Character.h"
#include "Architecture\GameData.hpp"

Scene::Scene(GameData * data)
{
	game_data = data;
	props_pool.reserve(number_of_props);
	character_pool.reserve(number_of_characters);
	loadPropTexStrings();
	populateProps();
}

std::string Scene::getName() const
{
	return name;
}

std::string Scene::getDescription() const
{
	return scene_description;
}

void Scene::initSceneProps(Play::SceneProps layout)
{
	//TODO switch layout with several scene layouts
	//Initialise sprites into pool in each case

	switch(layout)
	{
		case Play::SceneProps::KITCHEN:
		{
			props_pool.at(0).setIsActive(false);
			break;
		}
		case Play::SceneProps::APARTMENT_BEDROOM:
		{
			break;
		}
		case Play::SceneProps::ALLEYWAY:
		{
			break;
		}
		case Play::SceneProps::LIVING_ROOM:
		{
			break;
		}
		case Play::SceneProps::STREET:
		{
			break;
		}
		default:
		{
			break;
		}
	}
}

void Scene::initSceneCharacter(Play::SceneCharacters chars)
{
	//TODO load character into the character pool based on scene location
	//for(int i = 0; i < number_of_characters; i++)
	//{
	//	character_pool.push_back(WHAT?);
	//}
}

void Scene::loadPropTexStrings()
{
	//TODO find a nice way of doing this if pos?
	textureStrings[0] = "../Resources/Textures/1.png";
	textureStrings[1] = "../Resources/Textures/7.png";
	textureStrings[2] = "c";
	textureStrings[3] = "d";
	textureStrings[4] = "e";
	textureStrings[5] = "f";
	textureStrings[6] = "g";
	textureStrings[7] = "h";
	textureStrings[8] = "i";
	textureStrings[9] = "j";
}

void Scene::populateProps()
{
	//TODO Ricardo! this correct?
	for(int i = 0; i < number_of_props; i++)
	{
		Prop* p = new Prop;
		p->initSprite(game_data->getRenderer(), textureStrings[i]);
		props_pool.push_back(*p);
	}
}

void Scene::update()
{
	//TODO - cycle through characters in scene, update them.
	//for(Character x : character_pool)
	//{
	//	x.update();
	//}
}

void Scene::render()
{
	//TODO render all ACTIVE props and characters
	for(Prop current_prop : props_pool)
	{
		if(current_prop.getIsActive())
		{
			current_prop.render(game_data->getRenderer());
		}
	}

	for(Character &current_character : character_pool)
	{
		if(current_character.getIsActive())
		{
			current_character.render(game_data->getRenderer());
		}
	}
}
