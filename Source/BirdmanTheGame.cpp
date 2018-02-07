#include "BirdmanTheGame.hpp"

//STD
#include <iostream>

//LIB
#include <Engine/GameTime.h>
#include <Engine/InputEvents.h>

//SELF
#include "Architecture/GameData.hpp"
#include "Constants.hpp"
#include "States/SplashState.hpp"

//Chris - added here too, simply to initialise it. Unsure if needed here.
#include "Architecture/Managers/SceneManager.h"

BirdmanTheGame::~BirdmanTheGame()
{
	inputs->unregisterCallback(key_handler_id);
}

bool BirdmanTheGame::init()
{
	game_width = WINDOW_WIDTH;
	game_height = WINDOW_HEIGHT;

	if (!initAPI(ASGE::Renderer::WindowMode::WINDOWED))
	{
		return false;
	}

	game_data = std::make_unique<GameData>(renderer.get());

	renderer->setWindowTitle("Birbie");
	float cArray[] = { 0.08f, 0.08f, 0.08f };
	renderer->setClearColour(cArray);
	renderer->setSpriteMode(ASGE::SpriteSortMode::BACK_TO_FRONT);
	toggleFPS();

	key_handler_id = inputs->addCallbackFnc(ASGE::EventType::E_KEY, &BirdmanTheGame::keyHandler, this);

	game_data->getFontManager()->addFont("../../Resources/Fonts/Comic.ttf", "Default");
	game_data->getStateManager()->push<SplashState>();

	//Chris - hmmm
	// TODO use this to initialise the scene at gameplay state start
	game_data->getSceneManager()->setCurrentScene(0);

	return true;
}

void BirdmanTheGame::update(const ASGE::GameTime& gt)
{
	game_data->getInputManager()->update();
	game_data->getMessageQueue()->processMessages(std::chrono::microseconds(2000));
	game_data->getStateManager()->update(gt);

	if (game_data->getInputManager()->isKeyPressed(ASGE::KEYS::KEY_ESCAPE))
	{
		game_data->getStateManager()->pop();
	}

	if (renderer->exit() || this->exit || game_data->getStateManager()->empty())
	{
		signalExit();
	}
}

void BirdmanTheGame::render(const ASGE::GameTime& gt)
{
	game_data->getStateManager()->render();
	game_data->getSceneManager()->render();
}

void BirdmanTheGame::keyHandler(const ASGE::SharedEventData data)
{
	const auto key_event = static_cast<const ASGE::KeyEvent*>(data.get());

	game_data->getInputManager()->handleInput(key_event->key, key_event->action);
}
