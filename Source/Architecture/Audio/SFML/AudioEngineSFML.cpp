#include "AudioEngineSFML.hpp"

AudioEngineSFML::AudioEngineSFML(const std::string& audio_path)
	: AudioEngine(audio_path)
{
}

void AudioEngineSFML::play(const std::string& name, bool loop)
{
	if (!buffers[name].loadFromFile(audio_path + name))
	{
		throw "error loading " + audio_path + name;
	}
	
	sounds.push_back(std::make_unique<sf::Sound>(buffers[name]));
	sounds.back()->play();
}
