#include "Prop.h"
#include <Engine\Renderer.h>

void Prop::setIsActive(bool new_active) 
{
	isActive = new_active;
}

bool Prop::getIsActive() const
{
	return isActive;
}

void Prop::setPosition(float x, float y)
{
	prop_sprite->xPos(x);
	prop_sprite->yPos(y);
}

ASGE::Sprite * Prop::getSprite()
{
	return prop_sprite;
}

void Prop::initSprite(ASGE::Renderer * rend, std::string texture_name)
{
	prop_sprite = rend->createRawSprite();
	prop_sprite->loadTexture(texture_name);
	//TODO - little bit of math to determine dimensions based on screen width / height?
}

void Prop::render(ASGE::Renderer* renderer) const
{
	if(prop_sprite != nullptr)
	{
		renderer->renderSprite(*prop_sprite);
	}
}
