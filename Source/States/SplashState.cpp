#include "SplashState.hpp"
//SELF
#include "../Architecture/GameData.hpp"
#include "../States/MenuState.hpp"
#include "../States/FadeOutState.hpp"
#include "../States/FadeInState.hpp"
#include "../States/GameState.hpp"
#include "../Messages/AudioChangeMessage.hpp"
#include "..\Constants.hpp"

SplashState::SplashState(GameData* game_data)
	: BaseState(game_data)
	, left_curtain(game_data->getRenderer()->createRawSprite())
	, right_curtain(game_data->getRenderer()->createRawSprite())
	, tutorial_image(game_data->getRenderer()->createRawSprite())
{
	if (!left_curtain->loadTexture("../../Resources/Textures/leftcurtain.png"))
	{
		throw "Failed to load leftcurtain.png";
	}

	if (!right_curtain->loadTexture("../../Resources/Textures/rightcurtain.png"))
	{
		throw "Failed to load rightcurtain.png";
	}

	if (!tutorial_image->loadTexture("../../Resources/Textures/tutorial.png"))
	{
		throw "Failed to load tutorial.png";
	}

	right_curtain->xPos(float(game_data->getWindowWidth() / 2));
}

void SplashState::update(const ASGE::GameTime& gt)
{
	if (tutorial)
	{
		if (game_data->getInputManager()->isActionPressed("enter"))
		{
			tutorial = false;
			game_data->getStateManager()->push<GameState>();
			game_data->getStateManager()->push<FadeInState>();
		}
	}
}

void SplashState::render() const
{
	if (tutorial)
	{
		game_data->getRenderer()->renderSprite(*tutorial_image, Z_ORDER_LAYER::CURTAINS);
	}
	else
	{
		game_data->getRenderer()->renderSprite(*left_curtain, Z_ORDER_LAYER::CURTAINS);
		game_data->getRenderer()->renderSprite(*right_curtain, Z_ORDER_LAYER::CURTAINS);
	}
}

void SplashState::onActive()
{
}

void SplashState::onInactive()
{
}