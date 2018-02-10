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

	states.back()->update(gt);
}

void StateManager::render() const
{
	if (states.empty())
	{
		return;
	}

	size_t firstRenderState = 0;
	for (int i = states.size() - 1; i >= 0; --i)
	{
		if (!states[i]->shouldRenderPreviousState())
		{
			firstRenderState = i;
			break;
		}
	}

	for (size_t i = firstRenderState; i < states.size(); ++i)
	{
		states[i]->render();
	}
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
