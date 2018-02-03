#pragma once

//SELF
#include "../Architecture/States/BaseState.hpp"
#include "../Architecture/UI/Menu.hpp"
#include "../Architecture/Dialogues/DialogueTree.hpp"

class GameData;

/**
*  See BaseState for overriden functions
*/
class GameState : public BaseState
{
public:
	GameState(GameData* game_data);

	void update(const ASGE::GameTime&) override final;
	void render() const override final;
	void onActive() override final;
	void onInactive() override final;

private:
	void dialogue_init();

	Menu dialogue_menu;
	DialogueTree dialogues;
	std::string dialogue_text;
	int selected_option = -1;
};
