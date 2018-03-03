#pragma once
#include <Engine/Sprite.h>

namespace ASGE
{
	class Renderer;
	class Sprite;
}

class GameData;

constexpr float max_approval = 100.0f;

class Audience
{
public:
	//not really necessary to make them default since we don't define anything, can exclude them
	//https://en.wikipedia.org/wiki/Rule_of_three_(C%2B%2B_programming) (/rule of five)
	Audience(GameData* data);
	~Audience() = default;

	int getSadApproval();
	int getLightApproval();
	int getDarkApproval();
	int getComedyApproval();


	//todo: consider doing this in the constructor instead of having an init function
	void varyApprovalsBetweenNights();
	void updateApproval();
	void loadAudienceSprites();

	void update(float dt);
	void render() const;


	void setStartingApprovalMultipliers(int starting_val);
	float getOverallApproval();

	//Use these to add values (positive or negative) to the approval
	void addToSad(float);
	void addToLight(float);
	void addToDark(float);
	void addToComedy(float);

	void applyBoredom();

private:
	float sad_multiplier = 0.0f;
	float light_multiplier = 0.0f;
	float dark_multiplier = 0.0f;
	float comedy_multiplier = 0.0f;

	float overallApproval = 0.0f;

	float sad_approval = 20.0f;
	float light_approval = 20.0f;
	float dark_approval = 20.0f;
	float comedy_approval = 20.0f;

	float panel_pos_x;
	float panel_pos_y;

	float sprite_width_override;
	float sprite_height_override;

	GameData* game_data;

	std::unique_ptr<ASGE::Sprite> perfect_sprite;
	std::unique_ptr<ASGE::Sprite> great_sprite;
	std::unique_ptr<ASGE::Sprite> good_sprite;
	std::unique_ptr<ASGE::Sprite> indifferent_sprite;
};