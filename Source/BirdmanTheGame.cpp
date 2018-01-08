#include <Engine\InputEvents.h>

#include "BirdmanTheGame.h"
#include "GameFont.h"

namespace {
	const float BACKGROUND_LAYER = 0.0f;
	const float FOREGROUND_LAYER = 1.0f;
};

BirdmanTheGame::~BirdmanTheGame()
{
	this->inputs->unregisterCallback(key_handler_id);
	LoadedGameFont::loaded_fonts.clear();
}

bool BirdmanTheGame::init()
{
	game_width = 844;
	game_height = 476;

	if (!initAPI(ASGE::Renderer::WindowMode::WINDOWED))
	{
		return false;
	}

	renderer->setWindowTitle("Endless Runner");
	renderer->setClearColour(ASGE::COLOURS::BLACK);
	renderer->setSpriteMode(ASGE::SpriteSortMode::BACK_TO_FRONT);
	toggleFPS();

	this->inputs->use_threads = false;
	
	key_handler_id = this->inputs->addCallbackFnc(
		ASGE::EventType::E_KEY, &BirdmanTheGame::keyHandler, this);

	LoadedGameFont::loaded_fonts.reserve(5);

	backdrop = renderer->createUniqueSprite();
	if (!backdrop->loadTexture(".\\Resources\\Textures\\Backdrop.jpg"))
	{
		return false;
	}

	return true;
}

void BirdmanTheGame::update(const ASGE::GameTime& ms)
{

}

void BirdmanTheGame::render(const ASGE::GameTime& ms)
{
	renderer->renderSprite(*backdrop, BACKGROUND_LAYER);
}

void BirdmanTheGame::keyHandler(const ASGE::SharedEventData data)
{
	const ASGE::KeyEvent* key_event =
		static_cast<const ASGE::KeyEvent*>(data.get());

	auto key = key_event->key;
	auto action = key_event->action;

	if (key == ASGE::KEYS::KEY_ESCAPE)
	{
		signalExit();
	}
}
