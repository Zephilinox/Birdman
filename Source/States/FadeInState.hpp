#pragma once

//STD
#include <memory>

//LIB
#include <Engine/Sprite.h>

//SELF
#include "../Architecture/States/BaseState.hpp"

/**
*  See BaseState for overriden functions
*/
class FadeInState : public BaseState
{
public:
	FadeInState(GameData* game_data);
	void update(const ASGE::GameTime& gt) override final;
	void render() const override final;
	void onActive() override final;
	void onInactive() override final;

private:
	std::unique_ptr<ASGE::Sprite> black_screen;
};
