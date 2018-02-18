#pragma once
#include <vector>
#include <Engine\Renderer.h>
#include "Play.hpp"
#include "Character.h"
#include "Prop.h"

constexpr unsigned int number_of_props = 10;
constexpr unsigned int number_of_characters = 5;

class GameData;

class Scene
{
	friend class Play;

	struct SceneLink
	{
		Scene* scene = nullptr;
		std::string stage_description = "a dead end...";
	};

public:
	Scene(GameData* data);
	std::string getName() const;
	std::string getDescription() const;

	void initSceneProps(Play::SceneProps layout);
	void initSceneCharacter(Play::SceneCharacters chars);
	void loadPropTexStrings();
	void populateProps();

	void update();
	void render();

private:
	std::string name;
	std::string scene_description;

	std::string textureStrings[number_of_props];

	GameData* game_data;
	ASGE::Renderer* rend;

	//TODO - object pool of props
	std::vector<Prop> props_pool;
	std::vector<Character> character_pool;

	SceneLink light;
	SceneLink dark;
	SceneLink serious;
	SceneLink slapstick;
};