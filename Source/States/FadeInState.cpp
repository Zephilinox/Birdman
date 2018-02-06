#include "FadeOutState.hpp"

//SELF
#include "FadeInState.hpp"
#include "../Constants.hpp"
#include "../Architecture/GameData.hpp"

FadeInState::FadeInState(GameData* game_data)
	: BaseState(game_data, true)
	, black_screen(game_data->getRenderer()->createRawSprite())
{
	if (!black_screen->loadTexture("../../Resources/Textures/BlackScreen.png"))
	{
		throw std::exception("[FadeOutState::FadeOutState()] Failed to load '../../Resources/Textures/BlackScreen.png'\n");
	}
}

void FadeInState::update(const ASGE::GameTime& gt)
{
	black_screen->yPos(black_screen->yPos() + 800 * float((gt.delta_time.count() / 1000.0f)));

	if (black_screen->yPos() >= WINDOW_HEIGHT)
	{
		game_data->getStateManager()->pop();
	}
}

void FadeInState::render() const
{
	game_data->getRenderer()->renderSprite(*black_screen);
}

void FadeInState::onActive()
{
}

void FadeInState::onInactive()
{
}
