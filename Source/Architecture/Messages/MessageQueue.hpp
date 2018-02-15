#pragma once

//STD
#include <queue>
#include <chrono>

//SELF
#include "../Signals/Signal.hpp"
#include "Message.hpp"
#include "../Timer.hpp"

class MessageQueue
{
public:
	using FunctionType = Signal<Message*>::FunctionType;
	
	//Note: This is not a hard limit. Highly dependant on the number of listeners and priority messages.
	void processMessages(Timer::nanoseconds_float max_processing_time);

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

	template <typename T, typename... Args>
	void sendPriorityMessage(Args... args)
	{
		priority_message_queue.emplace(new T(args...));
	}

	template <typename T>
	void sendPriorityMessage()
	{
		priority_message_queue.emplace(new T);
	}
private:
	Signal<Message*> messenger;
	std::queue<std::unique_ptr<Message>> message_queue;
	std::queue<std::unique_ptr<Message>> priority_message_queue;
	Timer timer;
};