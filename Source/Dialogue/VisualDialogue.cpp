#include "VisualDialogue.hpp"

//LIB
#include <Engine/Renderer.h>

//SELF
#include "../Architecture/Dialogues/DialogueTree.hpp"
#include "../Constants.hpp"

VisualDialogue::VisualDialogue(GameData* game_data, DialogueTree* dialogue_tree, std::string starting_dialogue)
	: game_data(game_data)
	, dialogue_tree(dialogue_tree)
	, options(game_data)
	, starting_dialogue(starting_dialogue)
{}

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
		dialogue_text = dialogue_tree->play(starting_dialogue);
	}
}

void VisualDialogue::update()
{
	if (!dialogue_tree->playing)
	{
		dialogue_text = dialogue_tree->play("start_extra");
	}

	if (dialogue_tree->player_option && !has_set_player_options)
	{
		has_set_player_options = true;
		std::cout << "PLAYER OPTIONS\n";
		int validOptions = 0;
		for (int i = 0; i < dialogue_tree->current_player_options.size(); ++i)
		{
			std::string txt = dialogue_tree->current_player_options[i]->text();
			if (txt != "")
			{
				int id = options.addButton(WINDOW_WIDTH / 3, 200 + (validOptions * 70), txt, ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
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

	options.update();
}

void VisualDialogue::render() const
{
	game_data->getRenderer()->renderText(dialogue_text.c_str(), WINDOW_WIDTH / 3, 100);
	options.render();
}
