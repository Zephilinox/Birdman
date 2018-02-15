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

private:
	std::function<void()> function;
};