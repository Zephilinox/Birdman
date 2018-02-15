#pragma once
#include <vector>
#include <Engine\Renderer.h>
#include "Character.h"

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
	void update();
	void render(ASGE::Renderer* renderer);

private:
	std::string name;
	std::string scene_description;

	GameData* game_data;

	//TODO - object pool of props
	std::vector<int> props_pool;
	std::vector<Character> character_pool;

	SceneLink light;
	SceneLink dark;
	SceneLink serious;
	SceneLink slapstick;
};