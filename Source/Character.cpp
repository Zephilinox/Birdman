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
		horizontal_walk_sprite.addFrame(charTextureStrings[4], 0.250f, 0.0f, 0.0f);
		horizontal_walk_sprite.addFrame(charTextureStrings[5], 0.250f, 0.0f, 0.0f);
		horizontal_walk_sprite.addFrame(charTextureStrings[6], 0.25f, 0.0f, 0.0f);
		horizontal_walk_sprite.addFrame(charTextureStrings[5], 0.250f, 0.0f, 0.0f);


		//forward
		forward_walk_sprite.addFrame(charTextureStrings[7], 0.25f, 0.0f, 0.0f);
		forward_walk_sprite.addFrame(charTextureStrings[8], 0.25f, 0.0f, 0.0f);
		forward_walk_sprite.addFrame(charTextureStrings[9], 0.25f, 0.0f, 0.0f);

		//backward
		backward_walk_sprite.addFrame(charTextureStrings[10], 0.25f, 0.0f, 0.0f);
		backward_walk_sprite.addFrame(charTextureStrings[11], 0.25f, 0.0f, 0.0f);
		backward_walk_sprite.addFrame(charTextureStrings[12], 0.25f, 0.0f, 0.0f);
		break;
	}
	case Play::SceneCharacters::MIKE:
	{
		idle_sprite_forward = rend->createUniqueSprite();
		idle_sprite_forward->loadTexture(charTextureStrings[13]);

		idle_sprite_back = rend->createUniqueSprite();
		idle_sprite_back->loadTexture(charTextureStrings[14]);

		idle_sprite_left = rend->createUniqueSprite();
		idle_sprite_left->loadTexture(charTextureStrings[15]);

		idle_sprite_right = rend->createUniqueSprite();
		idle_sprite_right->loadTexture(charTextureStrings[16]);

		//left to right
		horizontal_walk_sprite.addFrame(charTextureStrings[17], 0.250f, 0.0f, 0.0f);
		horizontal_walk_sprite.addFrame(charTextureStrings[18], 0.250f, 0.0f, 0.0f);
		horizontal_walk_sprite.addFrame(charTextureStrings[19], 0.25f, 0.0f, 0.0f);
		horizontal_walk_sprite.addFrame(charTextureStrings[18], 0.250f, 0.0f, 0.0f);


		//forward
		forward_walk_sprite.addFrame(charTextureStrings[20], 0.25f, 0.0f, 0.0f);
		forward_walk_sprite.addFrame(charTextureStrings[21], 0.25f, 0.0f, 0.0f);
		forward_walk_sprite.addFrame(charTextureStrings[22], 0.25f, 0.0f, 0.0f);

		//backward
		backward_walk_sprite.addFrame(charTextureStrings[23], 0.25f, 0.0f, 0.0f);
		backward_walk_sprite.addFrame(charTextureStrings[24], 0.25f, 0.0f, 0.0f);
		backward_walk_sprite.addFrame(charTextureStrings[25], 0.25f, 0.0f, 0.0f);
		break;
	}
	case Play::SceneCharacters::LAURA:
	{
		idle_sprite_forward = rend->createUniqueSprite();
		idle_sprite_forward->loadTexture(charTextureStrings[26]);

		idle_sprite_back = rend->createUniqueSprite();
		idle_sprite_back->loadTexture(charTextureStrings[27]);

		idle_sprite_left = rend->createUniqueSprite();
		idle_sprite_left->loadTexture(charTextureStrings[28]);

		idle_sprite_right = rend->createUniqueSprite();
		idle_sprite_right->loadTexture(charTextureStrings[29]);

		//left to right
		horizontal_walk_sprite.addFrame(charTextureStrings[30], 0.250f, 0.0f, 0.0f);
		horizontal_walk_sprite.addFrame(charTextureStrings[31], 0.250f, 0.0f, 0.0f);
		horizontal_walk_sprite.addFrame(charTextureStrings[32], 0.25f, 0.0f, 0.0f);
		horizontal_walk_sprite.addFrame(charTextureStrings[31], 0.250f, 0.0f, 0.0f);


		//forward
		forward_walk_sprite.addFrame(charTextureStrings[33], 0.25f, 0.0f, 0.0f);
		forward_walk_sprite.addFrame(charTextureStrings[34], 0.25f, 0.0f, 0.0f);
		forward_walk_sprite.addFrame(charTextureStrings[35], 0.25f, 0.0f, 0.0f);

		//backward
		backward_walk_sprite.addFrame(charTextureStrings[36], 0.25f, 0.0f, 0.0f);
		backward_walk_sprite.addFrame(charTextureStrings[37], 0.25f, 0.0f, 0.0f);
		backward_walk_sprite.addFrame(charTextureStrings[38], 0.25f, 0.0f, 0.0f);
		break;
	}	
	case Play::SceneCharacters::LESLIE:
	{
		idle_sprite_forward = rend->createUniqueSprite();
		idle_sprite_forward->loadTexture(charTextureStrings[39]);

		idle_sprite_back = rend->createUniqueSprite();
		idle_sprite_back->loadTexture(charTextureStrings[40]);

		idle_sprite_left = rend->createUniqueSprite();
		idle_sprite_left->loadTexture(charTextureStrings[41]);

		idle_sprite_right = rend->createUniqueSprite();
		idle_sprite_right->loadTexture(charTextureStrings[42]);

		//left to right
		horizontal_walk_sprite.addFrame(charTextureStrings[43], 0.250f, 0.0f, 0.0f);
		horizontal_walk_sprite.addFrame(charTextureStrings[44], 0.250f, 0.0f, 0.0f);
		horizontal_walk_sprite.addFrame(charTextureStrings[45], 0.25f, 0.0f, 0.0f);
		horizontal_walk_sprite.addFrame(charTextureStrings[44], 0.250f, 0.0f, 0.0f);


		//forward
		forward_walk_sprite.addFrame(charTextureStrings[46], 0.25f, 0.0f, 0.0f);
		forward_walk_sprite.addFrame(charTextureStrings[47], 0.25f, 0.0f, 0.0f);
		forward_walk_sprite.addFrame(charTextureStrings[48], 0.25f, 0.0f, 0.0f);

		//backward
		backward_walk_sprite.addFrame(charTextureStrings[49], 0.25f, 0.0f, 0.0f);
		backward_walk_sprite.addFrame(charTextureStrings[50], 0.25f, 0.0f, 0.0f);
		backward_walk_sprite.addFrame(charTextureStrings[51], 0.25f, 0.0f, 0.0f);
		break;
	}	
	default:
		break;
	}

	float wid = idle_sprite_forward->width() * 3.0f;
	float hite = idle_sprite_forward->height() * 3.0f;

	//UPSCALE
	idle_sprite_forward->width(wid);
	idle_sprite_forward->height(hite);

	idle_sprite_right->width(wid);
	idle_sprite_right->height(hite);

	idle_sprite_back->width(wid);
	idle_sprite_back->height(hite);

	idle_sprite_left->width(wid);
	idle_sprite_left->height(hite);


}

