#pragma once

#include "../Signals/Signal.hpp"

class Message
{
public:
	Message() = default;

	virtual ~Message() noexcept = default;
	
	const std::string message_id = "Message";

protected:
	Message(std::string id)
		: message_id(id)
	{}
};