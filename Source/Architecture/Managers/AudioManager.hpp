#pragma once

//STD
#include <string>
#include <memory>

//SELF
#include "../Audio/AudioEngine.hpp"

//This is a factory atm, might become more later though.
class AudioManager
{
public:
	enum class Engine
	{
		None,
		IrrKlang,
		SFML
	};

	AudioManager(Engine engine, const std::string& audio_path);
	AudioEngine* getAudioEngine();
	Engine getEngineType();

	void play(const std::string& name, bool loop = false);

private:
	std::unique_ptr<AudioEngine> audio_engine;
	Engine engine;
};