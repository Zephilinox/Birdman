#include "FadeOutState.hpp"

//SELF
#include "FadeInState.hpp"
#include "../Constants.hpp"
#include "../Architecture/GameData.hpp"

FadeOutState::FadeOutState(GameData* game_data, std::function<void()> callback)
	: BaseState(game_data, true)
	, left_curtain(game_data->getRenderer()->createRawSprite())
	, right_curtain(game_data->getRenderer()->createRawSprite())
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

	left_curtain->xPos(float(-game_data->getWindowWidth()/2));
	right_curtain->xPos(float(game_data->getWindowWidth()));
}

void FadeOutState::update(const ASGE::GameTime& gt)
{	
	if (left_curtain->xPos() >= 0 && right_curtain->xPos() <= game_data->getWindowWidth() / 2.0f)
	{
		left_curtain->xPos(0);
		right_curtain->xPos(game_data->getWindowWidth() / 2.0f);

		game_data->getStateManager()->pop();
		fade_end_callback();
		game_data->getStateManager()->push<FadeInState>();
	}
	else
	{
		left_curtain->xPos(left_curtain->xPos() + (500 * float((gt.delta_time.count() / 1000.0f))));
		right_curtain->xPos(right_curtain->xPos() - (500 * float((gt.delta_time.count() / 1000.0f))));
	}
}

void FadeOutState::render() const
{
	game_data->getRenderer()->renderSprite(*left_curtain);
	game_data->getRenderer()->renderSprite(*right_curtain);
}

void FadeOutState::onActive()
{
}

void FadeOutState::onInactive()
{
}
