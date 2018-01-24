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
			std::cout << dialogues.play("start") << "\n";
		}
		else
		{
			std::cout << dialogues.next() << "\n";
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
		if (dialogues.getPlayer()->hasFlag("npc_found"))
		{
			return "*You stand over the dead corpse of the man who stole your sword*";
		}

		if (dialogues.getPlayer()->hasFlag("looking_for_npc"))
		{
			dialogues.getPlayer()->addFlag("npc_found");
			return "No! Please! *ARRRGH*";
		}

		if (dialogues.getActor("strange_npc")->hasFlag("stole_sword"))
		{
			return "*The strange NPC who stole your sword is nowhere to be found...";
		}

		if (!dialogues.getActor("strange_npc")->hasFlag("met_player"))
		{
			return "What? What do you want? Leave me be, peasant. I have no time for beggars.";
		}

		return "What? You're still here? What is it now.";
	},
	[&]()
	{
		if (!dialogues.getActor("strange_npc")->hasFlag("met_player"))
		{
			dialogues.getActor("strange_npc")->addFlag("met_player");
		}

		if (dialogues.getPlayer()->hasFlag("npc_found"))
		{
			//TODO: remove button function
			menu.getButton(2).setName("INVALID");

			return "end";
		}

		if (dialogues.getActor("strange_npc")->hasFlag("stole_sword"))
		{
			static bool addedButton = false;

			if (!addedButton)
			{
				menu.addButton(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 - 120, "FIND STRANGE NPC", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
				menu.getButton(2).on_click.connect(
					[&]()
				{
					dialogues.getPlayer()->addFlag("looking_for_npc");
					std::cout << "NPC FOUND\n";
				});
				addedButton = true;
			}

			return "";
		}
		else
		{
			return "start2";
		}
	});

	dialogues.addDialogue("start2", "strange_npc", "Well I'm quite busy right now.", "bye");

	dialogues.addDialogue("bye", "strange_npc",
	[&]()
	{
		if (dialogues.getPlayer()->hasFlag("super_cool_sword_thingy"))
		{
			dialogues.getActor("strange_npc")->addFlag("plans_to_steal_sword");
			return "Wait a minute...";
		}
		return "Speak to you another time.";
	},
	[&]()
	{
		if (dialogues.getActor("strange_npc")->hasFlag("plans_to_steal_sword"))
		{
			return "steal_sword";
		}

		return "";
	});

	dialogues.addDialogue("steal_sword", "strange_npc",
	[&]()
	{
		dialogues.getPlayer()->removeFlag("super_cool_sword_thingy");
		dialogues.getActor("strange_npc")->removeFlag("plans_to_steal_sword");
		dialogues.getActor("strange_npc")->addFlag("stole_sword");

		return "*Steals Your Sword*";
	}, "");

	dialogues.addDialogue("end", "player",
	[&]()
	{
		dialogues.getPlayer()->addFlag("super_cool_sword_thingy");

		return "He got what he deserved, and now I have my sword back.";
	}, "");
}