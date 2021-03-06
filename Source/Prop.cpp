#include "Prop.h"
#include <Engine\Renderer.h>
#include "Constants.hpp"

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

ASGE::Sprite* Prop::getSprite()
{
	return prop_sprite.get();
}

void Prop::initSprite(ASGE::Renderer* rend, std::string texture_name)
{
	prop_sprite.reset(rend->createRawSprite());
	prop_sprite->loadTexture(texture_name);
}

void Prop::render(ASGE::Renderer* renderer) const
{
	if (prop_sprite)
	{
		renderer->renderSprite(*prop_sprite.get(), Z_ORDER_LAYER::PROPS);
	}
}
