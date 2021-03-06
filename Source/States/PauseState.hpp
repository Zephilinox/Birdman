#pragma once

//STD
#include <string>

//SELF
#include "../Architecture/States/BaseState.hpp"
#include "../Architecture/UI/Menu.hpp"

class GameData;

/**
*  See BaseState for overriden functions
*/
class PauseState : public BaseState
{
public:
	PauseState(GameData* game_data, std::string current_music_path);

	void update(const ASGE::GameTime&) override final;
	void render() const override final;
	void onActive() override final;
	void onInactive() override final;

private:
	Menu menu;
	std::unique_ptr<ASGE::Sprite> dim_background;
	std::unique_ptr<ASGE::Sprite> pause_image;

	ManagedConnection managed_con;
	std::string current_music_path;
};
