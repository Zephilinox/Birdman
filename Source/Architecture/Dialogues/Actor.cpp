#include "Actor.hpp"

Actor::Actor(std::string name)
	: name(name)
{}

bool Actor::hasFlag(const std::string flag)
{
	if (flags.count(flag))
	{
		std::cout << "FLAG " + flag + " IS ON " + name + " FLAG\n";
		return true;
	}
	else
	{
		//std::cout << "FLAG " + flag + " IS NOT ON " + name + " FLAG\n";
		return false;
	}
}

void Actor::addFlag(std::string flag)
{
	if (flags.insert(flag).second)
	{
		std::cout << "FLAG " + flag + " ADDED ON" + name + "\n";
	}
	else
	{
		//std::cout << "FLAG " + flag + " FAILED TO ADD ON " + name + "\n";
	}
}

void Actor::removeFlag(const std::string flag)
{
	if (flags.erase(flag))
	{
		std::cout << "FLAG " + flag + " REMOVED ON " + name + "\n";
	}
	else
	{
		//std::cout << "FLAG " + flag + " FAILED TO REMOVE ON " + name + "\n";
	}
}