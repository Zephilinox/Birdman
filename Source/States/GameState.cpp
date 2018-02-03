#include "GameState.hpp"

//SELF
#include "../Architecture/GameData.hpp"
#include "../Constants.hpp"

GameState::GameState(GameData* game_data)
	: BaseState(game_data)
	, dialogue_menu(game_data)
{
	//add all the dialogue to the dialogue tree. each level would have its own dialogue tree
	dialogue_init();
}

void GameState::update(const ASGE::GameTime&)
{
	static bool has_set_player_options = false;

	dialogue_menu.update();

	if (!dialogues.playing)
	{
		dialogue_text = dialogues.play("start_extra");
	}

	if (game_data->getInputManager()->isKeyPressed(ASGE::KEYS::KEY_ENTER))
	{
		if (selected_option >= 0)
		{
			dialogue_text = dialogues.play(dialogues.current_player_options[selected_option]->next());
			selected_option = -1;
		}
		else
		{
			const auto& txt = dialogues.next();
			if (!dialogues.player_option)
			{
				dialogue_text = txt;
			}
		}

		if (dialogue_text == "")
		{
			dialogue_text = dialogues.play("start_extra");
		}
	}

	if (dialogues.player_option && !has_set_player_options)
	{
		has_set_player_options = true;
		std::cout << "PLAYER OPTIONS\n";
		int validOptions = 0;
		for (int i = 0; i < dialogues.current_player_options.size(); ++i)
		{
			std::string txt = dialogues.current_player_options[i]->text();
			if (txt != "")
			{
				int id = dialogue_menu.addButton(0, 200 + (validOptions * 50), txt, ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
				dialogue_menu.getButton(id).on_click.connect(
				[&, i]()
				{
					has_set_player_options = false;
					selected_option = i;
					dialogue_menu = Menu(game_data);
				});
				validOptions++;
			}
		}
	}
}

void GameState::render() const
{
	game_data->getRenderer()->renderText(dialogue_text.c_str(), 0, 100);
	dialogue_menu.render();
}

void GameState::onActive()
{
}

void GameState::onInactive()
{
}

void GameState::dialogue_init()
{
	dialogues.addPlayerOption("start_extra",
		[&]()
	{
		if (dialogues.getPlayer()->hasFlag("super_cool_sword_thingy"))
		{
			return "";
		}

		return "Get Sword";
	},
		[&]()
	{
		dialogues.getPlayer()->addFlag("super_cool_sword_thingy");
		return "";
	});

	dialogues.addPlayerOption("start_extra",
	[&]()
	{
		if (dialogues.getActor("strange_npc")->hasFlag("stole_sword") && !dialogues.getPlayer()->hasFlag("npc_found"))
		{
			return "Find Strange NPC";
		}

		return "Strange NPC";
	},
	[&]()
	{	
		if (dialogues.getActor("strange_npc")->hasFlag("stole_sword") && !dialogues.getPlayer()->hasFlag("npc_found"))
		{
			dialogues.getPlayer()->addFlag("npc_found");
			return "";
		}

		return "start";
	});

	dialogues.addDialogue("start", "strange_npc",
		[&]()
	{
		if (dialogues.getPlayer()->hasFlag("npc_killed"))
		{
			if (dialogues.getSpeaker()->hasFlag("stole_sword"))
			{
				return "*You stand over the dead corpse of the man who stole your sword*";
			}
			else
			{
				return "*You stand over the dead corpse of the man you randomly slaughtered*";
			}
		}

		if (dialogues.getPlayer()->hasFlag("npc_found"))
		{
			dialogues.getPlayer()->addFlag("npc_killed");
			return "No! Please! *ARRRGH*";
		}

		if (dialogues.getSpeaker()->hasFlag("stole_sword"))
		{
			return "*The strange NPC who stole your sword is nowhere to be found...*";
		}

		if (!dialogues.getSpeaker()->hasFlag("met_player"))
		{
			return "What? Who are you? What do you want?";
		}

		return "What? You're still here? What is it now.";
	},
		[&]()
	{
		dialogues.getSpeaker()->addFlag("met_player");

		if (dialogues.getPlayer()->hasFlag("npc_killed"))
		{
			return "end";
		}

		if (dialogues.getSpeaker()->hasFlag("stole_sword"))
		{
			return "";
		}
		else
		{
			return "start_options";
		}
	});

	//Note: player options work differently to ordinary dialogues though they're kinda the same
	//Multiple options can have the same dialogue name, which is what allows them to all be presented
	//If it returns a blank string when determining what the dialogue will be, the option should not be shown
	//(but this is meant to be handled by whatever handles the option, like the console/visual DialogueManager
	//Also keep in mind you might want to set the next dialogue to be one where the player is speaking and not an NPC, but I was lazy.
	dialogues.addPlayerOption("start_options", "Hello.", "start2");
	dialogues.addPlayerOption("start_options", "Howdy.", "start2");
	dialogues.addPlayerOption("start_options", "Hey.", "start2");

	dialogues.addPlayerOption("start_options",
		[&]()
	{
		if (dialogues.getPlayer()->hasFlag("super_cool_sword_thingy"))
		{
			dialogues.getPlayer()->addFlag("npc_found");
			return "Die!";
		}

		return "";
	}, "start");

	dialogues.addPlayerOption("start_options", "Bye.", "bye");

	dialogues.addDialogue("start2", "strange_npc", "Well I'm quite busy right now.", "bye");

	dialogues.addDialogue("bye", "strange_npc",
		[&]()
	{
		if (dialogues.getPlayer()->hasFlag("super_cool_sword_thingy"))
		{
			dialogues.getSpeaker()->addFlag("plans_to_steal_sword");
			return "Wait a minute...";
		}
		return "Speak to you another time.";
	},
		[&]()
	{
		if (dialogues.getSpeaker()->hasFlag("plans_to_steal_sword"))
		{
			return "steal_sword";
		}

		return "";
	});

	dialogues.addDialogue("steal_sword", "strange_npc",
		[&]()
	{
		dialogues.getPlayer()->removeFlag("super_cool_sword_thingy");
		dialogues.getSpeaker()->removeFlag("plans_to_steal_sword");
		dialogues.getSpeaker()->addFlag("stole_sword");

		return "*Steals Your Sword*";
	}, "");

	dialogues.addDialogue("end", "player",
		[&]()
	{
		if (dialogues.getSpeaker()->hasFlag("stole_sword"))
		{
			dialogues.getPlayer()->addFlag("super_cool_sword_thingy");

			return "He got what he deserved, and now I have my sword back.";
		}
		else
		{
			return "I don't know why I did that...";
		}
	}, "");
}