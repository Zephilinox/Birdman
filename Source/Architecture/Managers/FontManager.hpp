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

	FontManager(ASGE::Renderer* renderer);
	
	//Call these to add a new font from a font file
	void addFont(const std::string& path, const std::string& name);
	void addFont(const std::string& path, const std::string& name, FontSize size);

	//Call this to load a new font size for a specific font name
	void loadFont(const std::string& name, FontSize size);

	//Call this to set a font of the specified size, make sure the size has already been loaded.
	void setFont(const std::string& name, FontSize size);

private:
	ASGE::Renderer* renderer;
	std::unordered_map<std::string, Font> fonts;
};
