#include "AudioChangeMessage.hpp"

AudioChangeMessage::AudioChangeMessage(std::string audio_path)
	: Message(ID)
	, audio_path(audio_path)
{

}