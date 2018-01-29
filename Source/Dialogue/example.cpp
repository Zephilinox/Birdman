
//Also worth mentioning that this is utilising lambda's (might need to google that), but it can also use standard functions (take a look at std::function documentation)
//Standard functions can't be used in here though if we're #including it inside another function, so keep that in mind.
//#include is basically a big copy-paste command for an entire file.

dialogues.addDialogue("start", "strange_npc",
[&]()
{	
	if (dialogues.getPlayer()->hasFlag("npc_killed"))
	{
		if (dialogues.getSpeaker()->hasFlag("stole_sword"))
		{
			return "*You stand over the dead corpse of the man who stole your sword*";
		}
		else
		{
			return "*You stand over the dead corpse of the man you randomly slaughtered*";
		}
	}

	if (dialogues.getPlayer()->hasFlag("npc_found"))
	{
		dialogues.getPlayer()->addFlag("npc_killed");
		return "No! Please! *ARRRGH*";
	}

	if (dialogues.getSpeaker()->hasFlag("stole_sword"))
	{
		return "*The strange NPC who stole your sword is nowhere to be found...*";
	}

	if (!dialogues.getSpeaker()->hasFlag("met_player"))
	{
		return "What? Who are you? What do you want?";
	}

	return "What? You're still here? What is it now.";
},
[&]()
{
	//hacky, ideally this shouldn't be necessary
	//we can use a global "actor" to represent global flags
	//e.g. dialogues.getActor("global")->addFlag("added_button");
	//but might be useful in some niche cases, so leaving the example of it here for now.
	static bool added_button = false;

	dialogues.getSpeaker()->addFlag("met_player");

	if (dialogues.getPlayer()->hasFlag("npc_killed"))
	{
		//TODO: remove button function
		if (added_button)
		{
			menu.getButton(2).setName("INVALID");
		}

		return "end";
	}

	if (dialogues.getSpeaker()->hasFlag("stole_sword"))
	{
		//TODO: get button by name

		if (!added_button)
		{
			menu.addButton(WINDOW_WIDTH / 2 - 80, WINDOW_HEIGHT / 2 - 120, "FIND STRANGE NPC", ASGE::COLOURS::DIMGRAY, ASGE::COLOURS::ANTIQUEWHITE);
			menu.getButton(2).on_click.connect(
				[&]()
			{
				dialogues.getPlayer()->addFlag("npc_found");
				std::cout << "NPC FOUND\n";
			});
			added_button = true;
		}

		return "";
	}
	else
	{
		return "start_options";
	}
});

//Note: player options work differently to ordinary dialogues though they're kinda the same
//Multiple options can have the same dialogue name, which is what allows them to all be presented
//If it returns a blank string when determining what the dialogue will be, the option should not be shown
//(but this is meant to be handled by whatever handles the option, like the console/visual DialogueManager
//Also keep in mind you might want to set the next dialogue to be one where the player is speaking and not an NPC, but I was lazy.
dialogues.addPlayerOption("start_options", "Hello.", "start2");
dialogues.addPlayerOption("start_options", "Howdy.", "start2");
dialogues.addPlayerOption("start_options", "Hey.", "start2");

dialogues.addPlayerOption("start_options",
[&]()
{
	if (dialogues.getPlayer()->hasFlag("super_cool_sword_thingy"))
	{
		dialogues.getPlayer()->addFlag("npc_found");
		return "Die!";
	}

	return "";
}, "start");

dialogues.addPlayerOption("start_options", "Bye.", "bye");

dialogues.addDialogue("start2", "strange_npc", "Well I'm quite busy right now.", "bye");

dialogues.addDialogue("bye", "strange_npc",
[&]()
{
	if (dialogues.getPlayer()->hasFlag("super_cool_sword_thingy"))
	{
		dialogues.getSpeaker()->addFlag("plans_to_steal_sword");
		return "Wait a minute...";
	}
	return "Speak to you another time.";
},
[&]()
{
	if (dialogues.getSpeaker()->hasFlag("plans_to_steal_sword"))
	{
		return "steal_sword";
	}

	return "";
});

dialogues.addDialogue("steal_sword", "strange_npc",
[&]()
{
	dialogues.getPlayer()->removeFlag("super_cool_sword_thingy");
	dialogues.getSpeaker()->removeFlag("plans_to_steal_sword");
	dialogues.getSpeaker()->addFlag("stole_sword");

	return "*Steals Your Sword*";
}, "");

dialogues.addDialogue("end", "player",
[&]()
{
	if (dialogues.getSpeaker()->hasFlag("stole_sword"))
	{
		dialogues.getPlayer()->addFlag("super_cool_sword_thingy");

		return "He got what he deserved, and now I have my sword back.";
	}
	else
	{
		return "I don't know why I did that...";
	}
}, "");