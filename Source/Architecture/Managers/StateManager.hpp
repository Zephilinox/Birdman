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
	//todo: implement generic delayed calls
	std::vector<std::function<void()>> delayed_calls;
};

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