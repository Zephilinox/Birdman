#include "FadeOutState.hpp"

//SELF
#include "FadeInState.hpp"
#include "../Constants.hpp"
#include "../Architecture/GameData.hpp"

FadeOutState::FadeOutState(GameData* game_data)
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

	left_curtain->xPos(float(-WINDOW_WIDTH/2));
	right_curtain->xPos(float(WINDOW_WIDTH));
}

void FadeOutState::update(const ASGE::GameTime& gt)
{
	left_curtain->xPos(left_curtain->xPos() + (360 * float((gt.delta_time.count() / 1000.0f))));
	if (left_curtain->xPos() >= 0)
	{
		game_data->getStateManager()->pop();
		fade_end_callback();
		game_data->getStateManager()->top()->update(gt);
		game_data->getStateManager()->push<FadeInState>();
	}
	right_curtain->xPos(right_curtain->xPos() - (360 * float((gt.delta_time.count() / 1000.0f))));
	if (right_curtain->xPos() <= WINDOW_WIDTH/2)
	{
		game_data->getStateManager()->pop();
		fade_end_callback();
		game_data->getStateManager()->top()->update(gt);
		game_data->getStateManager()->push<FadeInState>();
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

void FadeOutState::setFadeEndCallback(std::function<void(void)> func)
{
	fade_end_callback = func;
}
