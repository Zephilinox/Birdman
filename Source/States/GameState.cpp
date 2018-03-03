#include "GameState.hpp"

//STD
#include <memory>

//LIB
#include <Engine/Sprite.h>

//SELF
#include "../Architecture/GameData.hpp"
#include "../Constants.hpp"
#include "../Play.hpp"
#include "../Scene.hpp"
#include "../Character.h"
#include "PauseState.hpp"

GameState::GameState(GameData* game_data)
	: BaseState(game_data)
	, visual_dialogue(game_data, &dialogue_tree, "start")
	, play_01(game_data)
	, bottom_panel(game_data->getRenderer()->createRawSprite())
	, top_panel(game_data->getRenderer()->createRawSprite())
{
	bottom_panel->loadTexture("../../Resources/Textures/UI/BottomPanel.png");
	bottom_panel->yPos(468);
	
	top_panel->loadTexture("../../Resources/Textures/UI/TopPanel.png");

	play_01.create();
	dialogue_init();
	dialogue_kitchen();
	visual_dialogue.updateTree();
}

void GameState::update(const ASGE::GameTime& gt)
{
	visual_dialogue.update(float(gt.delta_time.count()) / 1000.0f);
	play_01.update(float(gt.delta_time.count()) / 1000.0f);

	if (game_data->getInputManager()->isActionPressed("enter"))
	{
		visual_dialogue.interact();
	}

	if (game_data->getInputManager()->isActionPressed("escape"))
	{
		game_data->getStateManager()->push<PauseState>();
	}
}

void GameState::render() const
{
	game_data->getRenderer()->renderSprite(*bottom_panel);
	visual_dialogue.render();
	play_01.render();
	game_data->getRenderer()->renderSprite(*top_panel);
}

void GameState::onActive()
{
}

void GameState::onInactive()
{
}

//This is here as a point of reference, consider it documentation.

