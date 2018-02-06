#include "FadeOutState.hpp"

//SELF
#include "FadeInState.hpp"
#include "../Constants.hpp"
#include "../Architecture/GameData.hpp"

FadeOutState::FadeOutState(GameData* game_data)
	: BaseState(game_data, true)
	, black_screen(game_data->getRenderer()->createRawSprite())
{
	if (!black_screen->loadTexture("../../Resources/Textures/BlackScreen.png"))
	{
		throw std::exception("[FadeOutState::FadeOutState()] Failed to load '../../Resources/Textures/BlackScreen.png'\n");
	}

	black_screen->yPos(float(WINDOW_HEIGHT));
}

//todo: make this and fade in state nicer
//maybe like curtains?
//if curtains, find animation for it maybe and use AnimatedSprite? not sure
void FadeOutState::update(const ASGE::GameTime& gt)
{
	black_screen->yPos(black_screen->yPos() - (800 * float((gt.delta_time.count() / 1000.0f))));
	if (black_screen->yPos() <= 0)
	{
		game_data->getStateManager()->pop();
		fade_end_callback();
		game_data->getStateManager()->top()->update(gt);
		game_data->getStateManager()->push<FadeInState>();
	}
}

void FadeOutState::render() const
{
	game_data->getRenderer()->renderSprite(*black_screen);
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
