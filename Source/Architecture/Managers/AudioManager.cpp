#include "AudioManager.hpp"

//STD
#include <functional>
#include <string>

//SELF
#include "../Audio/irrKlang/AudioEngineIrrklang.hpp"
#include "../Audio/SFML/AudioEngineSFML.hpp"
#include "../Audio/None/AudioEngineNone.hpp"

AudioManager::AudioManager(Engine engine, const std::string& audio_path)
	: engine(engine)
{
	switch (engine)
	{
		//todo: handle switching to none if other engines fail to initialize? need to move from constructor to init funcs
		case Engine::IrrKlang:
			audio_engine.reset(new AudioEngineIrrklang(audio_path));
			break;
		case Engine::SFML:
			audio_engine.reset(new AudioEngineSFML(audio_path));
			break;
		case Engine::None:
		default:
			audio_engine.reset(new AudioEngineNone(audio_path));
			break;

	}
}

AudioEngine* AudioManager::getAudioEngine()
{
	return audio_engine.get();
}

AudioManager::Engine AudioManager::getEngineType()
{
	return engine;
}

void AudioManager::play(const std::string& name, bool loop)
{
	audio_engine->play(std::forward<const std::string&>(name), std::forward<bool>(loop));
}
