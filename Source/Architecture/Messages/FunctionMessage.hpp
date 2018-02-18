#pragma once

//STD
#include <functional>

//SELF
#include "Message.hpp"

class FunctionMessage : public Message
{
public:
	FunctionMessage(std::function<void()> func)
		: Message("Function")
		, function(func)
	{}

	void execute()
	{
		function();
	}

	static constexpr uint32_t ID = hash_str("Function");

private:
	std::function<void()> function;
};