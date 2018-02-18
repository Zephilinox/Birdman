#include "Character.h"


Character::Character()
{

}

Character::~Character()
{

}

void Character::setFacing(CharacterFacing new_facing)
{
	char_facing = new_facing;
}

void Character::setState(CharacterState new_state)
{
	char_state = new_state;
}

void Character::setIsActive(bool new_active)
{
	isActive = new_active;
}

bool Character::getIsActive()
{
	return isActive;
}

void Character::update()
{
	switch(char_facing)
	{
	case NORTH:
		{
			break;
		}
	case EAST:
		{
			break;
		}
	case SOUTH:
		{
			break;
		}
	case WEST:
		{
			break;
		}
	default:
		{
			break;
		}
	}

	switch(char_state)
	{
	case IDLE:
		{
			//character_sprite.update();
			break;
		}

	case WALKING:
		{
			if(char_facing == WEST || char_facing == EAST)
			{
				//TODO pass game_data delta time through to here?
				//horizontal_walk_sprite.update(double dt_milli);
			}
			else
			{
				//vertical_walk_sprite.update(double dt_milli);
			}
			break;
		}
	case POINTGUN:
		{
			break;
		}
	case SPEAKING:
		{
			//talking_sprite.update();
			break;
		}
	default:
		{
			break;
		}
	}
}

	void Character::render(ASGE::Renderer* renderer)
{
		switch(char_state)
		{
			case IDLE:
			{
				//render idle sprite in current facing
				break;
			}

			case WALKING:
			{
				//render current frame of walking sprite
				break;
			}
			case POINTGUN:
			{
				//render pointing gun frame
				break;
			}
			case SPEAKING:
			{
				//render current frame of talking sprite
				break;
			}
			default:
			{
				break;
			}
		}
}
