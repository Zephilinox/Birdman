#pragma once

//SELF
#include "../AudioEngine.hpp"

class AudioEngineNone : public AudioEngine
{
public:
	AudioEngineNone(const std::string& audio_path);
	~AudioEngineNone() = default;

	void play(const std::string& name, bool loop = false) override final;
};