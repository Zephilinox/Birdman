#pragma once

//STD
#include <functional>
#include <string>

//LIB
#include <Engine/Colours.h>

//SELF
#include "../GameData.hpp"
#include "../Signals/Signal.hpp"

class Button
{
public:
	Button() noexcept;

	void render() const;

	bool isSelected() const noexcept;
	void setSelected(bool selected) noexcept;

	void setPos(int x, int y) noexcept;
	void setName(std::string name);
	void setColour(ASGE::Colour colour) noexcept;
	void setSelectedColour(ASGE::Colour colour) noexcept;

	Signal<> on_click;

private:
	bool selected = false;
	
	int pos_x = 0;
	int pos_y = 0;
	std::string name = "Default";
	ASGE::Colour colour = ASGE::COLOURS::RED;
	ASGE::Colour selected_colour = ASGE::COLOURS::BLUE;
};