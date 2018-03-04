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
#include "States/FadeInState.hpp"
#include "States/GameState.hpp"
#include "Architecture/Messages/FunctionMessage.hpp"

BirdmanTheGame::~BirdmanTheGame()
{
	inputs->unregisterCallback(key_handler_id);
}

bool BirdmanTheGame::init()
{
	game_width = 1280;
	game_height = 720;

	if (!initAPI(ASGE::Renderer::WindowMode::WINDOWED))
	{
		return false;
	}

	renderer->setWindowTitle("Birbie");
	float cArray[] = { 0.08f, 0.08f, 0.08f };
	renderer->setClearColour(std::move(cArray));
	renderer->setSpriteMode(ASGE::SpriteSortMode::IMMEDIATE);

	key_handler_id = inputs->addCallbackFnc(ASGE::EventType::E_KEY, &BirdmanTheGame::keyHandler, this);

	game_data = std::make_unique<GameData>(renderer.get(), inputs.get(), game_width, game_height);
	game_data->getFontManager()->addFont("../../Resources/Fonts/DroidSansMono.ttf", "Default", 24);
	game_data->getFontManager()->addFont("../../Resources/Fonts/DroidSansMono.ttf", "Dialogue", 18);
	
	game_data->getStateManager()->push<SplashState>();

	game_data->getMessageQueue()->addListener([](Message* msg)
	{
		std::cout << "Processed\t" << msg->id << "\n";
	});

	game_data->getMessageQueue()->addListener([](Message* msg)
	{
		if (msg->id == FunctionMessage::ID)
		{
			std::cout << "Executed\t" << msg->id <<"\n";
			FunctionMessage* func = static_cast<FunctionMessage*>(msg);
			func->execute();
		}
	});

	game_data->getInputManager()->addAction("enter", ASGE::KEYS::KEY_ENTER);
	game_data->getInputManager()->addAction("enter", game_data->getInputManager()->gamepad_button_enter);

	game_data->getInputManager()->addAction("escape", ASGE::KEYS::KEY_ESCAPE);
	game_data->getInputManager()->addAction("escape", game_data->getInputManager()->gamepad_button_escape);

	game_data->getInputManager()->addAction("up", ASGE::KEYS::KEY_UP);
	game_data->getInputManager()->addAction("up", ASGE::KEYS::KEY_W);
	game_data->getInputManager()->addAction("up", game_data->getInputManager()->gamepad_button_up);

	game_data->getInputManager()->addAction("down", ASGE::KEYS::KEY_DOWN);
	game_data->getInputManager()->addAction("down", ASGE::KEYS::KEY_S);
	game_data->getInputManager()->addAction("down", game_data->getInputManager()->gamepad_button_down);

	return true;
}

void BirdmanTheGame::update(const ASGE::GameTime& gt)
{
	//Sleep if our FPS is too high so we avoid too much wasted CPU usage (2k+ fps)

	if (capFPS && 5 - gt.delta_time.count() > 0)
	{
		std::this_thread::sleep_for(
			std::chrono::duration<double, std::milli>(5 - gt.delta_time.count())
		);
	}

	game_data->getNetworkManager()->update();
	game_data->getMessageQueue()->processMessages(3ms);

	game_data->getInputManager()->update();
	game_data->getStateManager()->update(gt);

	if (game_data->getInputManager()->isKeyPressed(ASGE::KEYS::KEY_F1))
	{
		toggleFullscreen();
	}
	
	if (game_data->getInputManager()->isKeyPressed(ASGE::KEYS::KEY_F2))
	{
		toggleFPS();
	}

	if (game_data->getInputManager()->isKeyPressed(ASGE::KEYS::KEY_F3))
	{
		capFPS = !capFPS;
		std::cout << "capFPS = " << capFPS << "\n";
	}

	if (renderer->exit() || this->exit || game_data->exit || game_data->getStateManager()->empty())
	{
		signalExit();
	}
}

void BirdmanTheGame::render(const ASGE::GameTime& gt)
{
	game_data->getStateManager()->render();
}

void BirdmanTheGame::keyHandler(const ASGE::SharedEventData data)
{
	const auto key_event = static_cast<const ASGE::KeyEvent*>(data.get());

	game_data->getInputManager()->handleInput(key_event->key, key_event->action);
}

void BirdmanTheGame::toggleFullscreen()
{
	if (renderer->getWindowMode() == ASGE::Renderer::WindowMode::WINDOWED)
	{
		renderer->setWindowedMode(ASGE::Renderer::WindowMode::FULLSCREEN);
	}
	else
	{
		renderer->setWindowedMode(ASGE::Renderer::WindowMode::WINDOWED);
	}
}