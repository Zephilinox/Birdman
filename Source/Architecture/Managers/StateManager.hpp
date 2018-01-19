#pragma once

//STL
#include <memory>
#include <vector>

//LIB
#include "Engine/GameTime.h"

class BaseState;
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
	template <class T> T* push();

	BaseState* top();
	bool empty() const;

private:
	GameData* game_data;
	std::shared_ptr<BaseState> current_state;
	std::shared_ptr<BaseState> previous_state;
	std::vector<std::shared_ptr<BaseState>> states;
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
T* StateManager::push()
{
	if (!states.empty())
	{
		states.back()->onInactive();
	}

	std::shared_ptr<T> state = std::make_shared<T>(game_data);
	states.push_back(state);
	return state.get();
}
