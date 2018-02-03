#pragma once

//STD
#include <memory>

//LIB
#include <Engine/Sprite.h>

//SELF
#include "../Architecture/States/BaseState.hpp"
#include "GameState.hpp"

/**
*  See BaseState for overriden functions
*/
class FadeOutState : public BaseState
{
public:
	FadeOutState(GameData* game_data);
	void update(const ASGE::GameTime& gt) override final;
	void render() const override final;
	void onActive() override final;
	void onInactive() override final;

	void setFadeEndCallback(std::function<void(void)> func);

private:
	std::unique_ptr<ASGE::Sprite> black_screen;

	std::function<void(void)> fade_end_callback;
};
