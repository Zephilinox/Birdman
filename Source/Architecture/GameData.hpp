#pragma once

//LIB
#include <Engine/Renderer.h>

//SELF
#include "Managers/InputManager.hpp"
#include "Managers/StateManager.hpp"
#include "Managers/FontManager.hpp"
#include "Messages/MessageQueue.hpp"

class GameData
{
public:
	GameData(ASGE::Renderer* renderer);

	ASGE::Renderer* getRenderer() const;
	StateManager* getStateManager();
	InputManager* getInputManager();
	FontManager* getFontManager();
	MessageQueue* getMessageQueue();

private:
	ASGE::Renderer* renderer = nullptr;

	/** Input Manager. A wrapper around key states from callbacks by ASGE::Input. */
	InputManager input_manager;

	/** State Manager. A wrapper around storing states in a stack. */
	StateManager state_manager;

	FontManager font_manager;

	MessageQueue message_queue;
};
