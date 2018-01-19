#include "MessageQueue.hpp"

void MessageQueue::processMessages(std::chrono::microseconds max_processing_time)
{
	//Could optimise this more by having listeners choose which events they care about, thus less listeners being called back.
	auto start_time = std::chrono::high_resolution_clock().now();
	auto processing_time = std::chrono::microseconds::duration();

	while (!message_queue.empty() && processing_time < max_processing_time)
	{
		messenger.emit(message_queue.front().get());
		message_queue.pop();

		auto end_time = std::chrono::high_resolution_clock().now();
		processing_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
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
