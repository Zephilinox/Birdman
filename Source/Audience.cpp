#include "Audience.hpp"
#include "Rng.h"
#include "Architecture\GameData.hpp"

//STD
#include <random>

Audience::Audience(GameData* data)
{
	//TODO: move this to some kind of RNG class we can use anywhere
	game_data = data;

	sad_multiplier = game_data->getRandomNumberGenerator()->getRandomInt(1, max_approval);
	comedy_multiplier = game_data->getRandomNumberGenerator()->getRandomInt(1, max_approval);
	light_multiplier = game_data->getRandomNumberGenerator()->getRandomInt(1, max_approval);
	dark_multiplier = game_data->getRandomNumberGenerator()->getRandomInt(1, max_approval);

	panel_pos_x = 926.0f;
	panel_pos_y = 476.0f;
	sprite_width_override = 346.0f;
	sprite_height_override = 236.0f;

	loadAudienceSprites();
}

int Audience::getSadApproval()
{
	return sad_multiplier;
}

int Audience::getLightApproval()
{
	return light_multiplier;
}

int Audience::getDarkApproval()
{
	return dark_multiplier;
}

int Audience::getComedyApproval()
{
	return comedy_multiplier;
}

void Audience::varyApprovalsBetweenNights()
{
	float ten_percent_sad = sad_multiplier / 10.0f;
	float ten_percent_shocking = comedy_multiplier / 10.0f;
	float ten_percent_light = light_multiplier/10.0f;
	float ten_percent_dark = dark_multiplier / 10.0f;

	//TODO: move this to some kind of RNG class we can use anywhere
	std::random_device rd;
	std::mt19937 mt(rd());

	std::uniform_int_distribution<int> sad_variance(-ten_percent_sad, ten_percent_sad);
	std::uniform_int_distribution<int> shocking_variance(-ten_percent_shocking, ten_percent_shocking);
	std::uniform_int_distribution<int> light_variance(-ten_percent_light, ten_percent_light);
	std::uniform_int_distribution<int> dark_variance(-ten_percent_dark, ten_percent_dark);

	sad_multiplier += (int)sad_variance(mt);
	comedy_multiplier += (int)shocking_variance(mt);
	light_multiplier += (int)light_variance(mt);
	dark_multiplier += (int)dark_variance(mt);

}

void Audience::loadAudienceSprites()
{
	perfect_sprite = game_data->getRenderer()->createUniqueSprite();
	great_sprite = game_data->getRenderer()->createUniqueSprite();
	good_sprite = game_data->getRenderer()->createUniqueSprite();
	indifferent_sprite = game_data->getRenderer()->createUniqueSprite();
	

	perfect_sprite->loadTexture("../../Resources/Textures/Audience/Perfect.jpg");
	great_sprite->loadTexture("../../Resources/Textures/Audience/Excellent.jpg");
	good_sprite->loadTexture("../../Resources/Textures/Audience/Good.jpg");
	indifferent_sprite->loadTexture("../../Resources/Textures/Audience/Calm.jpg");

	perfect_sprite->xPos(panel_pos_x);
	perfect_sprite->yPos(panel_pos_y);

	great_sprite->xPos(panel_pos_x);
	great_sprite->yPos(panel_pos_y);

	good_sprite->xPos(panel_pos_x);
	good_sprite->yPos(panel_pos_y);

	indifferent_sprite->xPos(panel_pos_x);
	indifferent_sprite->yPos(panel_pos_y);

	perfect_sprite->width(sprite_width_override);
	perfect_sprite->height(sprite_height_override);

	great_sprite->width(sprite_width_override);
	great_sprite->height(sprite_height_override);

	good_sprite->width(sprite_width_override);
	good_sprite->height(sprite_height_override);

	indifferent_sprite->width(sprite_width_override);
	indifferent_sprite->height(sprite_height_override);
}


void Audience::update(float dt)
{
	//overallApproval = sad_approval + light_approval + dark_approval + comedy_approval;
	//overallApproval *= 0.25f;
	
	//TODO -remove and uncomment above - Testing only
	overallApproval += 10.0f * dt;

}

void Audience::render() const
{
	if(overallApproval >= 75.0f)
	{
		game_data->getRenderer()->renderSprite(*perfect_sprite);
	}
	else if(overallApproval >= 50.0f)
	{
		game_data->getRenderer()->renderSprite(*great_sprite);
	}
	else if(overallApproval >= 25.0f)
	{
		game_data->getRenderer()->renderSprite(*good_sprite);
	}
	else
	{
		game_data->getRenderer()->renderSprite(*indifferent_sprite);
	}
}

void Audience::setStartingApprovals(int starting_val)
{
	comedy_multiplier = starting_val;
	dark_multiplier = starting_val;
	light_multiplier = starting_val;
	sad_multiplier = starting_val;
}

float Audience::getOverallApproval()
{
	return overallApproval;
}

//Takes an int, applies it to the responsiveness/multiplier then adds it to the approval
void Audience::addToSad(int effect)
{
	sad_approval += effect * sad_multiplier;
}

//Takes an int, applies it to the responsiveness/multiplier then adds it to the approval
void Audience::addToLight(int effect)
{
	light_approval += effect * light_multiplier;
}

//Takes an int, applies it to the responsiveness/multiplier then adds it to the approval
void Audience::addToDark(int effect)
{
	dark_approval += effect * dark_multiplier;
}

//Takes an int, applies it to the responsiveness/multiplier then adds it to the approval
void Audience::addToComedy(int effect)
{
	comedy_approval += effect * comedy_multiplier;
}

void Audience::applyBoredom()
{
	//TODO balance this lowering, should occur with each actual dialogue choice made by player?
	overallApproval *= 0.9f;
}