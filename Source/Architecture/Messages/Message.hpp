#pragma once

#include "../Signals/Signal.hpp"

//https://stackoverflow.com/questions/36117884/compile-time-typeid-for-every-type

static constexpr uint32_t offset = 2166136261u;
static constexpr uint32_t prime = 16777619u;

constexpr uint32_t helper(uint32_t partial, const char* str)
{
	return str[0] == 0 ? partial : helper((partial^str[0]) * prime, str + 1);
}

constexpr uint32_t hash_str(const char* input)
{
	return helper(offset, input);
}

struct HashedID
{
	HashedID(const char* str)
		: hash(hash_str(str))
	{}
	
	const bool operator!=(const char* str) const
	{
		return hash != hash_str(str);
	}

	const bool operator!=(uint32_t hashed_str) const
	{
		return hash != hashed_str;
	}

	const bool operator==(const char* str) const
	{
		return hash == hash_str(str);
	}

	const bool operator==(uint32_t hashed_str) const
	{
		return hash == hashed_str;
	}

	const uint32_t hash;
};

class Message
{
public:
	Message() = delete;
	virtual ~Message() noexcept = default;
	
	const HashedID id = "Message";
	static constexpr uint32_t ID = hash_str("Message");

protected:
	Message(const char* id)
		: id(id)
	{}
};