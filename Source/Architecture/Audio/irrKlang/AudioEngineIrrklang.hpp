#pragma once

//STD
#include <memory>

//SELF
#include "../AudioEngine.hpp"

namespace irrklang
{
	class ISoundEngine;
}

class AudioEngineIrrklang : public AudioEngine
{
public:
	AudioEngineIrrklang(const std::string& audio_path);
	~AudioEngineIrrklang() = default;

	void play(const std::string& name, bool loop = false) override final;

private:
	std::unique_ptr<irrklang::ISoundEngine> audio_engine = nullptr;
};
