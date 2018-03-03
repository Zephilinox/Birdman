#include "Audience.hpp"
#include "Rng.h"
#include "Architecture\GameData.hpp"
#include <algorithm>

//STD
#include <random>

Audience::Audience(GameData* data)
{
	//TODO: move this to some kind of RNG class we can use anywhere
	game_data = data;

	panel_pos_x = 926.0f;
	panel_pos_y = 476.0f;
	sprite_width_override = 346.0f;
	sprite_height_override = 236.0f;

	loadAudienceSprites();
	setStartingApprovalMultipliers(1.0f);
}

float Audience::getSadApproval()
{
	return sad_multiplier;
}

float Audience::getLightApproval()
{
	return light_multiplier;
}

float Audience::getDarkApproval()
{
	return dark_multiplier;
}

float Audience::getComedyApproval()
{
	return comedy_multiplier;
}

void Audience::varyApprovalsBetweenNights()
{
	float ten_percent_sad = sad_multiplier / 10.0f;
	float ten_percent_comedy = comedy_multiplier / 10.0f;
	float ten_percent_light = light_multiplier/10.0f;
	float ten_percent_dark = dark_multiplier / 10.0f;


	float sad_variance = game_data->getRandomNumberGenerator()->getRandomFloat(-ten_percent_sad, ten_percent_sad);
	float comedy_variance = game_data->getRandomNumberGenerator()->getRandomFloat(-ten_percent_comedy, ten_percent_comedy);
	float dark_variance = game_data->getRandomNumberGenerator()->getRandomFloat(-ten_percent_dark, ten_percent_dark);
	float light_variance = game_data->getRandomNumberGenerator()->getRandomFloat(-ten_percent_light, ten_percent_light);

	sad_multiplier += sad_variance;
	comedy_multiplier += comedy_variance;
	light_multiplier += light_variance;
	dark_multiplier += dark_variance;

	sad_multiplier = std::clamp(sad_multiplier, 0.1f, 1.4f);
	comedy_multiplier = std::clamp(comedy_multiplier, 0.1f, 1.4f);
	light_multiplier = std::clamp(light_multiplier, 0.1f, 1.4f);
	dark_multiplier = std::clamp(dark_multiplier, 0.1f, 1.4f);
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

void Audience::setStartingApprovalMultipliers(int starting_val)
{
	comedy_multiplier = game_data->getRandomNumberGenerator()->getRandomFloat(starting_val - 0.7f, starting_val);
	dark_multiplier = game_data->getRandomNumberGenerator()->getRandomFloat(starting_val - 0.7f, starting_val);
	light_multiplier = game_data->getRandomNumberGenerator()->getRandomFloat(starting_val - 0.7f, starting_val);
	sad_multiplier = game_data->getRandomNumberGenerator()->getRandomFloat(starting_val - 0.7f, starting_val);
}

float Audience::getOverallApproval()
{
	return overallApproval;
}

void Audience::updateApproval()
{
	overallApproval = sad_approval + light_approval + dark_approval + comedy_approval;
	overallApproval *= 0.25f;
	overallApproval = std::clamp(overallApproval, 0.0f, 1.0f);
}

//Takes an int, applies it to the responsiveness/multiplier then adds it to the approval
void Audience::addToSad(float effect)
{
	sad_approval += effect * sad_multiplier;
	updateApproval();
	applyBoredom();
	sad_approval = std::clamp(sad_approval, 0.0f, 1.4f);
}

//Takes an int, applies it to the responsiveness/multiplier then adds it to the approval
void Audience::addToLight(float effect)
{
	light_approval += effect * light_multiplier;
	updateApproval();
	applyBoredom();
	light_approval = std::clamp(light_approval, 0.0f, 1.4f);
}

//Takes an int, applies it to the responsiveness/multiplier then adds it to the approval
void Audience::addToDark(float effect)
{
	dark_approval += effect * dark_multiplier;
	updateApproval();
	applyBoredom();
	dark_approval = std::clamp(dark_approval, 0.0f, 1.4f);
}

//Takes an int, applies it to the responsiveness/multiplier then adds it to the approval
void Audience::addToComedy(float effect)
{
	comedy_approval += effect * comedy_multiplier;
	updateApproval();
	applyBoredom();
	dark_approval = std::clamp(dark_approval, 0.0f, 1.4f);
}

void Audience::applyBoredom()
{
	//TODO balance this lowering, should occur with each actual dialogue choice made by player?
	overallApproval *= 0.9f;
}