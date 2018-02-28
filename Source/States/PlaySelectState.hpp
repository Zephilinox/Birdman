#pragma once

//SELF
#include "../Architecture/States/BaseState.hpp"

class GameData;

/**
*  See BaseState for overriden functions
*/
class PlaySelectState : public BaseState
{
public:
	PlaySelectState();

	void update(const ASGE::GameTime&) override final;
	void render() const override final;
	void onActive() override final;
	void onInactive() override final;

private:
};
