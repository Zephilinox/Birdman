#include "Prop.h"
#include <Engine\Renderer.h>

void Prop::setIsActive(bool new_active)
{
	isActive = new_active;
}

bool Prop::getIsActive()
{
	return isActive;
}

ASGE::Sprite * Prop::getSprite()
{
	return prop_sprite;
}

void Prop::initSprite(ASGE::Renderer * rend, std::string texture_name)
{
	prop_sprite = rend->createRawSprite();
	prop_sprite->loadTexture(texture_name);
}

void Prop::render(ASGE::Renderer * renderer)
{
	renderer->renderSprite(*prop_sprite);
}
