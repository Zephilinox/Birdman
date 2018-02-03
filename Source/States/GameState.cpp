#include "GameState.hpp"

//SELF
#include "../Architecture/GameData.hpp"
#include "../Constants.hpp"

GameState::GameState(GameData* game_data)
	: BaseState(game_data)
{
	//add all the dialogue to the dialogue tree. each level would have its own dialogue tree
	dialogue_init();
}

void GameState::update(const ASGE::GameTime&)
{
}

void GameState::render() const
{
}

void GameState::onActive()
{
}

void GameState::onInactive()
{
}

void GameState::dialogue_init()
{
	//Kinda messy, but this might be the best way to do this depending on how big our dialogue branches become per level.
	//It's an option anyway, alternatively just copy+paste it all in to here, as it was before.
	//Note that visual studio can't help you here with what functions and objects might be available, so expect red squiggly lines.
	#include "../Dialogue/example.cpp"
}