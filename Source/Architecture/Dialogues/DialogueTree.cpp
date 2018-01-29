#include "DialogueTree.hpp"

//SELF
#include "Actor.hpp"

void DialogueTree::addDialogue(std::string dialogue_name, std::string speaker_name, Dialogue::FunctionType dialogue_text, Dialogue::FunctionType next_dialogue)
{
	dialogues.emplace_back(std::move(dialogue_name), std::move(speaker_name), dialogue_text, next_dialogue);
}

void DialogueTree::addDialogue(std::string dialogue_name, std::string speaker_name, std::string dialogue_text, Dialogue::FunctionType next_dialogue)
{
	dialogues.emplace_back(std::move(dialogue_name), std::move(speaker_name),[dialogue_text](){	return dialogue_text;}, next_dialogue);
}

void DialogueTree::addDialogue(std::string dialogue_name, std::string speaker_name, Dialogue::FunctionType dialogue_text, std::string next_dialogue)
{
	dialogues.emplace_back(std::move(dialogue_name), std::move(speaker_name), dialogue_text,[next_dialogue](){	return next_dialogue;});
}

void DialogueTree::addDialogue(std::string dialogue_name, std::string speaker_name, std::string dialogue_text, std::string next_dialogue)
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

void DialogueTree::addPlayerOption(std::string dialogue_name, Dialogue::FunctionType dialogue_text, Dialogue::FunctionType next_dialogue)
{
	dialogues.emplace_back(std::move(dialogue_name), "player", dialogue_text, next_dialogue, true);
}

void DialogueTree::addPlayerOption(std::string dialogue_name, std::string dialogue_text, Dialogue::FunctionType next_dialogue)
{
	dialogues.emplace_back(std::move(dialogue_name), "player", [dialogue_text]() {return dialogue_text; }, next_dialogue, true);
}

void DialogueTree::addPlayerOption(std::string dialogue_name, Dialogue::FunctionType dialogue_text, std::string next_dialogue)
{
	dialogues.emplace_back(std::move(dialogue_name), "player", dialogue_text, [next_dialogue]() {return next_dialogue; }, true);
}

void DialogueTree::addPlayerOption(std::string dialogue_name, std::string dialogue_text, std::string next_dialogue)
{
	dialogues.emplace_back(std::move(dialogue_name), "player", [dialogue_text]() {return dialogue_text; }, [next_dialogue]() {return next_dialogue; }, true);
}

Actor* DialogueTree::getActor(std::string name)
{
	for (size_t i = 0; i < actors.size(); ++i)
	{
		if (actors[i].name == name)
		{
			return &actors[i];
		}
	}

	actors.emplace_back(std::move(name));
	return &actors.back();
}

Actor* DialogueTree::getPlayer()
{
	return getActor("player");
}

Actor* DialogueTree::getSpeaker()
{
	return getActor(current_dialogue->speaker);
}

std::string DialogueTree::play(std::string dialogue_name)
{
	//Don't ask how this works. it's a mess.

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

std::string DialogueTree::next()
{
	if (current_dialogue)
	{
		std::cout << "\nRUNNING NEXT DIALOGUE DETERMINATOR FOR '" << current_dialogue->name << "'\n";
		std::string n = current_dialogue->next();
		std::cout << "GOING TO NEXT DIALOGUE: '" << n << "'\n";
		return play(n);
	}

	playing = false;
	return "...\n";
}
