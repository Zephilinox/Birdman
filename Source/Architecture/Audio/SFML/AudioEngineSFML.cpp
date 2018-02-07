#include "AudioEngineSFML.hpp"

AudioEngineSFML::AudioEngineSFML(const std::string& audio_path)
	: AudioEngine(audio_path)
{
}

void AudioEngineSFML::play(const std::string & name, bool loop)
{
	if (!buffer.loadFromFile(audio_path + name))
	{
		throw "error loading SFML audio";
	}
	
	sound.setBuffer(buffer);
	sound.play();
}
