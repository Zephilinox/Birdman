#include "MenuState.hpp"

//SELF
#include "../Architecture/GameData.hpp"
#include "../Constants.hpp"
#include "GameState.hpp"

MenuState::MenuState(GameData* game_data)
	: BaseState(game_data)
	, menu(game_data)
{
	//menu buttons
	//this is rough and just for prototyping, might need something nicer for the actual game
	menu.addButton(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 - 120, "PLAY", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
	menu.addButton(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 - 40, "ADD SWORD", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
	menu.addButton(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 + 40, "EXIT", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
	
	//register actions to button clicks
	//button lives as long as menu, which lives as long as this menu state
	//so no need to keep track of Connection lifetime
	//Take at look at Signal.hpp for more info on how that works.
	menu.getButton(0).on_click.connect([game_data]()
	{
		game_data->getStateManager()->push<GameState>();
	});

	menu.getButton(1).on_click.connect([&]()
	{
		dialogues.getPlayer()->addFlag("super_cool_sword_thingy");
		std::cout << "PLAYER GAINED SWORD\n";
	});

	menu.getButton(2).on_click.connect([game_data]()
	{
		game_data->getStateManager()->pop();
	});

	//add all the dialogue to the dialogue tree. each level would have its own dialogue tree
	dialogue_init();
}

void MenuState::update(const ASGE::GameTime&)
{
	menu.update();

	//Handles console dialogue tree interaction, careful changing any of this atm.
	//Basically we will need to create a DialogueManager that wraps all of this stuff nicely and handles visuals/menus/selection/etc
	if (game_data->getInputManager()->isKeyPressed(ASGE::KEYS::KEY_E))
	{
		if (!dialogues.playing)
		{
			std::cout << dialogues.play("start") << "\n\n";
		}
		else
		{
			//This needs to be wrapped up and handled better, it's confusing
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
	//Kinda messy, but this might be the best way to do this depending on how big our dialogue branches become per level.
	//It's an option anyway, alternatively just copy+paste it all in to here, as it was before.
	//Note that visual studio can't help you here with what functions and objects might be available, so expect red squiggly lines.
	#include "../Dialogue/example.cpp"
}