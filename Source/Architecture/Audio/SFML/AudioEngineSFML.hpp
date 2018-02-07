#pragma once

//LIB
#include <SFML/Audio.hpp>

//SELF
#include "../AudioEngine.hpp"

class AudioEngineSFML : public AudioEngine
{
public:
	AudioEngineSFML(const std::string& audio_path);
	AudioEngineSFML() = default;

	void play(const std::string& name, bool loop = false) override final;
	
private:
	sf::SoundBuffer buffer;
	sf::Sound sound;
};