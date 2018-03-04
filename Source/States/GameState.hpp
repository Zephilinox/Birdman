#pragma once

//SELF
#include "../Architecture/States/BaseState.hpp"
#include "../Architecture/UI/Menu.hpp"
#include "../Architecture/Dialogues/DialogueTree.hpp"
#include "../Dialogue/VisualDialogue.hpp"
#include "../Play.hpp"

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
	void dialogue_kitchen();
	//void dialogue_init();

	DialogueTree dialogue_tree;
	VisualDialogue visual_dialogue;
	Play play_01;

	std::unique_ptr<ASGE::Sprite> bottom_panel;
	std::unique_ptr<ASGE::Sprite> top_panel;

	ManagedConnection managed_con;
	std::string current_music_path = "FF7.wav";
};
