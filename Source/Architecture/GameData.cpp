#include "GameData.hpp"

//STD
#include <assert.h>

//LIB
#include <ini_parser.hpp>

//SELF
#include "Managers/FontManager.hpp"
#include "Managers/AudioManager.hpp"
#include "Managers/NetworkManager.hpp"

ASGE::Renderer*					GameData::renderer;
std::unique_ptr<InputManager>	GameData::input;
std::unique_ptr<StateManager>	GameData::states;
std::unique_ptr<FontManager>	GameData::fonts;
std::unique_ptr<MessageQueue>	GameData::message_queue;
std::unique_ptr<AudioManager>	GameData::audio;
std::unique_ptr<NetworkManager>	GameData::network;
std::unique_ptr<Rng>			GameData::RNG;

int GameData::window_width = 0;
int GameData::window_height = 0;

bool GameData::initialized = false;

void GameData::initialize(ASGE::Renderer* render, int width, int height)
{
	assert(!initialized);
	assert(render);
	initialized = true;

	renderer = render;
	input = std::move(std::make_unique<InputManager>());
	states = std::move(std::make_unique<StateManager>());
	fonts = std::move(std::make_unique<FontManager>());
	message_queue = std::move(std::make_unique<MessageQueue>());
	audio = std::move(std::make_unique<AudioManager>("Resources/Sounds/"));
	network = std::move(std::make_unique<NetworkManager>());
	RNG = std::move(std::make_unique<Rng>());
	
	window_width = width;
	window_height = height;

	ini_parser ini("settings.ini");
	try
	{
		audio->setEngineType(ini.get_int("AudioEngine"));
	}
	catch (std::runtime_error& e)
	{
		std::cout << "ERROR: Setting 'AudioEngine' not found in 'settings.ini'.\n";
		std::cout << "ERROR INFO: " << e.what() << "\n";
	}
}

ASGE::Renderer* GameData::getRenderer() noexcept
{
	return renderer;
}

InputManager* GameData::getInput() noexcept
{
	return input.get();
}

FontManager* GameData::getFonts() noexcept
{
	return fonts.get();
}

MessageQueue* GameData::getMessageQueue() noexcept
{
	return message_queue.get();
}

StateManager* GameData::getStates() noexcept
{
	return states.get();
}

AudioManager* GameData::getAudio() noexcept
{
	return audio.get();
}

Rng* GameData::getRNG() noexcept
{
	return RNG.get();
}

NetworkManager* GameData::getNetwork() noexcept
{
	return network.get();
}

int GameData::getWindowWidth() noexcept
{
	return window_width;
}

int GameData::getWindowHeight() noexcept
{
	return window_height;
}