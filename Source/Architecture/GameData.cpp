#include "GameData.hpp"

//STD
#include <assert.h>

//TODO ricardo
//Ricardo - do we add scene manager to this list?????
	//You only need to add it if you need to pass something to the scene managers constructor
	//For instance, the state_manager needs a pointer to game_data, so *this* is passed
	//Where as the font_manager just needs a pointer to ASGE::Renderer, so *renderer* is passed
GameData::GameData(ASGE::Renderer* renderer)
	: renderer(renderer)
	, state_manager(this)
	, font_manager(renderer)
	, audio_manager(AudioManager::Engine::SFML, "Resources/Sounds/")
{
	assert(renderer);
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
