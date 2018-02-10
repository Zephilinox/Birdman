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
	if (states.empty())
	{
		current_state = nullptr;
		return;
	}

	current_state = top();
	current_state->update(gt);
}

void StateManager::render() const
{
	if (current_state)
	{
		if (current_state->shouldRenderPreviousState() && states.size() >= 2)
		{
			states[states.size() - 2]->render();
		}

		current_state->render();
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
	game_data->getMessageQueue()->sendMessage<CommandMessage>(
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
