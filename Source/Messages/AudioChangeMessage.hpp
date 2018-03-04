#pragma once

//SELF
#include "../Architecture/Messages/Message.hpp"

class AudioChangeMessage : public Message
{
public:
	AudioChangeMessage(std::string audio_path);

	static const HashedID ID = hash("AudioChangeMessage");
	std::string audio_path;
};