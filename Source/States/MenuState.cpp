#include "MenuState.hpp"

//SELF
#include "../Architecture/GameData.hpp"
#include "../Constants.hpp"

MenuState::MenuState(GameData* game_data)
	: BaseState(game_data)
	, menu(game_data)
{
	//menu buttons
	menu.addButton(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 - 40, "ADD SWORD", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
	menu.addButton(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 + 40, "EXIT", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);

	//register actions to button clicks
	//button lives as long as menu, which lives as long as this menu state
	//so no need to keep track of Connection lifetime
	menu.getButton(0).on_click.connect([&]()
	{
		dialogues.getPlayer()->addFlag("super_cool_sword_thingy");
		std::cout << "PLAYER GAINED SWORD\n";
	});

	menu.getButton(1).on_click.connect([game_data]()
	{
		game_data->getStateManager()->pop();
	});

	dialogue_init();
}

void MenuState::update(const ASGE::GameTime&)
{
	menu.update();

	if (game_data->getInputManager()->isKeyPressed(ASGE::KEYS::KEY_E))
	{
		if (!dialogues.playing)
		{
			std::cout << dialogues.play("start") << "\n\n";
		}
		else
		{
			if (dialogues.player_option)
			{
				int i = 0;
				for (Dialogue* d : dialogues.current_player_options)
				{
					std::string txt = d->text();

					++i;

					if (txt != "")
					{
						std::cout << "Option " << i << ": " << txt << "\n";
					}
				}

				int choice = -1;
				std::cout << "Choice: ";
				std::cin >> choice;
				//NOTE: will skip numbers, but won't matter for final game?
				std::cout << dialogues.play(dialogues.current_player_options[choice - 1]->next()) << "\n\n";
			}
			else
			{
				std::cout << dialogues.next() << "\n\n";
			}
		}
	}
}

void MenuState::render() const
{
	menu.render();
}

void MenuState::onActive()
{
}

void MenuState::onInactive()
{
}

void MenuState::dialogue_init()
{
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
		static bool added_button = false;

		dialogues.getSpeaker()->addFlag("met_player");

		if (dialogues.getPlayer()->hasFlag("npc_killed"))
		{
			//TODO: remove button function
			if (added_button)
			{
				menu.getButton(2).setName("INVALID");
			}

			return "end";
		}

		if (dialogues.getSpeaker()->hasFlag("stole_sword"))
		{
			//hacky
			//TODO: get button by name

			if (!added_button)
			{
				menu.addButton(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 - 120, "FIND STRANGE NPC", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
				menu.getButton(2).on_click.connect(
					[&]()
				{
					dialogues.getPlayer()->addFlag("npc_found");
					std::cout << "NPC FOUND\n";
				});
				added_button = true;
			}

			return "";
		}
		else
		{
			return "start_options";
		}
	});

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