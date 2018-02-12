#include "VisualDialogue.hpp"

//LIB
#include <Engine/Renderer.h>

//SELF
#include "../Architecture/Dialogues/DialogueTree.hpp"
#include "../Constants.hpp"

VisualDialogue::VisualDialogue(GameData* game_data, DialogueTree* dialogue_tree, std::string default_dialogue)
	: game_data(game_data)
	, dialogue_tree(dialogue_tree)
	, options(game_data)
	, default_dialogue(default_dialogue)
{
}

void VisualDialogue::setDefaultDialogue(std::string dialogue)
{
	default_dialogue = dialogue;
}

void VisualDialogue::interact()
{
	if (selected_option >= 0)
	{
		dialogue_text = dialogue_tree->play(dialogue_tree->current_player_options[selected_option]->next());
		selected_option = -1;
	}
	else
	{
		const auto& txt = dialogue_tree->next();
		if (!dialogue_tree->player_option)
		{
			dialogue_text = txt;
		}
	}

	if (dialogue_text == "")
	{
		dialogue_text = dialogue_tree->play(default_dialogue);
	}
}

void VisualDialogue::setupPlayerOptions()
{
	has_set_player_options = true;
	std::cout << "PLAYER OPTIONS\n";
	int validOptions = 0;
	for (size_t i = 0; i < dialogue_tree->current_player_options.size(); ++i)
	{
		std::string txt = dialogue_tree->current_player_options[i]->text();
		if (txt != "")
		{
			int id = options.addButton(WINDOW_WIDTH / 4, 200 + (validOptions * 70), txt, ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
			options.getButton(id).on_click.connect(
				[&, i]()
			{
				has_set_player_options = false;
				selected_option = i;
				options = Menu(game_data);
			});
			validOptions++;
		}
	}
}

void VisualDialogue::update()
{
	updateTree();
	options.update();
}

//todo: change this to be ran when changing dialogue tree we're managing
void VisualDialogue::updateTree()
{
	if (!dialogue_tree->playing)
	{
		dialogue_text = dialogue_tree->play(default_dialogue);
	}

	if (dialogue_tree->player_option && !has_set_player_options)
	{
		setupPlayerOptions();
	}
}

void VisualDialogue::render() const
{
	std::string prefix = "";
	if (dialogue_tree->getSpeaker() && dialogue_tree->getSpeaker()->name != "")
	{
		prefix = dialogue_tree->getSpeaker()->name + ": ";
	}

	game_data->getRenderer()->renderText((prefix + dialogue_text).c_str(), WINDOW_WIDTH / 4, 100);
	options.render();
}
