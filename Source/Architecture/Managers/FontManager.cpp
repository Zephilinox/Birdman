#include "FontManager.hpp"

//LIB
#include <Engine/Renderer.h>

//SELF
#include "../GameData.hpp"

FontManager::FontManager(std::string font_path)
	: font_path(font_path)
{}

void FontManager::addFont(const std::string& path, const std::string& name, FontSize size)
{
	if (fonts.count(name))
	{
		throw "Font already exists";
	}

	fonts[name] = GameData::getRenderer()->loadFont(std::string(font_path + path).c_str(), size);

	if (fonts[name] < 0)
	{
		fonts.erase(name);
		throw "Could not load font";
	}
}

void FontManager::setFont(const std::string& name)
{
	GameData::getRenderer()->setFont(fonts[name]);
}
