#include "PauseState.hpp"

//SELF
#include "FadeOutState.hpp"
#include "GameState.hpp"
#include "../Architecture/Timer.hpp"

PauseState::PauseState(GameData* game_data)
	: BaseState(game_data, true)
	, menu(game_data)
	, dim_background(game_data->getRenderer()->createRawSprite())
{
	if (!dim_background->loadTexture("../../Resources/Textures/BlackScreen.png"))
	{
		throw "Failed to load BlackScreen.png";
	}

	menu.addButton(game_data->getWindowWidth() / 2 - 80, game_data->getWindowHeight() / 2 - 120, "CONTINUE", ASGE::COLOURS::GRAY, ASGE::COLOURS::ANTIQUEWHITE);
	menu.addButton(game_data->getWindowWidth() / 2 - 80, game_data->getWindowHeight() / 2 - 40, "RESTART", ASGE::COLOURS::GRAY, ASGE::COLOURS::ANTIQUEWHITE);
	menu.addButton(game_data->getWindowWidth() / 2 - 80, game_data->getWindowHeight() / 2 + 40, "TOGGLE AUDIO", ASGE::COLOURS::GRAY, ASGE::COLOURS::ANTIQUEWHITE);
	menu.addButton(game_data->getWindowWidth() / 2 - 80, game_data->getWindowHeight() / 2 + 120, "EXIT", ASGE::COLOURS::GRAY, ASGE::COLOURS::ANTIQUEWHITE);

	menu.getButton(0).on_click.connect([gd = game_data]()
	{
		gd->getStateManager()->pop();
	});

	menu.getButton(1).on_click.connect([game_data]()
	{
		game_data->getStateManager()->pop();
		game_data->getStateManager()->push<FadeOutState>(
		[game_data]()
		{
			game_data->getStateManager()->pop();
			game_data->getStateManager()->push<GameState>();
		});
	});

	menu.getButton(2).on_click.connect([game_data]()
	{
		if (game_data->getAudioManager()->getEngineType() == AudioManager::Engine::None)
		{
			game_data->getAudioManager()->setEngineType(AudioManager::Engine::IrrKlang);
		}
		else
		{
			game_data->getAudioManager()->setEngineType(AudioManager::Engine::None);
		}
	});

	menu.getButton(3).on_click.connect([game_data]()
	{
		game_data->exit = true;
	});
}

void PauseState::update(const ASGE::GameTime &)
{
	menu.update();
	
	if (game_data->getInputManager()->isKeyPressed(ASGE::KEYS::KEY_ESCAPE))
	{
		game_data->getStateManager()->pop();
	}
}

void PauseState::render() const
{
	game_data->getRenderer()->renderSprite(*dim_background);
	menu.render();
}

void PauseState::onActive()
{
}

void PauseState::onInactive()
{
}
