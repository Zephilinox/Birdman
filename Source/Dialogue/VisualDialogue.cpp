#include "VisualDialogue.hpp"

//STD
#include <iostream>

//LIB
#include <Engine/Renderer.h>
#include <ini_parser.hpp>

//SELF
#include "../Architecture/Dialogues/DialogueTree.hpp"
#include "../Constants.hpp"

VisualDialogue::VisualDialogue(GameData* game_data, DialogueTree* dialogue_tree, std::string default_dialogue)
	: game_data(game_data)
	, dialogue_tree(dialogue_tree)
	, options(game_data)
	, default_dialogue(default_dialogue)
{
	ini_parser ini("settings.ini");
	
	try
	{
		setDialogueSpeed(ini.get_int("DialogueSpeed"));
	}
	catch (std::runtime_error& e)
	{
		std::cout << "ERROR: Setting 'DialogueSpeed' not found in 'settings.ini'.\n";
		std::cout << "ERROR INFO: " << e.what() << "\n";
		setDialogueSpeed(DialogueSpeed::Instant);
	}
}

void VisualDialogue::setDefaultDialogue(std::string dialogue)
{
	default_dialogue = dialogue;
}

void VisualDialogue::interact()
{
	if (dialogue_text_characters < dialogue_text.length())
	{
		dialogue_text_characters = dialogue_text.length();
		return;
	}

	if (selected_option >= 0)
	{
		const auto& txt = dialogue_tree->play(dialogue_tree->current_player_options[selected_option]->next());
		setDialogueText(txt);
		selected_option = -1;
	}
	else
	{
		const auto& txt = dialogue_tree->next();
		if (!dialogue_tree->player_option)
		{
			setDialogueText(txt);
		}
	}

	if (dialogue_text == "")
	{
		setDialogueText(dialogue_tree->play(default_dialogue));
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
			int id = options.addButton(30, game_data->getWindowHeight() - 120 + (validOptions * 30), txt, ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
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

	if (validOptions > 4)
	{
		throw "Too many player options!";
	}
}

void VisualDialogue::setDialogueText(std::string text)
{
	dialogue_text = text;
	dialogue_text_characters = 0;
	dialogue_characters_timer.restart();
}

void VisualDialogue::setDialogueSpeed(int speed)
{
	switch (speed)
	{
		case DialogueSpeed::VerySlow:
			dialogue_characters_delay = 0.2f;
			break;
		case DialogueSpeed::Slow:
			dialogue_characters_delay = 0.1f;
			break;
		case DialogueSpeed::Normal:
			dialogue_characters_delay = 0.05f;
			break;
		case DialogueSpeed::Fast:
			dialogue_characters_delay = 0.025f;
			break;
		case DialogueSpeed::VeryFast:
			dialogue_characters_delay = 0.0125f;
			break;
		case DialogueSpeed::Instant:
		default:
			dialogue_characters_delay = -1.0f;
			break;
	}
}

void VisualDialogue::update()
{
	if (dialogue_text_characters < dialogue_text.length() && dialogue_characters_timer.getElapsedTime() > dialogue_characters_delay)
	{
		dialogue_characters_timer.restart();
		dialogue_text_characters++;
	}

	updateTree();
	options.update();
}

//todo: change this to be ran when changing dialogue tree we're managing
void VisualDialogue::updateTree()
{
	if (!dialogue_tree->playing)
	{
		setDialogueText(dialogue_tree->play(default_dialogue));
	}

	if (dialogue_tree->player_option && !has_set_player_options)
	{
		setupPlayerOptions();
	}
}

void VisualDialogue::render() const
{
	game_data->getFontManager()->setFont("Default", 24);

	Actor* speaker = dialogue_tree->getSpeaker();
	if (dialogue_tree->player_option && dialogue_tree->getPreviousDialogue() && !dialogue_tree->getPreviousDialogue()->player_option)
	{
		speaker = dialogue_tree->getPreviousSpeaker();
	}

	if (speaker)
	{
		game_data->getRenderer()->renderText(speaker->realName.c_str(), 600, game_data->getWindowHeight() - 150);

		if (speaker->portrait)
		{
			speaker->portrait->xPos(600);
			speaker->portrait->yPos(game_data->getWindowHeight() - 135);
			game_data->getRenderer()->renderSprite(*speaker->portrait.get());
		}
	}

	game_data->getRenderer()->renderText(dialogue_text.substr(0, dialogue_text_characters).c_str(), 30, game_data->getWindowHeight() - 250);

	options.render();
}
