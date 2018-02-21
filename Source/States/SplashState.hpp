#pragma once

//SELF
#include "../Architecture/States/BaseState.hpp"
#include "../Architecture/Timer.hpp"

class GameData;

/**
*  See BaseState for overriden functions
*/
class SplashState : public BaseState
{
public:
	SplashState(GameData* game_data);

	void update(const ASGE::GameTime&) override final;
	void render() const override final;
	void onActive() override final;
	void onInactive() override final;

private:
	Timer timer;
};
