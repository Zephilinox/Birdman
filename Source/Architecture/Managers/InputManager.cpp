#include "InputManager.hpp"

/**
*   @brief   Constructor.
*   @details Initializes the recorded state of all keys
*/
InputManager::InputManager()
{
	for (int i = 0; i < ASGE::KEYS::KEY_LAST; ++i)
	{
		//Create & reserve the map with all valid keys
		toggle_keys[i] = ASGE::KEYS::KEY_RELEASED;
		keys[i] = ASGE::KEYS::KEY_RELEASED;
	}
}


void InputManager::update()
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

/**
*   @brief   Updates the recorded state of the key.
*   @details Should be called by a function registered as a callback via ASGE::Input.
Or as a direct callback itself.
*   @param   key is the key that will be changed
*   @param   state is the new state of the key, such as ASGE::KEYS::KEY_PRESSED
*   @return void*/
void InputManager::handleInput(int key, int state)
{
	std::lock_guard<std::mutex> guard(keys_mutex);
	toggle_keys[key] = state;
	keys[key] = state;
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
bool InputManager::isKeyDown(int key)
{
	//Returns true until the key is released
	return (toggle_keys[key] != ASGE::KEYS::KEY_RELEASED ||
		keys[key] != ASGE::KEYS::KEY_RELEASED);
}
