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
	renderer->setClearColour(cArray);
	renderer->setSpriteMode(ASGE::SpriteSortMode::IMMEDIATE);
	toggleFPS();

	key_handler_id = inputs->addCallbackFnc(ASGE::EventType::E_KEY, &BirdmanTheGame::keyHandler, this);

	game_data = std::make_unique<GameData>(renderer.get(), game_width, game_height);
	game_data->getFontManager()->addFont("../../Resources/Fonts/DroidSansMono.ttf", "Default", 24);
	
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

	return true;
}

void BirdmanTheGame::update(const ASGE::GameTime& gt)
{
	//Sleep if our FPS is too high so we avoid too much wasted CPU usage (2k+ fps)
	if (5 - gt.delta_time.count() > 0)
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