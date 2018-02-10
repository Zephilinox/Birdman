#include "StateManager.hpp"

//STD
#include <iostream>

//SELF
#include "../States/BaseState.hpp"
#include "../GameData.hpp"

StateManager::StateManager(GameData* game_data)
	: game_data(game_data)
{}

void StateManager::update(const ASGE::GameTime& gt)
{
	for (auto& f : delayed_calls)
	{
		f();
	}

	delayed_calls.clear();

	if (states.empty())
	{
		return;
	}

	top()->update(gt);
}

void StateManager::render() const
{
	if (states.empty())
	{
		return;
	}

	if (top()->shouldRenderPreviousState() && states.size() >= 2)
	{
		states[states.size() - 2]->render();
	}

	top()->render();
}

/**
*   @brief   Returns the top state on the stack.
*   @details Returns the top state on the stack. Might not be the active state.
Will throw an exception if the stack is empty.
*   @return  pointer to a BaseState */
BaseState* StateManager::top() const
{
	if (states.empty())
	{
		throw std::runtime_error("StateManager::top - Stack is empty");
	}

	return states.back().get();
}

/**
*   @brief   Destroys the top state on the stack.
*   @details Destroys the top state on the stack. Might not be the active state.
Will throw an exception if the stack is empty.
Calls onActive() after popping
*   @return  void */
void StateManager::pop()
{
	delayed_calls.push_back(
	[&]()
	{
		if (states.empty())
		{
			throw std::runtime_error("StateManager::pop - Stack is empty");
		}

		std::cout << "popped state\n";

		states.pop_back();

		if (!states.empty())
		{
			states.back()->onActive();
		}
	});
}

bool StateManager::empty() const
{
	return states.empty();
}
