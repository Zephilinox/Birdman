#pragma once

//STD
#include <functional>
#include <vector>
#include <string>
#include <experimental/vector>



class Actor
{
public:
	Actor(std::string name)
		: name(name)
	{}

	bool hasFlag(const std::string flag)
	{
		for (const auto& f : flags)
		{
			if (f == flag)
			{
				return true;
			}
		}

		return false;
	}

	void addFlag(const std::string flag)
	{
		flags.push_back(flag);
	}

	void removeFlag(const std::string flag)
	{
		std::experimental::erase_if(flags, [flag](const auto& f)
		{
			return f == flag;
		});
	}

	const std::string name;

private:
	std::vector<std::string> flags;
};



struct Dialogue
{
public:
	using FunctionType = std::function<std::string()>;

	Dialogue(std::string dialogue_name, std::string speaker_name,
		FunctionType dialogue_text, FunctionType next_dialogue,
		bool player_option = false)
		: name(dialogue_name)
		, speaker(speaker_name)
		, text(dialogue_text)
		, next_dialogue(next_dialogue)
		, player_option(player_option)
	{};

	const std::string name;
	const std::string speaker;
	const bool player_option;

	const FunctionType text;
	const FunctionType next_dialogue;
};



class DialogueTree
{
public:

	void addDialogue(std::string dialogue_name, std::string speaker_name,
		Dialogue::FunctionType dialogue_text, Dialogue::FunctionType next_dialogue)
	{
		dialogues.emplace_back(std::move(dialogue_name), std::move(speaker_name), dialogue_text, next_dialogue);
	}

	void addDialogue(std::string dialogue_name, std::string speaker_name,
		std::string dialogue_text, Dialogue::FunctionType next_dialogue)
	{
		dialogues.emplace_back(std::move(dialogue_name), std::move(speaker_name),
		[dialogue_text]()
		{
			return dialogue_text;
		}, next_dialogue);
	}

	void addDialogue(std::string dialogue_name, std::string speaker_name,
		Dialogue::FunctionType dialogue_text, std::string next_dialogue)
	{
		dialogues.emplace_back(std::move(dialogue_name), std::move(speaker_name), dialogue_text,
		[next_dialogue]()
		{
			return next_dialogue;
		});
	}

	void addDialogue(std::string dialogue_name, std::string speaker_name,
		std::string dialogue_text, std::string next_dialogue)
	{
		dialogues.emplace_back(std::move(dialogue_name), std::move(speaker_name),
		[dialogue_text]()
		{
			return dialogue_text;
		},
		[next_dialogue]()
		{
			return next_dialogue;
		});
	}

	void addPlayerOption(std::string dialogue_name,
		Dialogue::FunctionType dialogue_text, Dialogue::FunctionType next_dialogue)
	{
		dialogues.emplace_back(std::move(dialogue_name), "player", dialogue_text, next_dialogue, true);
	}

	Actor* getActor(std::string name)
	{
		for (int i = 0; i < actors.size(); ++i)
		{
			if (actors[i].name == name)
			{
				return &actors[i];
			}
		}

		actors.emplace_back(std::move(name));
		return &actors.back();
	}

	Actor* getPlayer()
	{
		return getActor("player");
	}

	std::string play(std::string dialogue_name)
	{
		if (dialogue_name == "")
		{
			playing = false;
			return "";
		}

		for (auto& d : dialogues)
		{
			if (d.name == dialogue_name)
			{
				current_dialogue = &d;
				playing = true;
				return d.speaker + ": " + d.text();
			}
		}

		current_dialogue = nullptr;
		playing = false;
		return "";
	}

	std::string next()
	{
		if (current_dialogue)
		{
			return play(current_dialogue->next_dialogue());
		}

		playing = false;
		return "...\n";
	}

	bool playing = false;
private:
	std::vector<Dialogue> dialogues;
	std::vector<Actor> actors;

	Dialogue* current_dialogue = nullptr;
};