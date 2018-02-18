#pragma once
#include <Engine\Sprite.h>
#include "Architecture\AnimatedSprite.hpp"


class Character
{
public:
	enum CharacterFacing
	{
		NORTH = 0,
		EAST,
		SOUTH,
		WEST
	};

	enum CharacterState
	{
		IDLE = 0,
		WALKING,
		POINTGUN,
		SPEAKING
	};

	Character();
	~Character();

	void setFacing(CharacterFacing new_facing);
	void setState(CharacterState new_state);

	void setIsActive(bool new_active);
	bool getIsActive();

	void update();
	void render(ASGE::Renderer* renderer);

private:
	bool isActive = false;

	//AnimatedSprite horizontal_walk_sprite;
	//AnimatedSprite vertical_walk_sprite;
	//AnimatedSprite talking_sprite;
	ASGE::Sprite* idle_sprite;
	ASGE::Sprite* point_gun;
	CharacterFacing char_facing = SOUTH;
	CharacterState char_state = IDLE;
};