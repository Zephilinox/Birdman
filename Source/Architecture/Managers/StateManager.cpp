#include "StateManager.hpp"

//STD
#include <iostream>

//SELF
#include "../States/BaseState.hpp"

StateManager::StateManager(GameData* game_data)
	: game_data(game_data)
{
}

void StateManager::update(const ASGE::GameTime& gt)
{
	if (states.empty())
	{
		throw std::runtime_error("StateManager::update - Stack is empty");
	}

	current_state = states.back();

	if (states.size() >= 2 && current_state->shouldRenderPreviousState())
	{
		previous_state = states[states.size() - 2];
	}
	else
	{
		previous_state = nullptr;
	}

	current_state->update(gt);
}

void StateManager::render() const
{
	current_state->render();
	if (previous_state)
	{
		previous_state->render();
	}
}

/**
*   @brief   Returns the top state on the stack.
*   @details Returns the top state on the stack. Might not be the active state.
Will throw an exception if the stack is empty.
*   @return  pointer to a BaseState */
BaseState* StateManager::top()
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
	if (states.empty())
	{
		throw std::runtime_error("StateManager::pop - Stack is empty");
	}
	
	states.pop_back();

	if (!states.empty())
	{
		states.back()->onActive();
	}
}

bool StateManager::empty() const
{
	return states.empty();
}
