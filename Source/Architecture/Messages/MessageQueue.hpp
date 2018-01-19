#pragma once

//STD
#include <queue>
#include <chrono>

//SELF
#include "../Signals/Signal.hpp"
#include "Message.hpp"

class MessageQueue
{
public:
	using FunctionType = Signal<Message*>::FunctionType;

	MessageQueue() = default;
	~MessageQueue() = default;

	//1000 microseconds = 1 millisecond
	//Note: This is not a hard limit. Highly dependant on the number of listeners.
	void processMessages(std::chrono::microseconds max_processing_time);

	Connection addListener(FunctionType function);
	bool removeListener(Connection c);

	template <typename T, typename... Args>
	void sendMessage(Args... args)
	{
		message_queue.emplace(new T(args...));
	}

	template <typename T>
	void sendMessage()
	{
		message_queue.emplace(new T);
	}

private:
	Signal<Message*> messenger;
	std::queue<std::unique_ptr<Message>> message_queue;
};