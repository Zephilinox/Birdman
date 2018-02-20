#include "NetworkingState.hpp"
//SELF
#include "../Architecture/GameData.hpp"
#include "../States/MenuState.hpp"
#include "../States/FadeOutState.hpp"

NetworkingState::NetworkingState(GameData* game_data)
	: BaseState(game_data)
{
}

void NetworkingState::update(const ASGE::GameTime& gt)
{
}

void NetworkingState::render() const
{

}

void NetworkingState::onActive()
{
}

void NetworkingState::onInactive()
{
}