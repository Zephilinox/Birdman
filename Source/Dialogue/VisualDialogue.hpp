#pragma once

//SELF
#include "../Architecture/UI/Menu.hpp"

class GameData;
class DialogueTree;

class VisualDialogue
{
public:
	VisualDialogue(GameData* game_data, DialogueTree* dialogue_tree, std::string default_dialogue);
	
	void setDefaultDialogue(std::string dialogue);

	void interact();

	void update();
	void updateTree();
	void render() const;

private:
	void setupPlayerOptions();

	GameData* game_data;
	DialogueTree* dialogue_tree;

	std::string default_dialogue;
	Menu options;
	std::string dialogue_text;
	int selected_option = -1;

	bool has_set_player_options = false;
};