void Character::setPosition(float x, float y)
{
	x_position = x;
	y_position = y;
}

void Character::slowMoveToPosition(float x, float y)
{
	char_state = CharacterState::WALKING;
	horizontal_walk_sprite.play();
	backward_walk_sprite.play();
	forward_walk_sprite.play();
	move_speed = 3.0f;
	target_x_position = x;
	target_y_position = y;
}

void Character::fastMoveToPosition(float x, float y)
{
	char_state = CharacterState::WALKING;
	horizontal_walk_sprite.play();
	backward_walk_sprite.play();
	forward_walk_sprite.play();
	move_speed = 5.0f;
	target_x_position = x;
	target_y_position = y;
}

void Character::loadCharacterTextureStrings()
{
	//TODO rename folders to follow actor names?
	//RIGGAN - IDLE
	charTextureStrings[0] = "../../Resources/Textures/Clint/Forwards/ClintF1.png";
	charTextureStrings[1] = "../../Resources/Textures/Clint/Backwards/ClintB1.png";
	charTextureStrings[2] = "../../Resources/Textures/Clint/Left Turn/ClintL1.png";
	charTextureStrings[3] = "../../Resources/Textures/Clint/Right Turn/ClintR1.png";

	//RIGGAN WALKING - RIGHT
	charTextureStrings[4] = "Clint/Right Turn/ClintR2";
	charTextureStrings[5] = "Clint/Right Turn/ClintR3";
	charTextureStrings[6] = "Clint/Right Turn/ClintR4";

	//RIGGAN WALKING - FORWARD
	charTextureStrings[7] = "Clint/Forwards/ClintF2";
	charTextureStrings[8] = "Clint/Forwards/ClintF3";
	charTextureStrings[9] = "Clint/Forwards/ClintF4";

	//RIGGAN WALKING = BACKWARDS
	charTextureStrings[10] = "Clint/Backwards/ClintB2";
	charTextureStrings[11] = "Clint/Backwards/ClintB3";
	charTextureStrings[12] = "Clint/Backwards/ClintB4";


	//Mike - IDLE
	charTextureStrings[13] = "../../Resources/Textures/Mike/Forwards/F1.png";
	charTextureStrings[14] = "../../Resources/Textures/Mike/Backwards/B1.png";
	charTextureStrings[15] = "../../Resources/Textures/Mike/LeftTurn/L1.png";
	charTextureStrings[16] = "../../Resources/Textures/Mike/RightTurn/R1.png";

	//MIKE WALKING - RIGHT
	charTextureStrings[17] = "Mike/RightTurn/R2";
	charTextureStrings[18] = "Mike/RightTurn/R3";
	charTextureStrings[19] = "Mike/RightTurn/R4";

	//MIKE WALKING - FORWARD
	charTextureStrings[20] = "Mike/Forwards/F2";
	charTextureStrings[21] = "Mike/Forwards/F3";
	charTextureStrings[22] = "Mike/Forwards/F4";

	//MIKE WALKING = BACKWARDS
	charTextureStrings[23] = "Mike/Backwards/B2";
	charTextureStrings[24] = "Mike/Backwards/B3";
	charTextureStrings[25] = "Mike/Backwards/B4";

	//LAURA
	//LAURA - IDLE
	charTextureStrings[26] = "../../Resources/Textures/Laura/Forwards/F1.png";
	charTextureStrings[27] = "../../Resources/Textures/Laura/Backwards/B1.png";
	charTextureStrings[28] = "../../Resources/Textures/Laura/LeftTurn/L1.png";
	charTextureStrings[29] = "../../Resources/Textures/Laura/RightTurn/R1.png";

	//LAURA WALKING - RIGHT
	charTextureStrings[30] = "Laura/RightTurn/R2";
	charTextureStrings[31] = "Laura/RightTurn/R3";
	charTextureStrings[32] = "Laura/RightTurn/R4";

	//LAURA WALKING - FORWARD
	charTextureStrings[33] = "Laura/Forwards/F2";
	charTextureStrings[34] = "Laura/Forwards/F3";
	charTextureStrings[35] = "Laura/Forwards/F4";

	//LAURA WALKING = BACKWARDS
	charTextureStrings[36] = "Laura/Backwards/B2";
	charTextureStrings[37] = "Laura/Backwards/B3";
	charTextureStrings[38] = "Laura/Backwards/B4";

	//LESLIE
	//LESLIE - IDLE
	charTextureStrings[39] = "../../Resources/Textures/Leslie/Forwards/F1.png";
	charTextureStrings[40] = "../../Resources/Textures/Leslie/Backwards/B1.png";
	charTextureStrings[41] = "../../Resources/Textures/Leslie/LeftTurn/L1.png";
	charTextureStrings[42] = "../../Resources/Textures/Leslie/RightTurn/R1.png";

	//LESLIE WALKING - RIGHT
	charTextureStrings[43] = "Leslie/RightTurn/R2";
	charTextureStrings[44] = "Leslie/RightTurn/R3";
	charTextureStrings[45] = "Leslie/RightTurn/R4";

	//LESLIE WALKING - FORWARD
	charTextureStrings[46] = "Leslie/Forwards/F2";
	charTextureStrings[47] = "Leslie/Forwards/F3";
	charTextureStrings[48] = "Leslie/Forwards/F4";

	//LESLIE WALKING = BACKWARDS
	charTextureStrings[49] = "Leslie/Backwards/B2";
	charTextureStrings[50] = "Leslie/Backwards/B3";
	charTextureStrings[51] = "Leslie/Backwards/B4";

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

	//update anim sprite poses
	forward_walk_sprite.xPos = x_position;
	backward_walk_sprite.xPos = x_position;
	horizontal_walk_sprite.xPos = x_position;

	forward_walk_sprite.yPos = y_position;
	backward_walk_sprite.yPos = y_position;
	horizontal_walk_sprite.yPos = y_position;

	switch(char_state)
	{
	case IDLE:
		{
		break;
		}

	case WALKING:
		{

		forward_walk_sprite.update(dt);
		backward_walk_sprite.update(dt);
		horizontal_walk_sprite.update(dt);

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

				horizontal_walk_sprite.restart();
				horizontal_walk_sprite.pause();
				backward_walk_sprite.restart();
				backward_walk_sprite.pause();
				forward_walk_sprite.restart();
				forward_walk_sprite.pause();
			}

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
