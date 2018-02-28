#include "Scene.hpp"
#include "Play.hpp"
#include "Prop.h"
#include "Character.h"
#include "Architecture\GameData.hpp"

Scene::Scene()
{
	props_pool.reserve(number_of_props);
	character_pool.reserve(number_of_characters);
	loadPropTextureStrings();
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
	//Depending on scene props required, enable them in the pool and set to their positions.
	switch(layout)
	{
		case Play::SceneProps::KITCHEN:
		{
			props_pool.at(0).setIsActive(true);
			props_pool.at(0).setPosition((GameData::getWindowWidth() * 0.5f) - (props_pool.at(0).getSprite()->width() * 0.5f), GameData::getWindowHeight() * 0.1f);
			props_pool.at(1).setIsActive(true);
			props_pool.at(1).setPosition((GameData::getWindowWidth() * 0.5f) - (props_pool.at(1).getSprite()->width() * 0.5f), GameData::getWindowHeight() * 0.35f);
			break;
		}
		case Play::SceneProps::APARTMENT_BEDROOM:
		{
			props_pool.at(2).setIsActive(true);
			props_pool.at(2).setPosition((GameData::getWindowWidth() * 0.5f) - (props_pool.at(2).getSprite()->width() * 0.5f), GameData::getWindowHeight() * 0.15f);
			props_pool.at(3).setIsActive(true);
			props_pool.at(3).setPosition((GameData::getWindowWidth() * 0.5f) - (props_pool.at(3).getSprite()->width() * 0.5f), GameData::getWindowHeight() * 0.25f);
			break;
		}
		case Play::SceneProps::ALLEYWAY:
		{
			props_pool.at(4).setIsActive(true);
			//props_pool.at(4).setPosition....
			props_pool.at(5).setIsActive(true);
			//props_pool.at(5).setPosition....
			break;
		}
		case Play::SceneProps::LIVING_ROOM:
		{
			props_pool.at(6).setIsActive(true);
			//props_pool.at(6).setPosition....
			props_pool.at(7).setIsActive(true);
			//props_pool.at(7).setPosition....
			break;
		}
		case Play::SceneProps::STREET:
		{
			props_pool.at(8).setIsActive(true);
			//props_pool.at(8).setPosition....
			props_pool.at(9).setIsActive(true);
			//props_pool.at(9).setPosition....
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
	std::unique_ptr<Character> R(new Character());
	R->initCharacter(chars);
	R->setIsActive(true);
	R->setFacing(Character::CharacterFacing::EAST);
	character_pool.push_back(std::move(R));

}

void Scene::loadPropTextureStrings()
{
	//TODO find a nice way of doing this if pos?
	propTextureStrings[0] = "../../Resources/Textures/1.png";
	propTextureStrings[1] = "../../Resources/Textures/2.png";
	propTextureStrings[2] = "../../Resources/Textures/3.png";
	propTextureStrings[3] = "../../Resources/Textures/4.png";
	propTextureStrings[4] = "../../Resources/Textures/5.png";
	propTextureStrings[5] = "../../Resources/Textures/6.png";
	propTextureStrings[6] = "../../Resources/Textures/7.png";
	propTextureStrings[7] = "../../Resources/Textures/8.png";
	propTextureStrings[8] = "../../Resources/Textures/9.png";
	propTextureStrings[9] = "../../Resources/Textures/10.png";
}



void Scene::populateProps()
{
	//TODO Ricardo! this correct?
	//ah.. I don't think so but I'm not sure, ask james?
	//I have no idea what happens when you push a dereferenced pointer to a vector of values, does it copy it?
	//if so, then that will cause a memory leak (x2, one for the prop you new'd, and one for the ASGE sprite the prop owns that never gets deleted)
	//If that is the case, then better off doing something like this:
	//Prop p;
	//p->initSprite(...)
	//props_pool.push_back(std::move(p))
	for(unsigned int i = 0; i < number_of_props; i++)
	{
		Prop* p = new Prop;
		p->initSprite(GameData::getRenderer(), propTextureStrings[i]);
		props_pool.push_back(std::move(*p));
	}
}

void Scene::setSceneID(int id)
{
	scene_id = id;
}

void Scene::update(float dt)
{
	for(auto& current_character : character_pool)
	{
		if(current_character->getIsActive())
		{
			current_character->update(dt);
		}
	}
}

void Scene::render() const
{
	//TODO render all ACTIVE props and characters
	for (const Prop& current_prop : props_pool)
	{
		if(current_prop.getIsActive())
		{
			current_prop.render(GameData::getRenderer());
		}
	}

	for (auto& current_character : character_pool)
	{
		if(current_character->getIsActive())
		{
			current_character->render();
		}
	}
}
