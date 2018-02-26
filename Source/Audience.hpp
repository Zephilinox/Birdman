#pragma once
#include <Engine/Sprite.h>

namespace ASGE
{
	class Renderer;
	class Sprite;
}

class GameData;

constexpr unsigned int max_approval = 100;

class Audience
{
public:
	//not really necessary to make them default since we don't define anything, can exclude them
	//https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming) (/rule of five)
	Audience(GameData* data);
	~Audience() = default;

	int getSad();
	//humour?
	int getLight();
	//horror?
	int getDark();
	int getShocking();
	//could base it off emotion wheels? maybe too much effort though
	//https://en.wikipedia.org/wiki/Emotion#/media/File:Plutchik-wheel.svg
	//gotta work out how the audience is going to work first

	//todo: consider doing this in the constructor instead of having an init function
	void varyApprovalsBetweenNights();
	void loadAudienceSprite();

private:
	int approval_sad = 0;
	int approval_light = 0;
	int approval_dark = 0;
	int approval_shocking = 0;

	GameData* game_data;

	ASGE::Sprite* audience_sprite = nullptr;
};