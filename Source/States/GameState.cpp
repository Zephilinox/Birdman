#include "GameState.hpp"

//STD
#include <memory>

//LIB
#include <Engine/Sprite.h>

//SELF
#include "../Architecture/GameData.hpp"
#include "../Constants.hpp"

GameState::GameState(GameData* game_data)
	: BaseState(game_data)
	, visual_dialogue(game_data, &dialogue_tree, "start_extra")
	, play_01(game_data)
{
	//add all the dialogue to the dialogue tree. each level would have its own dialogue tree
	dialogue_init();
	//dialogue_init2();
	//dialogue_init3();
	dialogue_kitchen();
	visual_dialogue.updateTree();
}

void GameState::update(const ASGE::GameTime&)
{
	visual_dialogue.update();

	if (game_data->getInputManager()->isKeyPressed(ASGE::KEYS::KEY_ENTER))
	{
		visual_dialogue.interact();
	}
}

void GameState::render() const
{
	visual_dialogue.render();
}

void GameState::onActive()
{
}

void GameState::onInactive()
{
}

void GameState::dialogue_init()
{
	dialogue_tree.getActor("player")->realName = "TeamBirb";
	auto spr = game_data->getRenderer()->createUniqueSprite();
	spr->loadTexture("../../Resources/Textures/Portraits/player.png");
	dialogue_tree.getActor("player")->portrait = std::move(spr);

	dialogue_tree.getActor("strange_npc")->realName = "Stranger";
	spr = game_data->getRenderer()->createUniqueSprite();
	spr->loadTexture("../../Resources/Textures/Portraits/strange_npc.png");
	dialogue_tree.getActor("strange_npc")->portrait = std::move(spr);

	dialogue_tree.addPlayerOption("start_extra",
	[&]()
	{
		if (dialogue_tree.getPlayer()->hasFlag("super_cool_sword_thingy"))
		{
			return "";
		}

		return "Get Sword";
	},
		[&]()
	{
		dialogue_tree.getPlayer()->addFlag("super_cool_sword_thingy");
		return "sword_got";
	});

	dialogue_tree.addDialogue("sword_got", DialogueTree::player, "Woo! I found a cool sword!", "");

	dialogue_tree.addPlayerOption("start_extra",
	[&]()
	{
		if (dialogue_tree.getActor("strange_npc")->hasFlag("stole_sword") && !dialogue_tree.getPlayer()->hasFlag("npc_found"))
		{
			return "Find Strange NPC";
		}

		return "Strange NPC";
	},
	[&]()
	{	
		if (dialogue_tree.getActor("strange_npc")->hasFlag("stole_sword") && !dialogue_tree.getPlayer()->hasFlag("npc_found"))
		{
			dialogue_tree.getPlayer()->addFlag("npc_found");
			return "";
		}

		return "start";
	});

	dialogue_tree.addPlayerOption("start_extra",
	[&]()
	{
		if (dialogue_tree.getPlayer()->hasFlag("npc_killed"))
		{
			return "Go to Town";
		}

		return "";
	},
	[&]()
	{
		visual_dialogue.setDefaultDialogue("town/menu");

		return "town/start";
	});

	dialogue_tree.addPlayerOption("start_extra", "Kitchen Scene", "kitchen/start");

	dialogue_tree.addDialogue("start", "strange_npc",
	[&]()
	{
		if (dialogue_tree.getPlayer()->hasFlag("npc_killed"))
		{
			if (dialogue_tree.getSpeaker()->hasFlag("stole_sword"))
			{
				return "*You stand over the dead corpse of the man who stole your sword*";
			}
			else
			{
				return "*You stand over the dead corpse of the man you randomly slaughtered*";
			}
		}

		if (dialogue_tree.getPlayer()->hasFlag("npc_found"))
		{
			dialogue_tree.getPlayer()->addFlag("npc_killed");
			return "No! Please! *ARRRGH*";
		}

		if (dialogue_tree.getSpeaker()->hasFlag("stole_sword"))
		{
			return "*The strange NPC who stole your sword is nowhere to be found...*";
		}

		if (!dialogue_tree.getSpeaker()->hasFlag("met_player"))
		{
			return "What? Who are you? What do you want?";
		}

		return "What? You're still here? What is it now.";
	},
	[&]()
	{
		dialogue_tree.getSpeaker()->addFlag("met_player");

		if (dialogue_tree.getPlayer()->hasFlag("npc_killed"))
		{
			return "end";
		}

		if (dialogue_tree.getSpeaker()->hasFlag("stole_sword"))
		{
			return "";
		}
		else
		{
			return "start_options";
		}
	});

	//Note: player options work differently to ordinary dialogue_tree though they're kinda the same
	//Multiple options can have the same dialogue name, which is what allows them to all be presented
	//If it returns a blank string when determining what the dialogue will be, the option should not be shown
	//(but this is meant to be handled by whatever handles the option, like the console/visual DialogueManager
	//Also keep in mind you might want to set the next dialogue to be one where the player is speaking and not an NPC, but I was lazy.
	dialogue_tree.addPlayerOption("start_options", "Hello.", "start2");
	dialogue_tree.addPlayerOption("start_options", "Howdy.", "start2");

	dialogue_tree.addPlayerOption("start_options",
	[&]()
	{
		if (dialogue_tree.getPlayer()->hasFlag("super_cool_sword_thingy"))
		{
			return "Die!";
		}

		return "";
	},
	[&]()
	{
		dialogue_tree.getPlayer()->addFlag("npc_found");
		return "start";
	});

	dialogue_tree.addPlayerOption("start_options", "Bye.", "bye_ply");
	dialogue_tree.addDialogue("bye_ply", DialogueTree::player, "Sorry, gotta run.", "bye");
	dialogue_tree.addDialogue("start2", "strange_npc", "Well I'm quite busy right now.", "bye");

	dialogue_tree.addDialogue("bye", "strange_npc",
	[&]()
	{
		if (dialogue_tree.getPlayer()->hasFlag("super_cool_sword_thingy"))
		{
			dialogue_tree.getSpeaker()->addFlag("plans_to_steal_sword");
			return "Wait a minute...";
		}
		return "Speak to you another time.";
	},
	[&]()
	{
		if (dialogue_tree.getSpeaker()->hasFlag("plans_to_steal_sword"))
		{
			return "steal_sword";
		}

		return "";
	});

	dialogue_tree.addDialogue("steal_sword", "strange_npc",
	[&]()
	{
		dialogue_tree.getPlayer()->removeFlag("super_cool_sword_thingy");
		dialogue_tree.getSpeaker()->removeFlag("plans_to_steal_sword");
		dialogue_tree.getSpeaker()->addFlag("stole_sword");

		return "*Steals Your Sword*";
	}, "");

	dialogue_tree.addDialogue("end", DialogueTree::player,
	[&]()
	{
		if (dialogue_tree.getSpeaker()->hasFlag("stole_sword"))
		{
			dialogue_tree.getPlayer()->addFlag("super_cool_sword_thingy");

			return "He got what he deserved, and now I have my sword back.";
		}
		else
		{
			return "I don't know why I did that...";
		}
	}, "");

	//Town Scene
	dialogue_tree.getActor("blab_npc")->realName = "Old Man";
	spr = game_data->getRenderer()->createUniqueSprite();
	spr->loadTexture("../../Resources/Textures/Portraits/blabbering_npc.png");
	dialogue_tree.getActor("blab_npc")->portrait = std::move(spr);

	dialogue_tree.addDialogue("town/start", "", "*You spot the town of Bree in the distance.*", "");

	dialogue_tree.addPlayerOption("town/menu", "Blacksmith", "town/blacksmith");
	dialogue_tree.addPlayerOption("town/menu", "Townhall", "town/townhall");
	dialogue_tree.addPlayerOption("town/menu", "Blabbering NPC", "town/blab");
	dialogue_tree.addPlayerOption("town/menu",
	[&]()
	{
		int& data = dialogue_tree.getPlayer()->getData<int>("town_menu_count", 0);
		std::string s = "You've been here " + std::to_string(data) + " times";
		data += 1;

		if (data >= 5)
		{
			dialogue_tree.getPlayer()->removeData("town_menu_count");
		}

		return s;
	}, "");

	dialogue_tree.addDialogue("town/blacksmith", "blacksmith_npc", "Yo we're closed.\nGet out.", "town/bye");
	dialogue_tree.addDialogue("town/townhall", "mayor","We have no quests, go away.", "town/bye");
	dialogue_tree.addDialogue("town/bye", DialogueTree::player, "Ah okay, bye.", "");
	dialogue_tree.addDialogue("town/blab", "blab_npc", "We should probably ensure one convo isn't longer than\n3 lines of text. We can break it up with '\\n' but we still\nneed to make sure it doesn't go on for too long", "town/blab2");
	dialogue_tree.addDialogue("town/blab2", "blab_npc", "If it does we can chain it like so, which is nice.\nIt's not worth the effort trying to automate any of this to be honest.\nWe'll just have to handle it all manually.", "town/start");
}

