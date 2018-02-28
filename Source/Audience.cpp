#include "Audience.hpp"
#include "Rng.h"
#include "Architecture\GameData.hpp"

//STD
#include <random>

Audience::Audience()
{
	//TODO: move this to some kind of RNG class we can use anywhere
	sad_multiplier = GameData::getRNG()->getRandomInt(1, max_approval);
	comedy_multiplier = GameData::getRNG()->getRandomInt(1, max_approval);
	light_multiplier = GameData::getRNG()->getRandomInt(1, max_approval);
	dark_multiplier = GameData::getRNG()->getRandomInt(1, max_approval);

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
	comedy_sprite = GameData::getRenderer()->createUniqueSprite();
	sad_sprite = GameData::getRenderer()->createUniqueSprite();
	light_sprite = GameData::getRenderer()->createUniqueSprite();
	dark_sprite = GameData::getRenderer()->createUniqueSprite();
	
	//TODO - Brendon - This is where we'll load the sprites for the audience reactions.
	//Add them in when you have them

	/*comedy_sprite->loadTexture("");
	sad_sprite->loadTexture("");
	light_sprite->loadTexture("");
	dark_sprite->loadTexture("");*/
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
	float overall_approval = sad_approval + light_approval + dark_approval + comedy_approval;
	overall_approval *= 0.25f;
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
	light_approval += effect * sad_multiplier;
}

//Takes an int, applies it to the responsiveness/multiplier then adds it to the approval
void Audience::addToDark(int effect)
{
	dark_approval += effect * sad_multiplier;
}

//Takes an int, applies it to the responsiveness/multiplier then adds it to the approval
void Audience::addToComedy(int effect)
{
	comedy_approval += effect * sad_multiplier;
}