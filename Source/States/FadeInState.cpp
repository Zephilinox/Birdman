#include "FadeOutState.hpp"

//SELF
#include "FadeInState.hpp"
#include "../Constants.hpp"
#include "../Architecture/GameData.hpp"

FadeInState::FadeInState(GameData* game_data)
	: BaseState(game_data, true)
	, left_curtain(game_data->getRenderer()->createRawSprite())
	, right_curtain(game_data->getRenderer()->createRawSprite())
{
	if (!left_curtain->loadTexture("../../Resources/Textures/leftcurtain.png"))
	{
		throw std::exception("[FadeOutState::FadeOutState()] Failed to load '../../Resources/Textures/leftcurtain.png'\n");
	}
	if (!right_curtain->loadTexture("../../Resources/Textures/rightcurtain.png"))
	{
		throw std::exception("[FadeOutState::FadeOutState()] Failed to load '../../Resources/Textures/rightcurtain.png'\n");
	}
	right_curtain->xPos(game_data->getWindowWidth() / 2.0f);
}
  
void FadeInState::update(const ASGE::GameTime& gt)
{
	left_curtain->xPos(left_curtain->xPos() - 500 * float((gt.delta_time.count() / 1000.0f)));
	right_curtain->xPos(right_curtain->xPos() + 500 * float((gt.delta_time.count() / 1000.0f)));

	if (left_curtain->xPos() < -game_data->getWindowWidth() /2 && right_curtain->xPos() > game_data->getWindowWidth())
	{
		game_data->getStateManager()->pop();
	}
}

void FadeInState::render() const
{
	game_data->getRenderer()->renderSprite(*left_curtain);

	game_data->getRenderer()->renderSprite(*right_curtain);
}

void FadeInState::onActive()
{
}

void FadeInState::onInactive()
{
}
