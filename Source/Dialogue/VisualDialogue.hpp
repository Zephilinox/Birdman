#pragma once

//SELF
#include "../Architecture/UI/Menu.hpp"
#include "../Architecture/Timer.hpp"

class GameData;
class DialogueTree;

enum DialogueSpeed
{
	VerySlow = 0,
	Slow = 1,
	Normal = 2,
	Fast = 3,
	VeryFast = 4,
	Instant = 5
};

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
	void setDialogueText(std::string text);
	void setDialogueSpeed(int speed);

	GameData* game_data;
	DialogueTree* dialogue_tree;

	std::string default_dialogue;
	Menu options;
	std::string dialogue_text;
	std::string speaker_name;
	ASGE::Sprite* portrait = nullptr;
	int selected_option = -1;

	bool has_set_player_options = false;
	size_t dialogue_text_characters = 0;
	Timer dialogue_characters_timer;
	float dialogue_characters_delay = 0.05f;
};