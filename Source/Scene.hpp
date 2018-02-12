#pragma once
#include <vector>

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
	void render();

private:
	std::string name;
	std::string scene_description;

	GameData* game_data;

	//TODO - object pool of props
	//std::vector<Prop>;

	SceneLink light;
	SceneLink dark;
	SceneLink shocking;
	SceneLink sad;
};