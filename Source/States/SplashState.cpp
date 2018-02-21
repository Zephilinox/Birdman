#include "SplashState.hpp"
//SELF
#include "../Architecture/GameData.hpp"
#include "../States/MenuState.hpp"
#include "../States/FadeOutState.hpp"

SplashState::SplashState(GameData* game_data)
	: BaseState(game_data)
{
}

void SplashState::update(const ASGE::GameTime& gt)
{
	if (timer.getElapsedTime() > 1)
	{
		game_data->getStateManager()->pop();
		game_data->getStateManager()->push<MenuState>();
	}
}

void SplashState::render() const
{
	game_data->getRenderer()->renderText("SPLASH SCREEN", 560, 320);
}

void SplashState::onActive()
{
}

void SplashState::onInactive()
{
}