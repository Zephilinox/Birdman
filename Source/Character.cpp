#include "Character.h"
#include "Play.hpp"
#include <Engine\Renderer.h>
#include <algorithm>
#include <cmath>


Character::Character()
{
	loadCharacterTextureStrings();
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

bool Character::getIsActive() const
{
	return isActive;
}

void Character::initCharacter(Play::SceneCharacters actor, ASGE::Renderer* rend)
{
	//TODO load textures appropriate to the character
	switch(actor)
	{
	case Play::SceneCharacters::RIGGAN:
	{
		idle_sprite_forward = rend->createUniqueSprite();
		idle_sprite_forward->loadTexture(charTextureStrings[0]);

		idle_sprite_back = rend->createUniqueSprite();
		idle_sprite_back->loadTexture(charTextureStrings[1]);

		idle_sprite_left = rend->createUniqueSprite();
		idle_sprite_left->loadTexture(charTextureStrings[2]);

		idle_sprite_right = rend->createUniqueSprite();
		idle_sprite_right->loadTexture(charTextureStrings[3]);
		break;
	}
	case Play::SceneCharacters::HANNAH:
	{
		break;
	}	
	case Play::SceneCharacters::JOHN:
	{
		break;
	}	
	case Play::SceneCharacters::LESLEY:
	{
		break;
	}	
	case Play::SceneCharacters::FISTO:
	{
		break;
	}
	default:
		break;
	}
}

void Character::setPosition(float x, float y)
{
	x_position = x;
	y_position = y;
}

void Character::slowMoveToPosition(float x, float y)
{
	char_state = CharacterState::WALKING;
	move_speed = 10.0f;
	target_x_position = x;
	target_y_position = y;
}

void Character::fastMoveToPosition(float x, float y)
{
	char_state = CharacterState::WALKING;
	move_speed = 25.0f;
	target_x_position = x;
	target_y_position = y;
}

void Character::loadCharacterTextureStrings()
{
	//TODO - maybe break this down so it's not such a fat function?
	//TODO rename folders to follow actor names?
	//RIGGAN
	charTextureStrings[0] = "../../Resources/Textures/Clint/Forwards/ClintF1.png";
	charTextureStrings[1] = "../../Resources/Textures/Clint/Backwards/ClingB1.png";
	charTextureStrings[2] = "../../Resources/Textures/Clint/LeftTurn/ClintL1.png";
	charTextureStrings[3] = "../../Resources/Textures/Clint/RightTurn/ClintR1.png";
	charTextureStrings[4] = "../../Resources/Textures/5.png";

	//LESLEY
	charTextureStrings[5] = "../../Resources/Textures/6.png";
	charTextureStrings[6] = "../../Resources/Textures/7.png";
	charTextureStrings[7] = "../../Resources/Textures/8.png";
	charTextureStrings[8] = "../../Resources/Textures/9.png";
	charTextureStrings[9] = "../../Resources/Textures/10.png";

	//JOHN etc...
}

void Character::update(float dt)
{

	//TODO - move this into function?
	//update all sprites positions to the same values
	idle_sprite_forward->xPos(x_position);
	idle_sprite_forward->yPos(y_position);
	idle_sprite_right->xPos(x_position);
	idle_sprite_right->yPos(y_position);
	idle_sprite_back->xPos(x_position);
	idle_sprite_back->yPos(y_position);
	idle_sprite_left->xPos(x_position);
	idle_sprite_left->yPos(y_position);


	switch(char_state)
	{
	case IDLE:
		{
			//character_sprite.update();
			break;
		}

	case WALKING:
		{
		bool xPosMatched = false;
		bool yPosMatched = false;
			if(x_position < target_x_position)
			{
				x_position += move_speed * dt;
			}
			else if(x_position > target_x_position)
			{
				x_position -= move_speed * dt;
			}
			else
			{
				xPosMatched = true;
			}

			if(y_position < target_y_position)
			{
				y_position += move_speed * dt;
			}
			else if(y_position > target_y_position)
			{
				y_position -= move_speed * dt;
			}
			else
			{
				yPosMatched = true;
			}

			if(yPosMatched && xPosMatched)
			{
				char_state = CharacterState::IDLE;
			}

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

void Character::render(ASGE::Renderer* renderer) const
{
		switch(char_state)
		{
			case IDLE:
			{
				//TODO - render current idle sprite
				//render idle sprite in current facing
				switch(char_facing)
				{
					case NORTH:
					{
						renderer->renderSprite(*idle_sprite_back);
						break;
					}
					case EAST:
					{
						renderer->renderSprite(*idle_sprite_right);
						break;
					}
					case SOUTH:
					{
						renderer->renderSprite(*idle_sprite_forward);
						break;
					}
					case WEST:
					{
						renderer->renderSprite(*idle_sprite_left);
						break;
					}
				}
				//renderer->renderSprite(*current_idle.lock());
				break;
			}

			case WALKING:
			{
				//render current frame of walking sprite

				//TODO REMOVE- USED ONLY IN TESTING MOVEMENT
				switch(char_facing)
				{
				case NORTH:
				{
					renderer->renderSprite(*idle_sprite_back);
					break;
				}
				case EAST:
				{
					renderer->renderSprite(*idle_sprite_right);
					break;
				}
				case SOUTH:
				{
					renderer->renderSprite(*idle_sprite_forward);
					break;
				}
				case WEST:
				{
					renderer->renderSprite(*idle_sprite_left);
					break;
				}
				}
				break;
			}
			case POINTGUN:
			{
				//render pointing gun frame
				break;
			}
			case SPEAKING:
			{
				//This may be handled in portrait?
				//render current frame of talking sprite
				break;
			}
			default:
			{
				break;
			}
		}
}
