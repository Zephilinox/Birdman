#include "Button.hpp"

//SELF
#include "../GameData.hpp"
#include "../Managers/FontManager.hpp"

Button::Button()
{
}

void Button::render(GameData* game_data) const
{
	game_data->getFontManager()->setFont("Default", 40);

	if (selected)
	{
		game_data->getRenderer()->renderText(name.c_str(), pos_x, pos_y, 1.0f, selected_colour);
	}
	else
	{
		game_data->getRenderer()->renderText(name.c_str(), pos_x, pos_y, 1.0f, colour);
	}
}

bool Button::isSelected() const
{
	return selected;
}

void Button::setSelected(bool s)
{
	selected = s;
}

void Button::setPos(int x, int y)
{
	pos_x = x;
	pos_y = y;
}

void Button::setName(std::string n)
{
	name = n;
}

void Button::setColour(ASGE::Colour c)
{
	colour = c;
}

void Button::setSelectedColour(ASGE::Colour c)
{
	selected_colour = c;
}
