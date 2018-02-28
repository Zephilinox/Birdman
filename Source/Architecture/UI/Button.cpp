#include "Button.hpp"

//SELF
#include "../GameData.hpp"
#include "../Managers/FontManager.hpp"

Button::Button() noexcept
{
}

void Button::render() const
{
	GameData::getFonts()->setFont("Default");

	if (selected)
	{
		GameData::getRenderer()->renderText(name.c_str(), pos_x, pos_y, 1.0f, selected_colour);
	}
	else
	{
		GameData::getRenderer()->renderText(name.c_str(), pos_x, pos_y, 1.0f, colour);
	}
}

bool Button::isSelected() const noexcept
{
	return selected;
}

void Button::setSelected(bool s) noexcept
{
	selected = s;
}

void Button::setPos(int x, int y) noexcept
{
	pos_x = x;
	pos_y = y;
}

void Button::setName(std::string n)
{
	name = n;
}

void Button::setColour(ASGE::Colour c) noexcept
{
	colour = c;
}

void Button::setSelectedColour(ASGE::Colour c) noexcept
{
	selected_colour = c;
}
