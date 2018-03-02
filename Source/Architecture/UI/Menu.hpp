#pragma once

//STD
#include <vector>

//LIB
#include <Engine/Sprite.h>

//SELF
#include "Button.hpp"

class GameData;

class Menu
{
public:
	Menu(GameData* game_data, bool vertical = true);

	void update();
	void render() const;
	void reset();

	int addButton(int x, int y, std::string name, ASGE::Colour colour, ASGE::Colour selected_colour);
	Button& getButton(int button_id);

	std::unique_ptr<ASGE::Sprite> selection_image = nullptr;
private:
	void selectButton(int button_id);

	GameData* game_data;
	std::vector<Button> buttons;

	unsigned selected_button_id;

	int key_increment;
	int key_increment_alt;
	int gamepad_increment;
	int key_decrement;
	int key_decrement_alt;
	int gamepad_decrement;
};