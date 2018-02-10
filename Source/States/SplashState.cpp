#include "SplashState.hpp"
//SELF
#include "../Architecture/GameData.hpp"
#include "../States/MenuState.hpp"
#include "../States/FadeOutState.hpp"

SplashState::SplashState(GameData* game_data)
	: BaseState(game_data)
{
	start_time = std::chrono::system_clock().now();
}

void SplashState::update(const ASGE::GameTime& gt)
{
	auto end_time = std::chrono::system_clock().now();
	std::chrono::duration<float> duration = end_time - start_time;

	if (duration.count() >= 1.0f)
	{
		//game_data->getStateManager()->pop();
		//game_data->getStateManager()->push<MenuState>();
		game_data->getStateManager()->pop();
		game_data->getStateManager()->push<MenuState>();
		start_time = end_time;
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