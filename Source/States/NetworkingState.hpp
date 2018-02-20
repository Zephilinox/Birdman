#pragma once

//STD
#include <chrono>

//SELF
#include "../Architecture/States/BaseState.hpp"

class GameData;

/**
*  See BaseState for overriden functions
*/
class NetworkingState : public BaseState
{
public:
	NetworkingState(GameData* game_data);

	void update(const ASGE::GameTime&) override final;
	void render() const override final;
	void onActive() override final;
	void onInactive() override final;

private:
};
