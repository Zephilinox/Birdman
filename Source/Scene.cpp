#include "Scene.hpp"

Scene::Scene(GameData * data)
{
	game_data = data;
}

std::string Scene::getName() const
{
	return name;
}

std::string Scene::getDescription() const
{
	return scene_description;
}

void Scene::update()
{
}

void Scene::render(ASGE::Renderer* renderer)
{
	//TODO render all props and actors

	//for(int p : props_pool)
	//{

	//}

	//for(Character x : character_pool)
	//{
	//	x.setFacing(Character::CharacterFacing::NORTH);
	//}
}
