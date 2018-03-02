#pragma once
#include <Engine/Sprite.h>

namespace ASGE
{
	class Renderer;
	class Sprite;
}

class GameData;

constexpr unsigned int max_approval = 100;

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
	void loadAudienceSprites();

	void update(float dt);
	void render() const;


	void setStartingApprovals(int starting_val);

	float getOverallApproval();

	//Use these to add values (positive or negative) to the approval
	void addToSad(int);
	void addToLight(int);
	void addToDark(int);
	void addToComedy(int);

	void applyBoredom();

private:
	int sad_multiplier = 0;
	int light_multiplier = 0;
	int dark_multiplier = 0;
	int comedy_multiplier = 0;

	float overallApproval = 0.0f;

	float sad_approval = 0.0f;
	float light_approval = 0.0f;
	float dark_approval = 0.0f;
	float comedy_approval = 0.0f;

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