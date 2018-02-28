#include "FadeOutState.hpp"

//SELF
#include "FadeInState.hpp"
#include "../Constants.hpp"
#include "../Architecture/GameData.hpp"

FadeInState::FadeInState()
	: BaseState(true)
	, left_curtain(GameData::getRenderer()->createRawSprite())
	, right_curtain(GameData::getRenderer()->createRawSprite())
{
	if (!left_curtain->loadTexture("../../Resources/Textures/leftcurtain.png"))
	{
		throw std::exception("[FadeOutState::FadeOutState()] Failed to load '../../Resources/Textures/leftcurtain.png'\n");
	}
	if (!right_curtain->loadTexture("../../Resources/Textures/rightcurtain.png"))
	{
		throw std::exception("[FadeOutState::FadeOutState()] Failed to load '../../Resources/Textures/rightcurtain.png'\n");
	}
	right_curtain->xPos(GameData::getWindowWidth() / 2.0f);
}
  
void FadeInState::update(const ASGE::GameTime& gt)
{
	left_curtain->xPos(left_curtain->xPos() - 500 * float((gt.delta_time.count() / 1000.0f)));
	right_curtain->xPos(right_curtain->xPos() + 500 * float((gt.delta_time.count() / 1000.0f)));

	if (left_curtain->xPos() < -GameData::getWindowWidth() /2 && right_curtain->xPos() > GameData::getWindowWidth())
	{
		GameData::getStates()->pop();
	}
}

void FadeInState::render() const
{
	GameData::getRenderer()->renderSprite(*left_curtain);

	GameData::getRenderer()->renderSprite(*right_curtain);
}

void FadeInState::onActive()
{
}

void FadeInState::onInactive()
{
}
