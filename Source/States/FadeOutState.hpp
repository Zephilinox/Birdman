#pragma once

//STD
#include <memory>
#include <chrono>
#include <functional>

//LIB
#include <Engine/Sprite.h>

//SELF
#include "../Architecture/States/BaseState.hpp"
#include "../Architecture/Timer.hpp"

class GameData;

/**
*  See BaseState for overriden functions
*/
class FadeOutState : public BaseState
{
public:
	FadeOutState(std::function<void()> callback);
	void update(const ASGE::GameTime& gt) override final;
	void render() const override final;
	void onActive() override final;
	void onInactive() override final;

private:
	std::unique_ptr<ASGE::Sprite> left_curtain;
	std::unique_ptr<ASGE::Sprite> right_curtain;
	bool closed = false;
	Timer timer;

	std::function<void()> fade_end_callback;
};
