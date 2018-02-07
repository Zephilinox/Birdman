#pragma once

//SELF
#include "../Architecture/States/BaseState.hpp"
#include "../Architecture/UI/Menu.hpp"
#include "../Architecture/Dialogues/DialogueTree.hpp"
#include "../Dialogue/VisualDialogue.hpp"

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
	void dialogue_init2();

	DialogueTree dialogue_tree;
	VisualDialogue visual_dialogue;
};
