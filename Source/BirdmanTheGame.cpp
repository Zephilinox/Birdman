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
	game_data = std::make_unique<GameData>(renderer.get());
	game_data->window_width = game_width;
	game_data->window_height = game_height;

	setup();

	game_data->getStateManager()->push<SplashState>();
	game_data->getMessageQueue()->addListener([](Message* msg)
	{
		if (msg->message_id != "Message")
		{
			std::cout << "Processed " << msg->message_id << "\n";
		}
	});

	game_data->getMessageQueue()->addListener([](Message* msg)
	{
		if (msg->message_id == "Function")
		{
			std::cout << "Executed " << msg->message_id << "\n";
			FunctionMessage* func = static_cast<FunctionMessage*>(msg);
			func->execute();
		}
	});

	return true;
}

void BirdmanTheGame::setup()
{
	renderer->setWindowTitle("Birbie");
	float cArray[] = { 0.08f, 0.08f, 0.08f };
	renderer->setClearColour(cArray);
	renderer->setSpriteMode(ASGE::SpriteSortMode::BACK_TO_FRONT);
	game_data->renderer = renderer.get();
	game_data->font_manager = FontManager(renderer.get());
	game_data->getFontManager()->addFont("../../Resources/Fonts/DroidSansMono.ttf", "Default");
	game_data->getFontManager()->loadFont("Default", 40);
	game_data->getFontManager()->loadFont("Default", 24);
	key_handler_id = inputs->addCallbackFnc(ASGE::EventType::E_KEY, &BirdmanTheGame::keyHandler, this);
}

void BirdmanTheGame::update(const ASGE::GameTime& gt)
{
	game_data->getMessageQueue()->processMessages(6ms);

	game_data->getInputManager()->update();
	game_data->getStateManager()->update(gt);

	if (game_data->getInputManager()->isKeyPressed(ASGE::KEYS::KEY_F1))
	{
		if (game_width != 1920)
		{
			game_width = 1920;
			game_height = 1080;
			game_data->window_width = game_width;
			game_data->window_height = game_height;

			if (!initAPI(ASGE::Renderer::WindowMode::FULLSCREEN))
			{
				throw "uhoh";
			}
		}
		else
		{
			game_width = 1280;
			game_height = 720;
			game_data->window_width = game_width;
			game_data->window_height = game_height;

			if (!initAPI(ASGE::Renderer::WindowMode::WINDOWED))
			{
				throw "uhoh";
			}
		}

		setup();
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
