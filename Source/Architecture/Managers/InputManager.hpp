#pragma once

//STD
#include <array>
#include <mutex>

//LIB
#include <Engine/Keys.h>

namespace ASGE
{
	namespace KEYS
	{
		/**< Extra key defines */
		constexpr int KEY_F1 = 290;
		constexpr int KEY_LAST = 512; /**< GLFW defines it as 348 however I want to be safe. */
	}
}

/**
* Input Manager. Handles input thread safety and different key states.
*/

/*
The input manager has some issues but I'm leaving it as it is for the time being.
Use isKeyDown over isKeyPressed if at all possible.
*/
class InputManager
{
public:
	InputManager() noexcept;

	void update();
	void handleInput(int key, int state);
	bool isKeyPressed(int key);
	bool isKeyDown(int key) noexcept;

private:
	std::array<int, ASGE::KEYS::KEY_LAST> toggle_keys;
	std::array<int, ASGE::KEYS::KEY_LAST> keys;
	std::mutex keys_mutex;
};
