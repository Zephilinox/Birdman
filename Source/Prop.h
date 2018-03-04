#pragma once
#include <Engine\Sprite.h>

class Prop
{
public:
	Prop() = default;

	void setIsActive(bool new_active);
	bool getIsActive() const;
	void setPosition(float x, float y);

	ASGE::Sprite* getSprite();

	void initSprite(ASGE::Renderer* rend, std::string texture_name);
	void render(ASGE::Renderer* renderer) const;

private:
	bool isActive = false;
	std::unique_ptr<ASGE::Sprite> prop_sprite = nullptr;
};
