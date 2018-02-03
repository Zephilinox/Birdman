#pragma once

//SELF
#include "../Architecture/UI/Menu.hpp"

class GameData;
class DialogueTree;

class VisualDialogue
{
public:
	VisualDialogue(GameData* game_data, DialogueTree* dialogue_tree, std::string starting_dialogue);
	
	void interact();

	void update();
	void render() const;

private:
	GameData* game_data;
	DialogueTree* dialogue_tree;

	std::string starting_dialogue;
	Menu options;
	std::string dialogue_text;
	int selected_option = -1;

	bool has_set_player_options = false;
};