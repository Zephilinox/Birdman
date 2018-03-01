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
	, dialogue_finished_marker(game_data->getRenderer())
	, speaker_underline(game_data->getRenderer()->createRawSprite())
{
	options.selection_image.reset(game_data->getRenderer()->createRawSprite());
	options.selection_image->loadTexture("../../Resources/Textures/UI/ButtonSelection.png");

	speaker_underline->loadTexture("../../Resources/Textures/UI/SpeakerUnderline.png");
	speaker_underline->xPos(735);
	speaker_underline->yPos(520);
	
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

	dialogue_finished_marker.xPos = 666;
	dialogue_finished_marker.yPos = game_data->getWindowHeight() - 186.0f;
	dialogue_finished_marker.addFrame("UI/DialogueMarker", 0.6f);
	dialogue_finished_marker.addFrame("UI/DialogueMarker", 0.3f, 0, 10);
	dialogue_finished_marker.pause();
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
		options.reset();
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
	const float dark[3] = { 0.4, 0.349, 0.317 };

	has_set_player_options = true;
	int validOptions = 0;
	for (size_t i = 0; i < dialogue_tree->current_player_options.size(); ++i)
	{
		std::string txt = dialogue_tree->current_player_options[i]->text();
		if (txt != "")
		{
			int id = options.addButton(20, game_data->getWindowHeight() - 113 + (validOptions * 30), txt, dark, dark);
			options.getButton(id).on_click.connect(
			[&, i]()
			{
				has_set_player_options = false;
				selected_option = i;
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

void VisualDialogue::setDialogueSpeed(int speed) noexcept
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

void VisualDialogue::update(float dt)
{
	if (dialogue_text_characters < dialogue_text.length() && dialogue_characters_timer.getElapsedTime() > dialogue_characters_delay)
	{
		dialogue_characters_timer.restart();
		dialogue_text_characters++;

		if (dialogue_text_characters >= dialogue_text.length())
		{
			dialogue_finished_marker.restart();
		}
	}

	updateTree();
	options.update();
	dialogue_finished_marker.update(dt);
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
	const float dark[3] = { 0.4, 0.349, 0.317 };

	game_data->getFontManager()->setFont("Dialogue");
	game_data->getRenderer()->setDefaultTextColour(dark);

	Actor* speaker = dialogue_tree->getSpeaker();
	if (dialogue_tree->player_option && dialogue_tree->getPreviousDialogue() && !dialogue_tree->getPreviousDialogue()->player_option)
	{
		speaker = dialogue_tree->getPreviousSpeaker();
	}

	if (speaker)
	{
		int txtSize = speaker->realName.size() * 4;
		game_data->getRenderer()->renderText(speaker->realName.c_str(), 740 + txtSize, game_data->getWindowHeight() - 210);
		game_data->getRenderer()->renderSprite(*speaker_underline);

		if (speaker->portrait)
		{
			speaker->portrait->xPos(740);
			speaker->portrait->yPos(game_data->getWindowHeight() - 185.0f);
			game_data->getRenderer()->renderSprite(*speaker->portrait.get());
		}
	}

	game_data->getRenderer()->renderText(dialogue_text.substr(0, dialogue_text_characters).c_str(), 20, game_data->getWindowHeight() - 220);

	if (dialogue_text != "" && dialogue_text_characters >= dialogue_text.length())
	{
		game_data->getRenderer()->renderSprite(*dialogue_finished_marker.getCurrentFrameSprite());
	}

	options.render();
}
