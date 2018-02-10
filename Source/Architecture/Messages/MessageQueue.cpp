#include "MessageQueue.hpp"

//STD
#include <iomanip>

void MessageQueue::processMessages(Timer::nanoseconds_float max_processing_time)
{
	//Could optimise this more by having listeners choose which events they care about, thus less listeners being called back.
	timer.restart();

	int processedMessages = 0;
	while (!message_queue.empty() && timer.getChronoElapsedTime() < max_processing_time)
	{
		processedMessages++;
		messenger.emit(message_queue.front().get());
		message_queue.pop();
	}

	if (timer.getChronoElapsedTime() > max_processing_time)
	{
		std::cout << "WARNING: MessageQueue took " << std::fixed << std::setprecision(1) << std::setfill('0') <<
			std::setw(3) << timer.getElapsedTime<Timer::milliseconds>() << "ms processing " <<
			std::setw(4) << processedMessages << " messages. " <<
			std::setw(4) << message_queue.size() << " remain\n";
	}
}

Connection MessageQueue::addListener(FunctionType function)
{
	return messenger.connect(function);
}

bool MessageQueue::removeListener(Connection c)
{
	return messenger.disconnect(c);
}
