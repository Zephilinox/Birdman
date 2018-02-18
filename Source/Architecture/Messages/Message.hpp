#pragma once

//https://stackoverflow.com/questions/36117884/compile-time-typeid-for-every-type
//https://stackoverflow.com/questions/37658794/integer-constant-overflow-warning-in-constexpr

namespace
{
	static constexpr uint32_t offset = 2166136261u;
	static constexpr uint32_t prime = 16777619u;

	constexpr uint32_t helper(uint32_t partial, const char* str)
	{
		return str[0] == 0 ? partial : helper(static_cast<unsigned long long>((partial^str[0])) * prime, str + 1);
	}
}

using HashedID = uint32_t;

//todo: figure out why I can't make this a protected member function
constexpr HashedID hash(const char* input)
{
	return helper(offset, input);
}

class Message
{
public:
	Message() = delete;
	virtual ~Message() noexcept = default;
	
	const HashedID id;
	static constexpr HashedID ID = hash("Message");

protected:
	Message(HashedID id)
		: id(id)
	{}
};