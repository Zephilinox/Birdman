#include "GameData.hpp"

//STD
#include <assert.h>

//LIB
#include <ini_parser.hpp>

GameData::GameData(ASGE::Renderer* renderer)
	: game(game)
	, renderer(renderer)
	, state_manager(this)
	, font_manager(renderer)
	, audio_manager("Resources/Sounds/")
{
	assert(renderer);

	ini_parser ini("settings.ini");

	try
	{
		audio_manager.setEngineType(ini.get_int("AudioEngine"));
	}
	catch (std::runtime_error& e)
	{
		std::cout << "ERROR: Setting 'AudioEngine' not found in 'settings.ini'.\n";
		std::cout << "ERROR INFO: " << e.what() << "\n";
	}
}

ASGE::Renderer* GameData::getRenderer() const
{
	return renderer;
}

InputManager* GameData::getInputManager()
{
	return &input_manager;
}

FontManager* GameData::getFontManager()
{
	return &font_manager;
}

MessageQueue* GameData::getMessageQueue()
{
	return &message_queue;
}

StateManager* GameData::getStateManager()
{
	return &state_manager;
}

AudioManager* GameData::getAudioManager()
{
	return &audio_manager;
}

Rng * GameData::getRandomNumberGenerator()
{
	return &random_number_gen;
}

int GameData::getWindowWidth()
{
	return window_width;
}

int GameData::getWindowHeight()
{
	return window_height;
}