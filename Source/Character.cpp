#include "Character.h"
#include "Play.hpp"
#include <Engine\Renderer.h>



Character::Character(ASGE::Renderer* rend) : 
	horizontal_walk_sprite(rend, true),
	forward_walk_sprite(rend, true),
	backward_walk_sprite(rend, true)
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

		//left to right
		horizontal_walk_sprite.addFrame(charTextureStrings[4], 250.0f, 0.0f, 0.0f);
		horizontal_walk_sprite.addFrame(charTextureStrings[5], 250.0f, 0.0f, 0.0f);
		horizontal_walk_sprite.addFrame(charTextureStrings[6], 250.0f, 0.0f, 0.0f);

		//forward
		forward_walk_sprite.addFrame(charTextureStrings[7], 250.0f, 0.0f, 0.0f);
		forward_walk_sprite.addFrame(charTextureStrings[8], 250.0f, 0.0f, 0.0f);
		forward_walk_sprite.addFrame(charTextureStrings[9], 250.0f, 0.0f, 0.0f);

		//backward
		backward_walk_sprite.addFrame(charTextureStrings[10], 250.0f, 0.0f, 0.0f);
		backward_walk_sprite.addFrame(charTextureStrings[11], 250.0f, 0.0f, 0.0f);
		backward_walk_sprite.addFrame(charTextureStrings[12], 250.0f, 0.0f, 0.0f);
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
	//RIGGAN IDLE
	charTextureStrings[0] = "../../Resources/Textures/Clint/Forwards/ClintF1.png";
	charTextureStrings[1] = "../../Resources/Textures/Clint/Backwards/ClingB1.png";
	charTextureStrings[2] = "../../Resources/Textures/Clint/LeftTurn/ClintL1.png";
	charTextureStrings[3] = "../../Resources/Textures/Clint/RightTurn/ClintR1.png";

	//RIGGAN WALKING - RIGHT
	charTextureStrings[4] = "../../Resources/Textures/Clint/RightTurn/ClintR2.png";
	charTextureStrings[5] = "../../Resources/Textures/Clint/RightTurn/ClintR3.png";
	charTextureStrings[6] = "../../Resources/Textures/Clint/RightTurn/ClintR4.png";

	//RIGGAN WALKING - FORWARD
	charTextureStrings[7] = "../../Resources/Textures/Clint/Forwards/ClintF2.png";
	charTextureStrings[8] = "../../Resources/Textures/Clint/Forwards/ClintF3.png";
	charTextureStrings[9] = "../../Resources/Textures/Clint/Forwards/ClintF4.png";

	//RIGGAN WALKING = BACKWARDS
	charTextureStrings[10] = "../../Resources/Textures/Clint/Backwards/ClintB2.png";
	charTextureStrings[11] = "../../Resources/Textures/Clint/Backwards/ClintB3.png";
	charTextureStrings[12] = "../../Resources/Textures/Clint/Backwards/ClintB4.png";

	//LESLEY
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

	forward_walk_sprite.update(dt);
	backward_walk_sprite.update(dt);
	horizontal_walk_sprite.update(dt);

	switch(char_state)
	{
	case IDLE:
		{
		//TODO - refactor to not be setting these all the time?
		horizontal_walk_sprite.restart();
		horizontal_walk_sprite.pause();
		backward_walk_sprite.restart();
		backward_walk_sprite.pause();
		forward_walk_sprite.restart();
		forward_walk_sprite.pause();

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

			horizontal_walk_sprite.play();
			backward_walk_sprite.play();
			forward_walk_sprite.play();
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
				break;
			}

			case WALKING:
			{
				switch(char_facing)
				{
					case NORTH:
					{
						renderer->renderSprite(*backward_walk_sprite.getCurrentFrameSprite());
						break;
					}
					case EAST:
					{
						renderer->renderSprite(*horizontal_walk_sprite.getCurrentFrameSprite());
						break;
					}
					case SOUTH:
					{
						renderer->renderSprite(*forward_walk_sprite.getCurrentFrameSprite());
						break;
					}
					case WEST:
					{
						renderer->renderSprite(*horizontal_walk_sprite.getCurrentFrameSprite());
						break;
					}
				}
				break;
			}

			default:
			{
				break;
			}
		}
}