void GameState::dialogue_init2()
{
	//Add an option to the input menu for the player
	dialogue_tree.addPlayerOption("start_extra",
	[&]()
	{
		if (dialogue_tree.getPlayer()->hasFlag("named..."))
		{
			return "";
		}

		return "*Struts across the stage*";
	},
	[&]()
	{
		return "done_action1";
	});

	dialogue_tree.addDialogue("done_action1", DialogueTree::player, "There are those that call me...", "select_name");

	dialogue_tree.addPlayerOption("select_name", "Tim...",
	[&]()
	{
		dialogue_tree.getPlayer()->addFlag("named...");
		return "done_action2";
	});

	dialogue_tree.addPlayerOption("select_name", "Barnabus...",
	[&]()
	{
		dialogue_tree.getPlayer()->addFlag("named...");
		return "done_action2";
	});
}

void GameState::dialogue_init3()
{
	dialogue_tree.addPlayerOption("start_extra", "playgame", 
		[&]() 
	{
		dialogue_tree.getPlayer()->addFlag("playing");
		return "play";
	});
	dialogue_tree.addDialogue("play", DialogueTree::player, "You are playing the game", "");
	dialogue_tree.addPlayerOption("start_extra", "maybeplay", "1");
	dialogue_tree.addDialogue("1", "Jim", "Ya names Jim", "");
	dialogue_tree.addPlayerOption("start_extra", 
		[&]()
	{
		if (dialogue_tree.getPlayer()->hasFlag("playing"))
		{
			
			return "continue";
		}
		return "";
	}, [&]()
	{
		dialogue_tree.getPlayer()->removeFlag("playing");
		return "con";
	});
}

