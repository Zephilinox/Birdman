#include "SplashState.hpp"
//SELF
#include "../Architecture/GameData.hpp"
#include "../States/MenuState.hpp"
#include "../States/FadeOutState.hpp"
#include "../States/FadeInState.hpp"
#include "../States/GameState.hpp"

SplashState::SplashState(GameData* game_data)
	: BaseState(game_data)
	, left_curtain(game_data->getRenderer()->createRawSprite())
	, right_curtain(game_data->getRenderer()->createRawSprite())
{
	if (!left_curtain->loadTexture("../../Resources/Textures/leftcurtain.png"))
	{
		throw "Failed to load leftcurtain.png";
	}

	if (!right_curtain->loadTexture("../../Resources/Textures/rightcurtain.png"))
	{
		throw "Failed to load rightcurtain.png";
	}

	right_curtain->xPos(float(game_data->getWindowWidth() / 2));
}

void SplashState::update(const ASGE::GameTime& gt)
{
	game_data->getStateManager()->push<GameState>();
	game_data->getStateManager()->push<FadeInState>();
	game_data->getAudioManager()->play("Piano Loop.wav", true);
}

void SplashState::render() const
{
	game_data->getRenderer()->renderSprite(*left_curtain);
	game_data->getRenderer()->renderSprite(*right_curtain);
}

void SplashState::onActive()
{
}

void SplashState::onInactive()
{
}