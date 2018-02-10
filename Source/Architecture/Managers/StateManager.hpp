#pragma once

//STL
#include <memory>
#include <vector>
#include <functional>

//LIB
#include "Engine/GameTime.h"

//SELF
#include "../Messages/MessageQueue.hpp"
#include "../States/BaseState.hpp"

class GameData;

/**
*  State Manager. A manager for the BaseState abstract class and its derivatives.
*/

class StateManager
{
public:
	StateManager(GameData* game_data);

	void update(const ASGE::GameTime& gt);
	void render() const;

	void pop();
	template <class T> void push();
	template <class T, class... Args> void push(Args... args);

	BaseState* top() const;
	bool empty() const;

private:
	GameData* game_data;
	std::vector<std::unique_ptr<BaseState>> states;
	std::vector<std::function<void()>> delayed_calls;
};

/**
*   @brief   Creates a new state
*   @details Creates a shared pointer to a derived class
of BaseState based on a template parameter
and then pushes it on to the stack.
Calls onInactive() before pushing
*   @tparam  T the state class to push
*   @return  the state that was pushed */
template <class T>
void StateManager::push()
{
	delayed_calls.push_back(
	[&]()
	{
		if (!states.empty())
		{
			states.back()->onInactive();
		}

		states.push_back(std::make_unique<T>(game_data));
	});
}

template <class T, class... Args>
void StateManager::push(Args... args)
{
	delayed_calls.push_back(
	[&, args...]()
	{
		if (!states.empty())
		{
			states.back()->onInactive();
		}

		states.push_back(std::make_unique<T>(game_data, args...));
	});
}