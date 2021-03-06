#pragma once
#include "Audience.hpp"
#include <vector>

class Scene;
class GameData;

constexpr unsigned int number_of_scenes = 10;

class Play
{
public:

	enum SceneProps
	{
		KITCHEN = 0,
		APARTMENT_BEDROOM,
		ALLEYWAY,
		LIVING_ROOM,
		STREET,
	};

	enum SceneCharacters
	{
		RIGGAN =0,
		MIKE,
		LESLIE,
		LAURA,
	};


	Play(GameData* data);
	~Play() = default;

	void create();
	void update(float dt);
	void render() const;
	void moveToNextScene();
	void moveToNextNight();
	
	Scene* getScene();
	Scene* getNextScene();
	Audience* getAudience();

private:
	std::vector<Scene> scenes;

	GameData* game_data;

	Audience audience;

	bool end_of_play = false;

	std::unique_ptr<ASGE::Sprite> stage;

	int current_scene = 0;
	int next_scene = 0;
	int night = 1;

	std::unique_ptr<ASGE::Sprite> top_left_panel;
	std::unique_ptr<ASGE::Sprite> underline;
};