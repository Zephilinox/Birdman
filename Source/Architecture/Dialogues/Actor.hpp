#pragma once

//STD
#include <unordered_set>
#include <unordered_map>
#include <string>
#include <Engine/Sprite.h>
#include <any>

class Actor
{
public:
	Actor(std::string name);

	bool hasFlag(const std::string flag);
	void addFlag(std::string flag);
	void removeFlag(const std::string flag);

	bool hasData(const std::string id)
	{
		if (datas.count(id))
		{
			std::cout << name + " HAS DATA " + id + "\n";
			return true;
		}
		else
		{
			std::cout << name + " DOES NOT HAVE DATA " + id + "\n";
			return false;
		}
	}

	template <class T>
	void addData(std::string id, T data)
	{
		if (datas.insert({ id, data }).second)
		{
			std::cout << "DATA " + id + " ADDED ON" + name + "\n";
		}
		else
		{
			std::cout << "DATA " + id + " FAILED TO ADD ON " + name + "\n";
		}
	}

	void removeData(const std::string id)
	{
		if (flags.erase(id))
		{
			std::cout << "DATA " + id + " REMOVED ON " + name + "\n";
		}
		else
		{
			std::cout << "DATA " + id + " FAILED TO REMOVE ON " + name + "\n";
		}
	}

	template <class T>
	T* getData(const std::string id)
	{
		if (datas.count(id))
		{
			return std::any_cast<T>(&datas[id]);
		}

		return nullptr;
	}

	const std::string name;

private:
	ASGE::Sprite* portrait = nullptr;
	ASGE::Sprite* model;
	std::unordered_set<std::string> flags;
	std::unordered_map<std::string, std::any> datas;
};