#include "MenuState.hpp"

//SELF
#include "../Architecture/GameData.hpp"
#include "../Constants.hpp"
#include "GameState.hpp"
#include "NetworkingState.hpp"
#include "FadeOutState.hpp"

MenuState::MenuState()
	: BaseState(false)
{
	//menu buttons
	//this is rough and just for prototyping, might need something nicer for the actual game
	menu.addButton(GameData::getWindowWidth() / 2 - 80, GameData::getWindowHeight() / 2 - 40, "PLAY", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
	menu.addButton(GameData::getWindowWidth() / 2 - 80, GameData::getWindowHeight() / 2 + 40, "EXIT", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
	menu.addButton(GameData::getWindowWidth() / 2 - 80, GameData::getWindowHeight() / 2 + 120, "[REMOVE FOR RELEASE] NETWORKING", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
	
	//register actions to button clicks
	//button lives as long as menu, which lives as long as this menu state
	//so no need to keep track of Connection lifetime
	//Take at look at Signal.hpp for more info on how that works.
	menu.getButton(0).on_click.connect([]()
	{
		//here we're using the fade out state to transition to another state
		GameData::getStates()->push<FadeOutState>([]()
		{
			GameData::getStates()->push<GameState>();
		});
	});

	menu.getButton(1).on_click.connect([]()
	{
		GameData::getStates()->pop();
	});

	menu.getButton(2).on_click.connect([]()
	{
		GameData::getStates()->push<NetworkingState>();
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