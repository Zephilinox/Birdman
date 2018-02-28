#include "SplashState.hpp"
//SELF
#include "../Architecture/GameData.hpp"
#include "../States/MenuState.hpp"
#include "../States/FadeOutState.hpp"

SplashState::SplashState()
	: BaseState(false)
{
}

void SplashState::update(const ASGE::GameTime& gt)
{
	if (timer.getElapsedTime() > 1)
	{
		GameData::getStates()->pop();
		GameData::getStates()->push<MenuState>();
	}
}

void SplashState::render() const
{
	GameData::getRenderer()->renderText("SPLASH SCREEN", 560, 320);
}

void SplashState::onActive()
{
}

void SplashState::onInactive()
{
}