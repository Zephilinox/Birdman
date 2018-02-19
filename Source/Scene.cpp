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
			//TODO- search predicates (find if)
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
	textureStrings[0] = "../../Resources/Textures/1.png";
	textureStrings[1] = "../../Resources/Textures/2.png";
	textureStrings[2] = "../../Resources/Textures/3.png";
	textureStrings[3] = "../../Resources/Textures/4.png";
	textureStrings[4] = "../../Resources/Textures/5.png";
	textureStrings[5] = "../../Resources/Textures/6.png";
	textureStrings[6] = "../../Resources/Textures/7.png";
	textureStrings[7] = "../../Resources/Textures/8.png";
	textureStrings[8] = "../../Resources/Textures/9.png";
	textureStrings[9] = "../../Resources/Textures/10.png";
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

void Scene::setSceneID(int id)
{
	scene_id = id;
}

void Scene::update()
{
	//TODO - cycle through characters in scene, update them.
	for(Character &x : character_pool)
	{
		x.update();
	}
}

void Scene::render()
{
	//TODO render all ACTIVE props and characters
	for(Prop &current_prop : props_pool)
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
