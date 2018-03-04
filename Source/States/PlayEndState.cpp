#include "PlayEndState.h"
#include "FadeOutState.hpp"
#include "GameState.hpp"
#include "..\Constants.hpp"

PlayEndState::PlayEndState(GameData * game_data, float score)
	: BaseState(game_data, true),
	ok_popup(game_data),
	dim_background(game_data->getRenderer()->createUniqueSprite()),
	score_panel(game_data->getRenderer()->createUniqueSprite())
{
	final_score = (int)score;

	//load textures
	if(!dim_background->loadTexture("../../Resources/Textures/BlackScreen.png"))
	{
		throw "Failed to load BlackScreen.png";
	}

	if(!score_panel->loadTexture("../../Resources/Textures/UI/PlayEndPanel.png"))
	{
		throw "Failed to load PlayEndPanel.png";
	}

	score_panel->xPos((game_data->getWindowWidth() * 0.5f) - (score_panel->width() * 0.5f));
	score_panel->yPos((game_data->getWindowHeight() * 0.5f) - (score_panel->height() * 0.5f));

	//add button with set colour
	const float c[3] = { 0.817f, 0.668f, 0.515f };
	ok_popup.addButton(game_data->getWindowWidth() / 2 - 54, (game_data->getWindowHeight() / 2) + 35, "PLAY AGAIN", c, ASGE::COLOURS::ANTIQUEWHITE);
	ok_popup.addButton(game_data->getWindowWidth() / 2 - 24, game_data->getWindowHeight() / 2 + 72, "EXIT", c, ASGE::COLOURS::ANTIQUEWHITE);

	//on click, reset game
	ok_popup.getButton(0).on_click.connect([game_data]()
	{
		game_data->getStateManager()->push<FadeOutState>(
			[game_data]()
		{
			game_data->getStateManager()->pop();
			game_data->getStateManager()->pop();
			game_data->getStateManager()->push<GameState>();
		});
	});

	ok_popup.getButton(1).on_click.connect([game_data]()
	{
		game_data->exit = true;
	});
}

void PlayEndState::update(const ASGE::GameTime &)
{
	ok_popup.update();
}

void PlayEndState::render() const
{
	game_data->getRenderer()->renderSprite(*dim_background, Z_ORDER_LAYER::PANELS);
	game_data->getRenderer()->renderSprite(*score_panel, Z_ORDER_LAYER::PANELS + 1);
	ok_popup.render(Z_ORDER_LAYER::PANELS_HIGHER_TEXT);
	std::string output_score = "Final Score: " + std::to_string(final_score);
	const float c[3] = { 0.917, 0.768, 0.615 };
	game_data->getRenderer()->renderText(output_score, (game_data->getWindowWidth() / 2) - 80, (game_data->getWindowHeight() / 2) -40, 1.0f, c, Z_ORDER_LAYER::PANELS_TEXT);
}

void PlayEndState::onActive()
{
}

void PlayEndState::onInactive()
{
}
