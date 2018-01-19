#include "GameData.hpp"
#include <assert.h>

GameData::GameData(ASGE::Renderer* renderer)
	: renderer(renderer)
	, state_manager(this)
	, font_manager(renderer)
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