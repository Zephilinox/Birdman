#pragma once

//STD
#include <unordered_set>
#include <string>
#include <Engine/Sprite.h>

class Actor
{
public:
	Actor(std::string name);

	bool hasFlag(const std::string flag);
	void addFlag(std::string flag);
	void removeFlag(const std::string flag);

	const std::string name;

private:
	ASGE::Sprite* portrait = nullptr;
	ASGE::Sprite* model;
	std::unordered_set<std::string> flags;
};