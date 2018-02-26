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
	void loadPropTextureStrings();
	void populateProps();
	void setSceneID(int id);

	void update(float dt);
	void render() const;

private:
	std::string name;
	std::string scene_description;

	std::string propTextureStrings[number_of_props];


	GameData* game_data;

	//TODO - object pool of props
	std::vector<Prop> props_pool;
	std::vector<Character> character_pool;

	int light_value = 0;
	int dark_value = 0;
	int serious_value = 0;
	int slapstick_value = 0;

	int scene_id = 0;

	SceneLink light;
	SceneLink dark;
	SceneLink serious;
	SceneLink slapstick;
};