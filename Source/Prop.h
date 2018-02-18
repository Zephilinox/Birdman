#pragma once
#include <Engine\Sprite.h>

class Prop
{
public:
	Prop() = default;
	//TODO Ricardo, this needed?
	~Prop() { delete prop_sprite; }

	void setIsActive(bool new_active);
	bool getIsActive();

	ASGE::Sprite* getSprite();

	void initSprite(ASGE::Renderer* rend, std::string texture_name);
	void render(ASGE::Renderer* renderer);

private:
	bool isActive = false;
	ASGE::Sprite* prop_sprite;
};
