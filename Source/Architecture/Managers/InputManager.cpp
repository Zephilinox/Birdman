#include "InputManager.hpp"

//STD
#include <cassert>

//LIB
#include <ini_parser.hpp>

/**
*   @brief   Constructor.
*   @details Initializes the recorded state of all keys
*/
InputManager::InputManager(ASGE::Input* input) noexcept
	: input(input)
{
	toggle_keys.fill(ASGE::KEYS::KEY_RELEASED);
	keys.fill(ASGE::KEYS::KEY_RELEASED);

	buttons_last_frame.fill(ASGE::KEYS::KEY_RELEASED);
	buttons.fill(ASGE::KEYS::KEY_RELEASED);

	callback_id = input->addCallbackFnc(ASGE::EventType::E_GAMEPAD_STATUS, &InputManager::gamepadHandler, this);

	ini_parser parser("settings.ini");

	try
	{
		gamepad_button_up = parser.get_int("GamePadUp");
		gamepad_button_down = parser.get_int("GamePadDown");
		gamepad_button_enter = parser.get_int("GamePadEnter");
		gamepad_button_escape = parser.get_int("GamePadEscape");
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << "\n";
	}
}

InputManager::~InputManager()
{
	input->unregisterCallback(callback_id);
}


void InputManager::update()
{
	{
		std::lock_guard<std::mutex> guard(keys_mutex);
		for (auto& key : keys)
		{
			//If the key hasn't been released since the last update
			//Then set it to repeated so that
			//KeyDown returns true but 
			//KeyPressed returns false
			if (key == ASGE::KEYS::KEY_PRESSED)
			{
				key = ASGE::KEYS::KEY_REPEATED;
			}
		}
	}

	auto game_pad = getGamePad();

	//http://www.glfw.org/docs/3.3/group__gamepad__buttons.html ?
	if (game_pad.is_connected)
	{
		assert(game_pad.no_of_buttons < ASGE::KEYS::KEY_LAST);

		for (int i = 0; i < game_pad.no_of_buttons; ++i)
		{
			buttons_last_frame[i] = buttons[i];
			buttons[i] = game_pad.buttons[i];
		}
	}

	//todo remove, only for testing
	for (int i = 0; i < game_pad.no_of_buttons; ++i)
	{
		if (isGamePadButtonPressed(i))
		{
			if (game_pad.buttons[i])
			{
				std::cout << "GamePad Button Pressed: ID " << i << "\n";
			}
		}
	}
}

/**
*   @brief   Updates the recorded state of the key.
*   @details Should be called by a function registered as a callback via ASGE::Input.
Or as a direct callback itself.
*   @param   key is the key that will be changed
*   @param   state is the new state of the key, such as ASGE::KEYS::KEY_PRESSED
*   @return void*/
void InputManager::handleInput(int key, int state)
{
	//Fixes Alt+PrintScreen
	if (key >= 0)
	{
		std::lock_guard<std::mutex> guard(keys_mutex);
		toggle_keys[key] = state;
		keys[key] = state;
	}
}


/**
*   @brief   Determines if the key was pressed this frame
*   @param   key is the key whose state is being checked
*   @return  true if the key was pressed since the last call to update. */
bool InputManager::isKeyPressed(int key)
{
	if (toggle_keys[key] == ASGE::KEYS::KEY_PRESSED)
	{
		std::lock_guard<std::mutex> guard(keys_mutex);
		toggle_keys[key] = ASGE::KEYS::KEY_RELEASED;
		keys[key] = ASGE::KEYS::KEY_PRESSED;
		return true;
	}

	return (keys[key] == ASGE::KEYS::KEY_PRESSED);
}


/**
*   @brief   Determines if the key is being held down
*   @param   key is the key whose state is being checked
*   @return  true if the key is being held down. */
bool InputManager::isKeyDown(int key) noexcept
{
	//Returns true until the key is released
	[[gsl::suppress(bounds.2)]]
	return (toggle_keys[key] != ASGE::KEYS::KEY_RELEASED ||
		keys[key] != ASGE::KEYS::KEY_RELEASED);
}

bool InputManager::isGamePadButtonPressed(int button)
{
	return buttons_last_frame[button] == ASGE::KEYS::KEY_RELEASED &&
		buttons_last_frame[button] != buttons[button];
}

bool InputManager::isGamePadButtonDown(int button)
{
	return buttons[button] == ASGE::KEYS::KEY_PRESSED;
}

GamePadData InputManager::getGamePad()
{
	//?
	for (int i = 0; i < 1; ++i)
	{
		auto gamepad = input->getGamePad(i);
		if (gamepad.is_connected)
		{
			return gamepad;
		}
	}

	GamePadData gamepad(input->getGamePad(0));
	return gamepad;
}

void InputManager::gamepadHandler(const ASGE::SharedEventData data)
{
	const auto gamepad_event = static_cast<const ASGE::GamePadEvent*>(data.get());

	//todo: what's the point of this?
	std::cout << "Gamepad Event Received (is connected) " << gamepad_event->connected;
}
