#include "FadeOutState.hpp"

//SELF
#include "FadeInState.hpp"
#include "../Constants.hpp"
#include "../Architecture/GameData.hpp"

FadeOutState::FadeOutState(std::function<void()> callback)
	: BaseState(true)
	, left_curtain(GameData::getRenderer()->createRawSprite())
	, right_curtain(GameData::getRenderer()->createRawSprite())
	, fade_end_callback(callback)
{
	if (!left_curtain->loadTexture("../../Resources/Textures/leftcurtain.png"))
	{
		throw "Failed to load leftcurtain.png";
	}
	if (!right_curtain->loadTexture("../../Resources/Textures/rightcurtain.png"))
	{
		throw "Failed to load rightcurtain.png";
	}

	left_curtain->xPos(float(-GameData::getWindowWidth()/2));
	right_curtain->xPos(float(GameData::getWindowWidth()));
}

void FadeOutState::update(const ASGE::GameTime& gt)
{
	if (!closed)
	{
		left_curtain->xPos(left_curtain->xPos() + (500 * float((gt.delta_time.count() / 1000.0f))));
		right_curtain->xPos(right_curtain->xPos() - (500 * float((gt.delta_time.count() / 1000.0f))));

		if (left_curtain->xPos() >= 0 && right_curtain->xPos() <= GameData::getWindowWidth() / 2)
		{
			timer.restart();
			closed = true;
		}
	}
	
	if (closed && timer.getElapsedTime() >= 0.1f)
	{
		GameData::getStates()->pop();
		fade_end_callback();
		GameData::getStates()->push<FadeInState>();
	}
}

void FadeOutState::render() const
{
	GameData::getRenderer()->renderSprite(*left_curtain);
	GameData::getRenderer()->renderSprite(*right_curtain);
}

void FadeOutState::onActive()
{
}

void FadeOutState::onInactive()
{
}
