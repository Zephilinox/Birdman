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
#include "../Messages/FunctionMessage.hpp"

class GameData;

class StateManager
{
public:
	StateManager() noexcept = default;

	void update(const ASGE::GameTime& gt);
	void render() const;

	void pop();
	template <class T> void push();
	template <class T, class... Args> void push(Args... args);

	BaseState* top() const;
	bool empty() const noexcept;

private:
	std::vector<std::unique_ptr<BaseState>> states;
};

template <class T>
void StateManager::push()
{
	GameData::getMessageQueue()->sendPriorityMessage<FunctionMessage>(
	[&]()
	{
		if (!states.empty())
		{
			states.back()->onInactive();
		}

		states.push_back(std::make_unique<T>());
	});
}

template <class T, class... Args>
void StateManager::push(Args... args)
{
	GameData::getMessageQueue()->sendPriorityMessage<FunctionMessage>(
	[&, args...]()
	{
		if (!states.empty())
		{
			states.back()->onInactive();
		}

		states.push_back(std::make_unique<T>(args...));
	});
}