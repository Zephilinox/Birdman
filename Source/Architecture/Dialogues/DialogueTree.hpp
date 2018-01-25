#pragma once

//STD
#include <functional>
#include <vector>
#include <unordered_set>
#include <string>
#include <experimental/vector>
#include <iomanip>

class Actor
{
public:
	Actor(std::string name)
		: name(name)
	{}

	bool hasFlag(const std::string flag)
	{
		if (flags.count(flag))
		{
			std::cout << "FLAG " + flag + " IS ON " + name + " FLAG\n";
			return true;
		}
		else
		{
			std::cout << "FLAG " + flag + " IS NOT ON " + name + " FLAG\n";
			return false;
		}
	}

	void addFlag(std::string flag)
	{
		if (flags.insert(flag).second)
		{
			std::cout << "FLAG " + flag + " ADDED ON" + name + "\n";
		}
		else
		{
			std::cout << "FLAG " + flag + " FAILED TO ADD ON " + name + "\n";
		}
	}

	void removeFlag(const std::string flag)
	{
		if (flags.erase(flag))
		{
			std::cout << "FLAG " + flag + " REMOVED ON " + name + "\n";
		}
		else
		{
			std::cout << "FLAG " + flag + " FAILED TO REMOVE ON " + name + "\n";
		}
	}

	const std::string name;

private:
	std::unordered_set<std::string> flags;
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

	void addPlayerOption(std::string dialogue_name,
		std::string dialogue_text, Dialogue::FunctionType next_dialogue)
	{
		dialogues.emplace_back(std::move(dialogue_name), "player", [dialogue_text]() {return dialogue_text; }, next_dialogue, true);
	}

	void addPlayerOption(std::string dialogue_name,
		Dialogue::FunctionType dialogue_text, std::string next_dialogue)
	{
		dialogues.emplace_back(std::move(dialogue_name), "player", dialogue_text, [next_dialogue]() {return next_dialogue; }, true);
	}

	void addPlayerOption(std::string dialogue_name,
		std::string dialogue_text, std::string next_dialogue)
	{
		dialogues.emplace_back(std::move(dialogue_name), "player", [dialogue_text]() {return dialogue_text; }, [next_dialogue]() {return next_dialogue; }, true);
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

	Actor* getSpeaker()
	{
		return getActor(current_dialogue->speaker);
	}

	std::string play(std::string dialogue_name)
	{
		if (dialogue_name == "")
		{
			playing = false;
			return "";
		}

		if (player_option)
		{
			current_player_options.clear();
			player_option = false;
		}

		for (auto& d : dialogues)
		{
			if (d.name == dialogue_name)
			{
				if (!player_option && !d.player_option)
				{
					current_dialogue = &d;
					playing = true;
					std::cout << "RUNNING DIALOGUE TEXT DETERMINATOR FOR '" << d.name << "'\n";
					std::string text = d.text();

					if (text != "")
					{
						text = d.speaker + ": " + text;
						return std::move(text);
					}
					else
					{
						std::cout << "ERROR: DIALOGUE TEXT FOR '" << d.name << "' IS EMPTY. STOPPING.\n";
						current_dialogue = nullptr;
						playing = false;
						return "";
					}
				}
				else
				{
					current_dialogue = nullptr;
					playing = true;
					player_option = true;
					current_player_options.push_back(&d);
				}
			}
		}

		if (player_option)
		{
			return "";
		}
		else
		{
			current_dialogue = nullptr;
			playing = false;
			return "";
		}
	}

	std::string next()
	{
		if (current_dialogue)
		{
			std::cout << "\nRUNNING NEXT DIALOGUE DETERMINATOR FOR '" << current_dialogue->name << "'\n";
			std::string n = current_dialogue->next_dialogue();
			std::cout << "GOING TO NEXT DIALOGUE: '" << n << "'\n";
			return play(n);
		}

		playing = false;
		return "...\n";
	}

	bool playing = false;
	bool player_option = false;
	std::vector<Dialogue*> current_player_options;
private:
	std::vector<Dialogue> dialogues;
	std::vector<Actor> actors;

	Dialogue* current_dialogue = nullptr;
};