void GameState::dialogue_kitchen()
{
	dialogue_tree.addDialogue("kitchen/start", "lesley", "He loved me", "kitchen/start1");
	dialogue_tree.addDialogue("kitchen/start1", "ralph", "Yeah. He loved her so much he tried to kill her.", "kitchen/start2");
	dialogue_tree.addDialogue("kitchen/start2", "laura", "He tried to kill you?", "kitchen/start3");
	dialogue_tree.addDialogue("kitchen/start3", "lesley", "No.\nOkay, well he did beat me up one night.", "kitchen/start4");
	dialogue_tree.addDialogue("kitchen/start4", "lesley", "He dragged me around the living room by my ankles, yelling\n\"I Love you, I love you, bitch.\"", "kitchen/start5");
	dialogue_tree.addDialogue("kitchen/start5", "ralph", "What do you do with a love like that?", "kitchen/start6");
	dialogue_tree.addDialogue("kitchen/start6", "ralph", "How is that...\nThat is not love and you know it.\nWhy do you insist on calling it...", "kitchen/start7");
	dialogue_tree.addDialogue("kitchen/start7", "lesley", "You can say what you want, but I know what it was.", "kitchen/start8");
	dialogue_tree.addDialogue("kitchen/start8", "ralph", "What about you, Nick?\nDoes that sound like love to you?", "kitchen/start9");
	dialogue_tree.addDialogue("kitchen/start9", "riggan", "Sorry I'm late.\nI'm the wrong person to ask.\nI've only heard his name mentioned in passing.", "kitchen/start10");
	dialogue_tree.addDialogue("kitchen/start10", "riggan", "You'd have to know the particulars\nBut I think what you're saying is\nthat love is absolute.", "kitchen/start11");
	dialogue_tree.addDialogue("kitchen/start11", "ralph", "Yeah. The kind of love I'm talking about is...", "kitchen/start12");
	dialogue_tree.addDialogue("kitchen/start12", "ralph", "The kind of love I'm talking about, you don't try and kill people.", "kitchen/start13");
	dialogue_tree.addDialogue("kitchen/start13", "lesley", "It was love, Mel.\nTo Eddie, it was.", "kitchen/start14");
	dialogue_tree.addDialogue("kitchen/start14", "lesley", "I don't care what anybody says.\nHe was ready to die for it.", "kitchen/start15");
	dialogue_tree.addDialogue("kitchen/start15", "ralph", "Ask her what he did after she left him.", "kitchen/start16");
	dialogue_tree.addDialogue("kitchen/start16", "lesley", "He shot himself in the mouth.\nBut he screwed that up, too.\nPoor Ed.", "kitchen/start17");
	dialogue_tree.addDialogue("kitchen/start17", "ralph", "Poor Ed, my ass.\nThe guy was dangerous.", "kitchen/start18");
	dialogue_tree.addDialogue("kitchen/start18", "laura", "How'd he screw it up if he shot himself in the mouth?", "kitchen/start19");
	dialogue_tree.addDialogue("kitchen/start19", "ralph", "He used to carry this twenty-two.\nWe lived like fugitives those days.\n", "kitchen/start20");
	dialogue_tree.addDialogue("kitchen/start20", "ralph", "I never knew if he was going to come\nout of the bushes or from behind\na car and just start shooting.", "kitchen/start21");
	dialogue_tree.addDialogue("kitchen/start21", "ralph", "The man was crazy.\nHe was capable of anything.", "kitchen/start22");
	dialogue_tree.addDialogue("kitchen/start22", "laura", "Christ. What a nightmare...", "kitchen/start23");
	dialogue_tree.addDialogue("kitchen/start23", "ralph", "He used to call me at the hospital and say...", "kitchen/start24");
	dialogue_tree.addDialogue("kitchen/start24", "ralph", "\"Son of a bitch. Your days are numbered.\"", "");
}