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
	
	FontManager(std::string font_path = "");
	
	//Call these to add a new font from a font file
	void addFont(const std::string& path, const std::string& name, FontSize size);
	
	//Call this to set a font of the specified size, make sure the size has already been loaded.
	void setFont(const std::string& name);

private:
	std::unordered_map<std::string, FontID> fonts;
	std::string font_path;
};
