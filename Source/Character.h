#pragma once
#include <Engine\Sprite.h>
#include "Architecture\AnimatedSprite.hpp"
#include "Play.hpp"

constexpr unsigned int number_of_characters = 5;

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
	bool getIsActive() const;
	void initCharacter(Play::SceneCharacters actor, ASGE::Renderer* rend);
	void setPosition(float x, float y);

	void slowMoveToPosition(float x, float y);
	void fastMoveToPosition(float x, float y);

	void loadCharacterTextureStrings();

	void update(float dt);
	void render(ASGE::Renderer* renderer) const;

private:

	float move_speed = 10.0f;

	bool isActive = false;

	std::string charTextureStrings[(number_of_characters * 4)];

	//std::weak_ptr<ASGE::Sprite> current_idle;

	float x_position = 0.0f;
	float y_position = 0.0f;
	float target_x_position = 300.0f;
	float target_y_position = 300.0f;

	//AnimatedSprite horizontal_walk_sprite;
	//AnimatedSprite vertical_walk_sprite;
	//AnimatedSprite talking_sprite;
	std::shared_ptr<ASGE::Sprite> idle_sprite_forward;
	std::shared_ptr<ASGE::Sprite> idle_sprite_right;
	std::shared_ptr<ASGE::Sprite> idle_sprite_back;
	std::shared_ptr<ASGE::Sprite> idle_sprite_left;
	ASGE::Sprite* point_gun;

	CharacterFacing char_facing = SOUTH;
	CharacterState char_state = IDLE;
};