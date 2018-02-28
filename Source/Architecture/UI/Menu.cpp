#include "Menu.hpp"

//SELF
#include "../GameData.hpp"
#include "../Managers/InputManager.hpp"
#include "../Managers/AudioManager.hpp"

Menu::Menu(bool vertical)
{
	buttons.reserve(4);
	selected_button_id = 0;

	if (vertical)
	{
		key_increment = ASGE::KEYS::KEY_UP;
		key_increment_alt = ASGE::KEYS::KEY_W;
		key_decrement = ASGE::KEYS::KEY_DOWN;
		key_decrement_alt = ASGE::KEYS::KEY_S;
	}
	else
	{
		key_increment = ASGE::KEYS::KEY_LEFT;
		key_increment_alt = ASGE::KEYS::KEY_A;
		key_decrement = ASGE::KEYS::KEY_RIGHT;
		key_decrement_alt = ASGE::KEYS::KEY_D;
	}
}

void Menu::update()
{
	if (buttons.empty()) return;

	if (GameData::getInput()->isKeyPressed(key_decrement) ||
		GameData::getInput()->isKeyPressed(key_decrement_alt))
	{
		if (selected_button_id == buttons.size() - 1)
		{
			selectButton(0);
		}
		else
		{
			selectButton(selected_button_id + 1);
		}
	}
	
	if (GameData::getInput()->isKeyPressed(key_increment) ||
		GameData::getInput()->isKeyPressed(key_increment_alt))
	{
		if (selected_button_id == 0)
		{
			selectButton(buttons.size() - 1);
		}
		else
		{
			selectButton(selected_button_id - 1);
		}
	}

	if (GameData::getInput()->isKeyPressed(ASGE::KEYS::KEY_ENTER))
	{
		GameData::getAudio()->play("button_click.wav");

		if (buttons.size())
		{
			buttons[selected_button_id].on_click.emit();
		}
	}
}

void Menu::render() const
{
	for (const Button& b : buttons)
	{
		b.render();
	}
}

int Menu::addButton(int x, int y, std::string name, ASGE::Colour colour, ASGE::Colour selected_colour)
{
	Button b;
	b.setPos(x, y);
	b.setName(name);
	b.setColour(colour);
	b.setSelectedColour(selected_colour);
	buttons.push_back(b);

	if (buttons.size() == 1)
	{
		buttons[selected_button_id].setSelected(true);
	}

	return buttons.size() - 1;
}

Button& Menu::getButton(int button_id)
{
	return buttons.at(button_id);
}

void Menu::selectButton(int button_id)
{
	buttons[selected_button_id].setSelected(false);
	selected_button_id = button_id;
	buttons[selected_button_id].setSelected(true);

	GameData::getAudio()->play("button_select.wav");
}
