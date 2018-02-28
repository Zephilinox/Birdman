#pragma once

//STD
#include <vector>

//SELF
#include "Button.hpp"

class GameData;

class Menu
{
public:
	Menu(bool vertical = true);

	void update();
	void render() const;

	int addButton(int x, int y, std::string name, ASGE::Colour colour, ASGE::Colour selected_colour);
	Button& getButton(int button_id);

private:
	void selectButton(int button_id);

	std::vector<Button> buttons;

	unsigned selected_button_id;

	int key_increment;
	int key_increment_alt;
	int key_decrement;
	int key_decrement_alt;
};