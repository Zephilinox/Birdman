#include "MenuState.hpp"

//SELF
#include "../Architecture/GameData.hpp"
#include "../Constants.hpp"

MenuState::MenuState(GameData* game_data)
	: BaseState(game_data)
	, menu(game_data)
{
	//menu buttons
	menu.addButton(WINDOW_WIDTH / 2 - 40, WINDOW_HEIGHT / 2 - 40, "PLAY", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
	menu.addButton(WINDOW_WIDTH / 2 - 35, WINDOW_HEIGHT / 2 + 40, "EXIT", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);

	//register actions to button clicks
	//button lives as long as menu, which lives as long as this menu state
	//so no need to keep track of Connection lifetime
	menu.getButton(0).on_click.connect([game_data]()
	{

	});

	menu.getButton(1).on_click.connect([game_data]()
	{
		game_data->getStateManager()->pop();
	});
}

void MenuState::update(const ASGE::GameTime&)
{
	menu.update();
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
