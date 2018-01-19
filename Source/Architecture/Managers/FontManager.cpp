#include "FontManager.hpp"

//LIB
#include <Engine/Renderer.h>

FontManager::FontManager(ASGE::Renderer* renderer)
	: renderer(renderer)
{}

void FontManager::addFont(const std::string& path, const std::string& name)
{
	if (fonts.count(name))
	{
		throw std::runtime_error("FontManager::addFont - Font already exists");
	}

	fonts[name].path = path;
}

void FontManager::addFont(const std::string& path, const std::string& name, FontSize size)
{
	addFont(path, name);
	loadFont(name, size);
}

void FontManager::setFont(const std::string& name, FontSize size)
{
	loadFont(name, size);
	renderer->setFont(fonts[name].ids[size]);
}

void FontManager::loadFont(const std::string& name, FontSize size)
{
	if (!fonts.count(name))
	{
		throw std::runtime_error("FontManager::loadFont - Font doesn't exist");
	}

	if (!fonts[name].ids.count(size))
	{
		fonts[name].ids[size] = renderer->loadFont(fonts[name].path.c_str(), size);

		if (fonts[name].ids[size] < 0)
		{
			//NOTE: We have to call this again for some reason. The first call sometimes causes glGenTextures to fail.
			fonts[name].ids[size] = renderer->loadFont(fonts[name].path.c_str(), size);
		}

		//Second call didn't help
		if (fonts[name].ids[size] < 0)
		{
			throw std::runtime_error("FontManager::loadFont - Could not load font");
		}
	}
}