void GameState::dialogue_init()
{
	dialogue_tree.getActor("player")->realName = "TeamBirb";
	auto spr = game_data->getRenderer()->createUniqueSprite();
	spr->loadTexture("../../Resources/Textures/Mike/spl1.png");
	dialogue_tree.getActor("player")->portrait = std::move(spr);

	dialogue_tree.getActor("strange_npc")->realName = "Stranger";
	spr = game_data->getRenderer()->createUniqueSprite();
	spr->loadTexture("../../Resources/Textures/Leslie/hpl1.png");
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
	spr->loadTexture("../../Resources/Textures/Laura/ppl1.png");
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


void GameState::dialogue_kitchen()
{
	auto riggan = dialogue_tree.getActor("riggan");
	riggan->realName = "Nick"; //for kitchen scene, Ed in apartment
	auto riggan_pic = game_data->getRenderer()->createUniqueSprite();
	riggan_pic->loadTexture("../../Resources/Textures/Clint/cpl1.png");
	riggan->portrait = std::move(riggan_pic);

	auto leslie = dialogue_tree.getActor("leslie");
	auto leslie_pic = game_data->getRenderer()->createUniqueSprite();
	leslie_pic->loadTexture("../../Resources/Textures/Clint/hpl1.png");
	leslie->portrait = std::move(leslie_pic);

	auto laura = dialogue_tree.getActor("laura");
	auto laura_pic = game_data->getRenderer()->createUniqueSprite();
	laura_pic->loadTexture("../../Resources/Textures/Laura/ppl1.png");
	laura->portrait = std::move(laura_pic);

	auto mike = dialogue_tree.getActor("mike");
	auto mike_pic = game_data->getRenderer()->createUniqueSprite();
	mike_pic->loadTexture("../../Resources/Textures/Mike/spl1.png");
	mike->portrait = std::move(mike_pic);

	dialogue_tree.addDialogue("start", "",
	[&]()
	{
		return play_01.getScene()->getDescription();
	},
	[&]()
	{
		return "kitchen/start";
	});

	dialogue_tree.addDialogue("kitchen/start", "leslie",
	[&]()
	{
		auto leslie = play_01.getScene()->getCharacter(Play::LESLIE);
		leslie->setFacing(Character::CharacterFacing::SOUTH);
		leslie->setPosition(400, 300);

		return "He loved me.";
	}, "kitchen/start1");


	////How to effect the play, scene and audience



	//Intro Scene
	dialogue_tree.addDialogue("kitchen/start1", "ralph", "Yeah. He loved her so much he tried to kill her.", "kitchen/start2");
	dialogue_tree.addDialogue("kitchen/start2", "laura", "He tried to kill you?", "kitchen/start3");
	dialogue_tree.addDialogue("kitchen/start3", "leslie", "No.\nOkay, well he did beat me up one night.", "kitchen/start4");
	dialogue_tree.addDialogue("kitchen/start4", "leslie", "He dragged me around the living room by my ankles, yelling\n\"I Love you, I love you, bitch.\"", "kitchen/start5");
	dialogue_tree.addDialogue("kitchen/start5", "ralph", "What do you do with a love like that?", "kitchen/start6");
	dialogue_tree.addDialogue("kitchen/start6", "ralph", "How is that...\nThat is not love and you know it.\nWhy do you insist on calling it...", "kitchen/start7");
	dialogue_tree.addDialogue("kitchen/start7", "leslie", "You can say what you want, but I know what it was.", "kitchen/start8");
	dialogue_tree.addDialogue("kitchen/start8", "ralph", "What about you, Nick?\nDoes that sound like love to you?", "kitchen/option1");

	//How to - inline function to move scene based on player choices
	dialogue_tree.addPlayerOption("kitchen/option1", "Dismissive", [&]()
	{
		//add an arbitrary value to the "sad" value of the audience, this is applied to a multiplier, then offset by boredom.
		play_01.getAudience()->addToSad(10);
		//add an arbitrary value to the "sad" value of the scene, this is used to determine which scene is gone to next.
		play_01.getScene()->addToSad(1);
		//Determines which scene the play will go to next based on the values stored in scene. Transitions and moves to that scene.
		play_01.moveToNextScene();
		return "kitchen/sad/start9";
	});
	dialogue_tree.addPlayerOption("kitchen/option1", "Cold", "kitchen/comedy/start9");
	dialogue_tree.addPlayerOption("kitchen/option1", "Indecisive", "kitchen/light/start9");
	dialogue_tree.addPlayerOption("kitchen/option1", "Disagree", "kitchen/dark/start9");



	//If Sad Option  - Pick 1 - (done)
	dialogue_tree.addDialogue("kitchen/sad/start9", "riggan", "Sorry I'm late.\nI'm the wrong person to ask.\nI've only heard his name mentioned in passing.", "kitchen/sad/start10");
	dialogue_tree.addDialogue("kitchen/sad/start10", "riggan", "You'd have to know the particulars\nBut I think what you're saying is\nthat love is absolute.", "kitchen/sad/start11");
	dialogue_tree.addDialogue("kitchen/sad/start11", "ralph", "Yeah. The kind of love I'm talking about is...", "kitchen/sad/start12");
	dialogue_tree.addDialogue("kitchen/sad/start12", "ralph", "The kind of love I'm talking about, you don't try and kill people.", "kitchen/sad/start13");
	dialogue_tree.addDialogue("kitchen/sad/start13", "leslie", "It was love, Mel.\nTo Eddie, it was.", "kitchen/sad/start14");
	dialogue_tree.addDialogue("kitchen/sad/start14", "leslie", "I don't care what anybody says.\nHe was ready to die for it.", "kitchen/sad/start15");
	dialogue_tree.addDialogue("kitchen/sad/start15", "ralph", "Ask her what he did after she left him.", "kitchen/sad/start16");
	dialogue_tree.addDialogue("kitchen/sad/start16", "leslie", "He shot himself in the mouth.\nBut he screwed that up, too.\nPoor Ed.", "kitchen/sad/start17");
	dialogue_tree.addDialogue("kitchen/sad/start17", "ralph", "Poor Ed, my ass.\nThe guy was dangerous.", "kitchen/sad/start18");
	dialogue_tree.addDialogue("kitchen/sad/start18", "laura", "How'd he screw it up if he shot himself in the mouth?", "kitchen/sad/start19");
	dialogue_tree.addDialogue("kitchen/sad/start19", "ralph", "He used to carry this twenty-two.\nWe lived like fugitives those days.\n", "kitchen/sad/start20");
	dialogue_tree.addDialogue("kitchen/sad/start20", "ralph", "I never knew if he was going to come\nout of the bushes or from behind\na car and just start shooting.", "kitchen/sad/start21");
	dialogue_tree.addDialogue("kitchen/sad/start21", "ralph", "The man was crazy.\nHe was capable of anything.", "kitchen/sad/start22");
	dialogue_tree.addDialogue("kitchen/sad/start22", "laura", "Christ. What a nightmare...", "kitchen/sad/start23");
	dialogue_tree.addDialogue("kitchen/sad/start23", "ralph", "He used to call me at the hospital and say...", "kitchen/sad/start24");
	dialogue_tree.addDialogue("kitchen/sad/start24", "ralph", "\"Son of a bitch. Your days are numbered.\"", "");

	//If Sad Option - Pick 2 - (done)
	dialogue_tree.addDialogue("apartment/sad/start1", "riggan", "Terri! Terri!\nTerri! I know you're in there!", "Apartment/sad/start2");
	dialogue_tree.addDialogue("apartment/sad/start2", "riggan", "*You bang on the door loudly with force. It opens.*", "Apartment/sad/start3");
	dialogue_tree.addDialogue("apartment/sad/start3", "riggan", "Terri????", "Apartment/sad/start4");
	dialogue_tree.addDialogue("apartment/sad/start4", "leslie", "Ed!...\nWhat are you doing here?", "Apartment/sad/start5");
	dialogue_tree.addDialogue("apartment/sad/start5", "riggan", "Why? I need you to tell me why.\nI lived for you, I worshipped you...", "Apartment/sad/start6");
	dialogue_tree.addDialogue("apartment/sad/start6", "mike", "Listen Ed, I know this is hard but...", "Apartment/sad/start7");
	dialogue_tree.addDialogue("apartment/sad/start7", "riggan", "Fuck you.\nShut up\nFuck you!", "Apartment/sad/start8");
	dialogue_tree.addDialogue("apartment/sad/start8", "riggan", "*You push Mel, he falls to the floor*", "Apartment/sad/start9");
	dialogue_tree.addDialogue("apartment/sad/start9", "leslie", "Eddie!\nPlease!", "Apartment/sad/start10");
	dialogue_tree.addDialogue("apartment/sad/start10", "riggan", "*You point the gun at Mel's head*", "Apartment/sad/start11");
	dialogue_tree.addDialogue("apartment/sad/start11", "riggan", "What's wrong with me.\nWhy do I end up having to beg people to love me?", "Apartment/sad/start12");
	dialogue_tree.addDialogue("apartment/sad/start12", "leslie", "Ed.\nEddie.\nPlease..... Give me the gun.", "Apartment/sad/start13");
	dialogue_tree.addDialogue("apartment/sad/start13", "leslie", "*Leslie begins to cry*", "Apartment/sad/start14");
	dialogue_tree.addDialogue("apartment/sad/start14", "leslie", "Just look at me.\nI was drowning.\nI was not capable of...", "Apartment/sad/start15");
	dialogue_tree.addDialogue("apartment/sad/start15", "leslie", "You deserve to be loved.\nYou do.", "Apartment/sad/start16");
	dialogue_tree.addDialogue("apartment/sad/start16", "riggan", "I just wanted to be what you wanted.", "Apartment/sad/start17");
	dialogue_tree.addDialogue("apartment/sad/start17", "riggan", "Now I spend every fucking minute praying to be somebody else.\nSomeone I'm not.\nAnyone...", "Apartment/sad/start18");
	dialogue_tree.addDialogue("apartment/sad/start18", "mike", "Put down the gun, Ed.\nShe just doesn't love you anymore.\"", "");

	//If Comedy Option - Pick 1 - (done)
	dialogue_tree.addDialogue("kitchen/comedy/start9", "riggan", "Well if you ask me.\nSeems like the guy likes tough love.\nIf thats the case reply with tough love.", "kitchen/comedy/start10");
	dialogue_tree.addDialogue("kitchen/comedy/start10", "riggan", "if you still loved him.\nYou should of them smacked him with a pan or even a bat.", "kitchen/comedy/start11");
	dialogue_tree.addDialogue("kitchen/comedy/start11", "ralph", "This isn't something to joke about.\nThe kind of love I'm talking about is...", "kitchen/comedy/start12");
	dialogue_tree.addDialogue("kitchen/comedy/start12", "ralph", "The kind of love I'm talking about, you don't try and kill people more hurt.", "kitchen/comedy/start13");
	dialogue_tree.addDialogue("kitchen/comedy/start13", "leslie", "It was love, Mel.\nTo Eddie, it was.", "kitchen/comedy/start14");
	dialogue_tree.addDialogue("kitchen/comedy/start14", "leslie", "I don't care what anybody says.\nHe was ready to die for it.", "kitchen/comedy/start15");
	dialogue_tree.addDialogue("kitchen/comedy/start15", "ralph", "Ask her what he did after she left him.", "kitchen/comedy/start16");
	dialogue_tree.addDialogue("kitchen/comedy/start16", "leslie", "He shot himself in the mouth.\nBut he screwed that up, too.\nPoor Ed.", "kitchen/comedy/start17");
	dialogue_tree.addDialogue("kitchen/comedy/start17", "ralph", "Poor Ed, my ass.\nThe guy was dangerous.", "kitchen/comedy/start18");
	dialogue_tree.addDialogue("kitchen/comedy/start18", "laura", "How'd he screw it up if he shot himself in the mouth?", "kitchen/comedy/start19");
	dialogue_tree.addDialogue("kitchen/comedy/start19", "ralph", "He used to carry this twenty-two.\nWe lived like fugitives those days.\n", "kitchen/comedy/start20");
	dialogue_tree.addDialogue("kitchen/comedy/start20", "ralph", "I never knew if he was going to come\nout of the bushes or from behind\na car and just start shooting.", "kitchen/comedy/start21");
	dialogue_tree.addDialogue("kitchen/comedy/start21", "ralph", "The man was crazy.\nHe was capable of anything.", "kitchen/comedy/start22");
	dialogue_tree.addDialogue("kitchen/comedy/start22", "laura", "Christ. What a nightmare...", "kitchen/comedy/start23");
	dialogue_tree.addDialogue("kitchen/comedy/start23", "ralph", "He used to call me at the hospital and say...", "kitchen/comedy/start24");
	dialogue_tree.addDialogue("kitchen/comedy/start24", "ralph", "\"Son of a bitch. Your days are numbered.\"", "");

	//If Comedy Option - Pick 2 - ()
	dialogue_tree.addDialogue("apartment/start1", "riggan", "Terri!\nI know your in there, I'm coming in!", "Apartment/start2");
	dialogue_tree.addDialogue("apartment/start2", "riggan", "*You enter through the door*", "Apartment/start3");
	dialogue_tree.addDialogue("apartment/start3", "riggan", "How's it going", "Apartment/start4");
	dialogue_tree.addDialogue("apartment/start4", "leslie", "Ed!...\n What are you doing here?", "Apartment/start5");
	dialogue_tree.addDialogue("apartment/start5", "riggan", "Why? Did Mel not tell you>.\n Mel knows how much i worship you...\nSo he invited me for a threesome", "Apartment/start6");
	dialogue_tree.addDialogue("apartment/start6", "mike", "Listen Ed, I know this is hard but...\n the threesome is off", "Apartment/start7");
	dialogue_tree.addDialogue("apartment/start7", "riggan", "Fuck you.\n Shut up\nWhy would you do this to me!", "Apartment/start8");
	dialogue_tree.addDialogue("apartment/start8", "riggan", "*You push Mel, he falls to the floor*", "Apartment/start9");
	dialogue_tree.addDialogue("apartment/start9", "leslie", "Eddie!\nPlease don't!", "Apartment/start10");
	dialogue_tree.addDialogue("apartment/start10", "riggan", "*You pull a water gun out your jacket, then point it to Mel's head*", "Apartment/start11");
	dialogue_tree.addDialogue("apartment/start11", "riggan", "Why would you hurt my feelings like this.\n Why do I end up having to be the failed third wheel?", "Apartment/start12");
	dialogue_tree.addDialogue("apartment/start12", "leslie", "Ed.\nEddie.\nPlease..... Give me the gun.", "Apartment/start13");
	dialogue_tree.addDialogue("apartment/start13", "leslie", "*Leslie begin to cry*", "Apartment/start14");
	dialogue_tree.addDialogue("apartment/start14", "leslie", "If you really want me, then have me.\nJust dont hurt Mel.\n I am not capable of...", "Apartment/start15");
	dialogue_tree.addDialogue("apartment/start15", "leslie", "You deserve to be loved.\n You do and I even mean not as a third wheel.", "Apartment/start16");
	dialogue_tree.addDialogue("apartment/start16", "riggan", "I just wanted to be what you wanted.", "Apartment/start17");
	dialogue_tree.addDialogue("apartment/start17", "riggan", "Now i spend every fucking minute thinking of this fake bastard.\nI can't believe you left me for him.\nAnyone else but him", "Apartment/start18");
	dialogue_tree.addDialogue("apartment/start18", "mike", "Put down the gun, Ed.\nDon't do anything stupid.", "Apartment/start19");
	dialogue_tree.addDialogue("apartment/start19", "riggan", "*You spray water into Mel's face*", "Apartment/start20");
	dialogue_tree.addDialogue("apartment/start20", "mike", "*Mel's makeup caked face starts to run, it now shows the monster beneath*", "Apartment/start21");
	dialogue_tree.addDialogue("apartment/start21", "riggan", "Oh my god, you left this for me\nI can't even look at you Terri\n I can't believe i almost had a threesome with that...", "Apartment/start22");
	dialogue_tree.addDialogue("apartment/start22", "riggan", "*You walk out the room\n You are so shocked and disgusted that you might shoot yourself*", "Apartment/start20");
	

	//If Light Option - Pick 1 - (done)
	dialogue_tree.addDialogue("kitchen/light/start9", "riggan", "Apologises for being late.\nWell he might of loved her deep down.\nI've only heard his name mentioned in passing.", "kitchen/light/start10");
	dialogue_tree.addDialogue("kitchen/light/start10", "riggan", "He didnt seem like to bad a man then.\nBut I don't think that's really love.\nYou don't try to kill people you love!", "kitchen/light/start11");
	dialogue_tree.addDialogue("kitchen/light/start11", "ralph", "Exactly, it's like you've read my mind.", "kitchen/light/start12");
	dialogue_tree.addDialogue("kitchen/light/start12", "ralph", "With real love, you don't try and kill people.", "kitchen/light/start13");
	dialogue_tree.addDialogue("kitchen/light/start13", "leslie", "It was love, Mel.\nTo Eddie, it was.", "kitchen/light/start14");
	dialogue_tree.addDialogue("kitchen/light/start14", "leslie", "I don't care what anybody says.\nHe was ready to die for it.", "kitchen/light/start15");
	dialogue_tree.addDialogue("kitchen/light/start15", "ralph", "Ask her what he did after she left him.", "kitchen/light/start16");
	dialogue_tree.addDialogue("kitchen/light/start16", "leslie", "He shot himself in the mouth.\nBut he screwed that up, too.\nPoor Ed.", "kitchen/light/start17");
	dialogue_tree.addDialogue("kitchen/light/start17", "ralph", "Poor Ed, my ass.\nThe guy was dangerous.", "kitchen/light/start18");
	dialogue_tree.addDialogue("kitchen/light/start18", "laura", "How'd he screw it up if he shot himself in the mouth?", "kitchen/light/start19");
	dialogue_tree.addDialogue("kitchen/light/start19", "ralph", "He used to carry this twenty-two.\nWe lived like fugitives those days.\n", "kitchen/light/start20");
	dialogue_tree.addDialogue("kitchen/light/start20", "ralph", "I never knew if he was going to come\nout of the bushes or from behind\na car and just start shooting.", "kitchen/light/start21");
	dialogue_tree.addDialogue("kitchen/light/start21", "ralph", "The man was crazy.\nHe was capable of anything.", "kitchen/light/start22");
	dialogue_tree.addDialogue("kitchen/light/start22", "laura", "Christ. What a nightmare...", "kitchen/light/start23");
	dialogue_tree.addDialogue("kitchen/light/start23", "ralph", "He used to call me at the hospital and say...", "kitchen/light/start24");
	dialogue_tree.addDialogue("kitchen/light/start24", "ralph", "\"Son of a bitch. Your days are numbered.\"", "");

	//If Light Option - Pick 2 - ()
	dialogue_tree.addDialogue("apartment/start1", "riggan", "Terri!\nCome answer the door Terri, we need to talk!\nI know you're in there.", "Apartment/start2");
	dialogue_tree.addDialogue("apartment/start2", "riggan", "*You repetitively knock on the door\nAs you try to force your way in\nYou find the door is open.*", "Apartment/start3");
	dialogue_tree.addDialogue("apartment/start3", "riggan", "Terri????", "Apartment/start4");
	dialogue_tree.addDialogue("apartment/start4", "leslie", "Ed!...\nWhat are you doing here?", "Apartment/start5");
	dialogue_tree.addDialogue("apartment/start5", "riggan", "I need you to tell me why.\nWhy would you do this to me\nI lived for you, I worshipped you...", "Apartment/start6");
	dialogue_tree.addDialogue("apartment/start6", "mike", "Listen Ed, I know you're in a bad place right now...", "Apartment/start7");
	dialogue_tree.addDialogue("apartment/start7", "riggan", "Shut up\nI don't want to hear anything from you!", "Apartment/start8");
	dialogue_tree.addDialogue("apartment/start8", "riggan", "*You push Mel, he falls to the floor*", "Apartment/start9");
	dialogue_tree.addDialogue("apartment/start9", "leslie", "Eddie!\nPlease!", "Apartment/start10");
	dialogue_tree.addDialogue("apartment/start10", "riggan", "*You point your gun towards Mel*", "Apartment/start11");
	dialogue_tree.addDialogue("apartment/start11", "riggan", "What's wrong with me.\n Why do I end up having to beg people to love me?", "Apartment/start12");
	dialogue_tree.addDialogue("apartment/start12", "leslie", "Ed.\nEddie.\nPlease..... Give me the gun.", "Apartment/start13");
	dialogue_tree.addDialogue("apartment/start13", "leslie", "*Leslie begin to cry*", "Apartment/start14");
	dialogue_tree.addDialogue("apartment/start14", "leslie", "Just look at me.\n I was drowning.\n I was not capable of...", "Apartment/start15");
	dialogue_tree.addDialogue("apartment/start15", "leslie", "You deserve to be loved.\n You do.", "Apartment/start16");
	dialogue_tree.addDialogue("apartment/start16", "riggan", "I just wanted to be what you wanted, it all i ever wanted.\nI did it for you!.", "Apartment/start17");
	dialogue_tree.addDialogue("apartment/start17", "riggan", "Now i spend every fucking minute praying to be somebody else.\nSomeone I'm not.\n I love you, after all i've done for you", "Apartment/start18");
	dialogue_tree.addDialogue("apartment/start18", "mike", "Put down the gun, Ed.\nShe just doesn't love you anymore.\"", "");

	//If Dark Option - Pick 1 - (done)
	dialogue_tree.addDialogue("kitchen/dark/start9", "riggan", "He beat you?! If he did, you should\nof stuck up for yourself.\nI think if you...", "kitchen/dark/start10");
	dialogue_tree.addDialogue("kitchen/dark/start10", "riggan", "had beaten him till he\nwas on deaths door, he would learnt his lesson\nHe deserves it.", "kitchen/dark/start11");
	dialogue_tree.addDialogue("kitchen/dark/start11", "ralph", "I don't think that's the correct thing to do...", "kitchen/dark/start12");
	dialogue_tree.addDialogue("kitchen/dark/start12", "ralph", "What I was try to say was that\nthe kind of love I'm talking about\nyou don't try and kill people.", "kitchen/dark/start13");
	dialogue_tree.addDialogue("kitchen/dark/start13", "leslie", "It was love, Mel.\nTo Eddie, it was.", "kitchen/dark/start14");
	dialogue_tree.addDialogue("kitchen/dark/start14", "leslie", "I don't care what anybody says.\nHe was ready to die for it.", "kitchen/dark/start15");
	dialogue_tree.addDialogue("kitchen/dark/start15", "ralph", "Ask her what he did after she left him.", "kitchen/dark/start16");
	dialogue_tree.addDialogue("kitchen/dark/start16", "leslie", "He shot himself in the mouth.\nBut he screwed that up, too.\nPoor Ed.", "kitchen/dark/start17");
	dialogue_tree.addDialogue("kitchen/dark/start17", "ralph", "Poor Ed, my ass.\nThe guy was dangerous.", "kitchen/dark/start18");
	dialogue_tree.addDialogue("kitchen/dark/start18", "laura", "How'd he screw it up if he shot himself in the mouth?", "kitchen/dark/start19");
	dialogue_tree.addDialogue("kitchen/dark/start19", "ralph", "He used to carry this twenty-two.\nWe lived like fugitives those days.\n", "kitchen/dark/start20");
	dialogue_tree.addDialogue("kitchen/dark/start20", "ralph", "I never knew if he was going to come\nout of the bushes or from behind\na car and just start shooting.", "kitchen/dark/start21");
	dialogue_tree.addDialogue("kitchen/dark/start21", "ralph", "The man was crazy.\nHe was capable of anything.", "kitchen/dark/start22");
	dialogue_tree.addDialogue("kitchen/dark/start22", "laura", "Christ. What a nightmare...", "kitchen/dark/start23");
	dialogue_tree.addDialogue("kitchen/dark/start23", "ralph", "He used to call me at the hospital and say...", "kitchen/dark/start24");
	dialogue_tree.addDialogue("kitchen/dark/start24", "ralph", "\"Son of a bitch. Your days are numbered.\"", "");

	//If Dark Option - Pick 2 - ()
	dialogue_tree.addDialogue("apartment/start2", "riggan", "*You bang on the door loudly with force and open the door*", "Apartment/start3");
	dialogue_tree.addDialogue("apartment/start3", "riggan", "Terri????", "Apartment/start4");
	dialogue_tree.addDialogue("apartment/start4", "leslie", "Ed!...\n What are you doing here?", "Apartment/start5");
	dialogue_tree.addDialogue("apartment/start5", "riggan", "Why? I need you to tell me why.\n I lived for you, I worshipped you...", "Apartment/start6");
	dialogue_tree.addDialogue("apartment/start6", "mike", "Listen Ed, I know this is hard but...", "Apartment/start7");
	dialogue_tree.addDialogue("apartment/start7", "riggan", "Fuck you.\n Shut up\nI'll fucking kill you!", "Apartment/start8");
	dialogue_tree.addDialogue("apartment/start8", "riggan", "*You punch Mel, he falls to the floor*", "Apartment/start9");
	dialogue_tree.addDialogue("apartment/start9", "leslie", "Eddie!\nPlease!", "Apartment/start10");
	dialogue_tree.addDialogue("apartment/start10", "riggan", "*You point the gun at Mel's head*", "Apartment/start11");
	dialogue_tree.addDialogue("apartment/start11", "riggan", "What's wrong with me.\nWhat have i done wrong.\n Why don't you want me?", "Apartment/start12");
	dialogue_tree.addDialogue("apartment/start12", "leslie", "Ed.\nEddie.\nPlease..... Give me the gun.\nThen we can talk about this .", "Apartment/start13");
	dialogue_tree.addDialogue("apartment/start13", "leslie", "*Leslie begin to cry*", "Apartment/start14");
	dialogue_tree.addDialogue("apartment/start14", "leslie", "Just look at me.\n I was drowning.\n I was not capable of...", "Apartment/start15");
	dialogue_tree.addDialogue("apartment/start15", "leslie", "You deserve to be loved.\n You do i mean it.\nCalm down please, lets just talk.", "Apartment/start16");
	dialogue_tree.addDialogue("apartment/start16", "riggan", "I just wanted to be what you wanted\nSo why don't you want me.", "Apartment/start17");
	dialogue_tree.addDialogue("apartment/start17", "riggan", "Now i spend every fucking minute praying to be somebody else.\nSomeone I'm not.\nAll of that for you!", "Apartment/start18");
	dialogue_tree.addDialogue("apartment/start18", "mike", "Put down the gun, Ed.\nShe just doesn't love you anymore\nYou need to move on.\"", "");
}