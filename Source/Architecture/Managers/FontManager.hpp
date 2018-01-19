#pragma once

//STD
#include <unordered_map>

namespace ASGE
{
	class Renderer;
}

class FontManager
{
public:
	using FontID = int;
	using FontSize = int;

	struct Font
	{
		std::unordered_map<FontSize, FontID> ids;
		std::string path;
	};

	FontManager::FontManager(ASGE::Renderer* renderer);
	
	void addFont(const std::string& path, const std::string& name);
	void addFont(const std::string& path, const std::string& name, FontSize size);

	void setFont(const std::string& name, FontSize size);

private:
	void loadFont(const std::string& name, FontSize size);

	ASGE::Renderer* renderer;
	std::unordered_map<std::string, Font> fonts;
};
