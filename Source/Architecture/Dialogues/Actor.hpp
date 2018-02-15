#pragma once

//STD
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <any>
#include <iostream>

//LIB
#include <Engine/Sprite.h>

class Actor
{
public:
	Actor(std::string name);

	bool hasFlag(const std::string flag);
	void addFlag(std::string flag);
	void removeFlag(const std::string flag);

	bool hasData(const std::string id);
	void removeData(const std::string id);

	template <class T>
	void addData(std::string id, T data)
	{
		if (datas.insert({id, data}).second)
		{
			std::cout << "DATA " + id + " ADDED ON " + name + "\n";
			std::cout << "DATA IS " << getData<T>(id) << "\n";
		}
		else
		{
			std::cout << "DATA " + id + " FAILED TO ADD ON " + name + "\n";
		}
	}

	template <class T>
	T& getData(const std::string id)
	{
		return std::any_cast<T&>(datas[id]);
	}

	const std::string name;
	std::string realName;

private:
	ASGE::Sprite* portrait = nullptr;
	ASGE::Sprite* model;
	std::unordered_set<std::string> flags;
	std::unordered_map<std::string, std::any> datas;
};