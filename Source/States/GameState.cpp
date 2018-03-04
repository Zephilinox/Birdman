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
#include "../Messages/AudioChangeMessage.hpp"
#include "PlayEndState.h"

GameState::GameState(GameData* game_data)
	: BaseState(game_data)
	, visual_dialogue(game_data, &dialogue_tree, "kitchen/start0")
	, play_01(game_data)
	, bottom_panel(game_data->getRenderer()->createRawSprite())
	, top_panel(game_data->getRenderer()->createRawSprite())
{
	game_data->getAudioManager()->reset();
	game_data->getAudioManager()->play("FF7.wav", true);

	bottom_panel->loadTexture("../../Resources/Textures/UI/BottomPanel.png");
	bottom_panel->yPos(468);
	
	top_panel->loadTexture("../../Resources/Textures/UI/TopPanel.png");

	play_01.create();
	//dialogue_init();
	dialogue_kitchen();
	visual_dialogue.updateTree();

	managed_con = game_data->getMessageQueue()->addListener(
	[&](Message* msg)
	{
		if (msg->id == AudioChangeMessage::ID)
		{
			const auto* audio_msg = static_cast<AudioChangeMessage*>(msg);
			current_music_path = audio_msg->audio_path;
		}
	});
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
		game_data->getStateManager()->push<PauseState>(current_music_path);
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
/*
void GameState::dialogue_init()
{
	dialogue_tree.getActor("player")->realName = "TeamBirb";
	auto spr = game_data->getRenderer()->createUniqueSprite();
	spr->loadTexture("../../Resources/Textures/mel/spl1.png");
	dialogue_tree.getActor("player")->portrait = std::move(spr);

	dialogue_tree.getActor("strange_npc")->realName = "Stranger";
	spr = game_data->getRenderer()->createUniqueSprite();
	spr->loadTexture("../../Resources/Textures/terri/hpl1.png");
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

	dialogue_tree.addPlayerOption("start_extra", "Kitchen Scene", "kitchen/start0");

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
}*/

void GameState::dialogue_kitchen()
{
	auto nick = dialogue_tree.getActor("nick");
	nick->realName = "Nick (You)";
	auto nick_pic = game_data->getRenderer()->createUniqueSprite();
	nick_pic->loadTexture("../../Resources/Textures/Clint/cpl1.png");
	nick->portrait = std::move(nick_pic);

	auto ed = dialogue_tree.getActor("ed");
	ed->realName = "Ed (You)";
	auto ed_pic = game_data->getRenderer()->createUniqueSprite();
	ed_pic->loadTexture("../../Resources/Textures/Clint/cpl1.png");
	ed->portrait = std::move(ed_pic);

	auto terri = dialogue_tree.getActor("terri");
	auto terri_pic = game_data->getRenderer()->createUniqueSprite();
	terri_pic->loadTexture("../../Resources/Textures/Leslie/hpl1.png");
	terri->portrait = std::move(terri_pic);

	auto laura = dialogue_tree.getActor("laura");
	auto laura_pic = game_data->getRenderer()->createUniqueSprite();
	laura_pic->loadTexture("../../Resources/Textures/Laura/ppl1.png");
	laura->portrait = std::move(laura_pic);

	auto mel = dialogue_tree.getActor("mel");
	auto mel_pic = game_data->getRenderer()->createUniqueSprite();
	mel_pic->loadTexture("../../Resources/Textures/Mike/spl1.png");
	mel->portrait = std::move(mel_pic);

	/* BIG FAT REMINDER BIG FAT REMINDER BIG FAT REMINDER BIG FAT REMINDER BIG FAT REMINDER BIG FAT REMINDER BIG FAT REMINDER BIG FAT REMINDER BIG FAT REMINDER
	// double check all spelling/grammer
	// link up all the dialogue to the audience stuff
	// create the player options for each decision
	// test all dialogue fits within the panel
	 BIG FAT REMINDER BIG FAT REMINDER BIG FAT REMINDER BIG FAT REMINDER BIG FAT REMINDER BIG FAT REMINDER BIG FAT REMINDER BIG FAT REMINDER BIG FAT REMINDER*/

	dialogue_tree.addDialogue("kitchen/start0", "",
		[&]()
	{
		return play_01.getScene()->getDescription();
	},
		[&]()
	{
		return "kitchen/start";
	});

	dialogue_tree.addDialogue("kitchen/start", "terri",
		[&]()
	{
		auto terri = play_01.getScene()->getCharacter(Play::LESLIE);
		//terri->setFacing(Character::CharacterFacing::SOUTH);
		//terri->setPosition(400, 300);

		return "He loved me.";
	}, "kitchen/start1");

	//Intro Scene

	dialogue_tree.addDialogue("kitchen/start1", "mel", "Yeah. He loved her so much he tried to kill her.", "kitchen/start2");
	dialogue_tree.addDialogue("kitchen/start2", "laura", "He tried to kill you?", "kitchen/start3");
	dialogue_tree.addDialogue("kitchen/start3", "terri", "No.\nOkay, well he did beat me up one night.", "kitchen/start4");
	dialogue_tree.addDialogue("kitchen/start4", "terri", "He dragged me around the living room by my ankles, yelling\n\"I Love you, I love you, bitch.\"", "kitchen/start5");
	dialogue_tree.addDialogue("kitchen/start5", "mel", "What do you do with a love like that?", "kitchen/start6");
	dialogue_tree.addDialogue("kitchen/start6", "mel", "How is that...\nThat is not love and you know it.\nWhy do you insist on calling it...", "kitchen/start7");
	dialogue_tree.addDialogue("kitchen/start7", "terri", "You can say what you want, but I know what it was.", "kitchen/start8");

	dialogue_tree.addDialogue("kitchen/start8", "mel",
		[&]()
	{
		auto riggan = play_01.getScene()->getCharacter(Play::RIGGAN);
		riggan->setIsActive(true);
		riggan->setFacing(Character::CharacterFacing::WEST);
		riggan->setPosition(1100, 280);
		riggan->fastMoveToPosition(730, 280);

		return "What about you, Nick?\nDoes that sound like love to you?";
	}, "kitchen/option1");

	dialogue_tree.addPlayerOption("kitchen/option1", "Try to avoid the question", "kitchen/sad/start9");
	dialogue_tree.addPlayerOption("kitchen/option1", "Make light of the situation", "kitchen/comedy/start9");
	dialogue_tree.addPlayerOption("kitchen/option1", "Give a sincere reply", "kitchen/light/start9");
	dialogue_tree.addPlayerOption("kitchen/option1", "Give some poor advice on the mattere", "kitchen/dark/start9");

	//If Sad Option  - Pick 1 - (done)

	dialogue_tree.addDialogue("kitchen/sad/start9", "nick", 
		[&]()
	{
		game_data->getAudioManager()->reset();
		game_data->getAudioManager()->play("Piano Loop.wav", true);
		game_data->getMessageQueue()->sendMessage<AudioChangeMessage>("Piano Loop.wav");

		return "Sorry I'm late.\nI'm the wrong person to ask.\nI've only heard his name mentioned in passing.";
	}, "kitchen/sad/start10");

	dialogue_tree.addDialogue("kitchen/sad/start10", "nick", "You'd have to know the particulars,\nBut I think what you're saying is\nthat love is absolute.", "kitchen/sad/start11");
	dialogue_tree.addDialogue("kitchen/sad/start11", "mel", "Yeah. The kind of love I'm talking about is...", "kitchen/sad/start12");
	dialogue_tree.addDialogue("kitchen/sad/start12", "mel", 
		[&]()
	{
		auto mel = play_01.getScene()->getCharacter(Play::MIKE);
		mel->setFacing(Character::CharacterFacing::SOUTH);

		return "The kind of love I'm talking about,\n you don't try and kill people.";
	}, "kitchen/sad/start13");
	dialogue_tree.addDialogue("kitchen/sad/start13", "terri", "It was love, Mel.\nTo Eddie, it was.", "kitchen/sad/start14");
	dialogue_tree.addDialogue("kitchen/sad/start14", "terri",
		[&]()
	{
		auto mel = play_01.getScene()->getCharacter(Play::MIKE);
		mel->setFacing(Character::CharacterFacing::EAST);

		return "I don't care what anybody says.\nHe was ready to die for it.";
	}, "kitchen/sad/start15");
	dialogue_tree.addDialogue("kitchen/sad/start15", "mel", "Ask her what he did after she left him.", "kitchen/sad/start16");
	dialogue_tree.addDialogue("kitchen/sad/start16", "terri", "He shot himself in the mouth.\nBut he screwed that up, too.\nPoor Ed.", "kitchen/sad/start17");
	dialogue_tree.addDialogue("kitchen/sad/start17", "mel", "Poor Ed, my ass.\nThe guy was dangerous.", "kitchen/sad/start18");
	dialogue_tree.addDialogue("kitchen/sad/start18", "laura", "How'd he screw it up if he shot himself in the mouth?", "kitchen/sad/start19");
	dialogue_tree.addDialogue("kitchen/sad/start19", "mel", "He used to carry this twenty-two.\nWe lived like fugitives those days.\n", "kitchen/sad/start20");
	dialogue_tree.addDialogue("kitchen/sad/start20", "mel", "I never knew if he was going to come\nout of the bushes or from behind\na car and just start shooting.", "kitchen/sad/start21");
	dialogue_tree.addDialogue("kitchen/sad/start21", "mel", "The man was crazy.\nHe was capable of anything.", "kitchen/sad/start22");
	dialogue_tree.addDialogue("kitchen/sad/start22", "laura",
		[&]()
	{
		auto mover = play_01.getScene()->getCharacter(Play::LAURA);
		mover->setFacing(Character::CharacterFacing::NORTH);
		mover->fastMoveToPosition(mover->getXPosition(), mover->getYPosition() - 90.0f);

		return "Christ. What a nightmare...";
	}, "kitchen/sad/start23");
	dialogue_tree.addDialogue("kitchen/sad/start23", "mel", "He used to call me at the hospital and say...", "kitchen/sad/start24");
	dialogue_tree.addDialogue("kitchen/sad/start24", "mel",
		[&]()
	{
		auto mover = play_01.getScene()->getCharacter(Play::LAURA);
		mover->setFacing(Character::CharacterFacing::SOUTH);
		return "Son of a bitch. Your days are numbered.";
	}, "kitchen/sad/start25");

	dialogue_tree.addDialogue("kitchen/sad/start25", "mel", "The maniac shot himself right in front of us\nI rode with him in the ambulance to the hospital.", "kitchen/sad/start26");
	dialogue_tree.addDialogue("kitchen/sad/start26", "terri", "I'll never get that image out of my head.\nRight before he did it, his eyes...\nthey were so sad.", "kitchen/sad/start27");
	dialogue_tree.addDialogue("kitchen/sad/start27", "laura",
		[&]()
	{
		auto mover = play_01.getScene()->getCharacter(Play::LAURA);
		mover->setFacing(Character::CharacterFacing::SOUTH);
		mover->fastMoveToPosition(mover->getXPosition(), mover->getYPosition() + 90.0f);
		return "Did you have to treat him?";

	}, "kitchen/sad/start28");
	dialogue_tree.addDialogue("kitchen/sad/start28", "mel","I didn't have to, But i did.\nHe was in bad shape.", "kitchen/sad/start29");
	dialogue_tree.addDialogue("kitchen/sad/start29", "mel",
		[&]()
	{
		auto mover = play_01.getScene()->getCharacter(Play::LAURA);
		mover->setFacing(Character::CharacterFacing::WEST);
		mover = play_01.getScene()->getCharacter(Play::MIKE);
		mover->setFacing(Character::CharacterFacing::SOUTH);
		mover = play_01.getScene()->getCharacter(Play::RIGGAN);
		mover->setFacing(Character::CharacterFacing::SOUTH);
		return "His head swelled up to like twice the size of a normal head.\nI'd never seen anything like it.";

	}, "kitchen/sad/start30");
	dialogue_tree.addDialogue("kitchen/sad/start30", "mel", "And I swear to God.\nI hope I never have to do it again", "kitchen/sad/start31");
	dialogue_tree.addDialogue("kitchen/sad/start31", "mel",
		[&]()
	{
		auto mover = play_01.getScene()->getCharacter(Play::MIKE);
		mover->setFacing(Character::CharacterFacing::EAST);
		return "Ask Nick what real love is.\nHe'll agree with me, You watch";

	}, "kitchen/sad/start32");
	dialogue_tree.addDialogue("kitchen/sad/start32", "terri", "Why don't we just head to the restaurant?", "kitchen/sad/start33");
	dialogue_tree.addDialogue("kitchen/sad/start33", "laura", "Don't get him started, Mel.\nYou haven't seen how he's been lately.\nHe's been depressed.", "kitchen/sad/start34");
	dialogue_tree.addDialogue("kitchen/sad/start34", "laura", "I'm worried about him, he's been...", "kitchen/sad/start35");
	dialogue_tree.addDialogue("kitchen/sad/start35", "nick",
		[&]()
	{
		auto mover = play_01.getScene()->getCharacter(Play::RIGGAN);
		mover->setFacing(Character::CharacterFacing::EAST);
		mover->slowMoveToPosition(mover->getXPosition() + 200.0f, mover->getYPosition());
		return  "Been what?\n I'll tell you what real love is\nThis happened a few months ago.";

	}, "kitchen/sad/start36");
	dialogue_tree.addDialogue("kitchen/sad/start36", "nick", "And it ought to make us ashamed when we talk\nlike we know what we're talking about,\nwhen we talk about love.", "kitchen/sad/start37");
	dialogue_tree.addDialogue("kitchen/sad/start37", "laura", "Nick, for God's sake. Are you getting drunk?", "kitchen/sad/start38");
	dialogue_tree.addDialogue("kitchen/sad/start38", "nick", "I don't have to be drunk to say what I think.", "kitchen/sad/start39");
	dialogue_tree.addDialogue("kitchen/sad/start39", "mel", "Nobody's drunk. We're just having a few drinks", "kitchen/sad/start40");
	dialogue_tree.addDialogue("kitchen/sad/start40", "terri", "You've had more than a few.", "kitchen/sad/start41");
	dialogue_tree.addDialogue("kitchen/sad/start41", "nick", "What are you, counting?", "kitchen/sad/start42");
	dialogue_tree.addDialogue("kitchen/sad/start42", "laura", "Don't talk like a drunk if you're not...", "kitchen/sad/start43");
	dialogue_tree.addDialogue("kitchen/sad/start43", "nick", "Shut up,\nFor once in your life.\nWill you do me a favor and shut up for a minute?\"",
	[&]()
	{
		//add an arbitrary value to the "sad" value of the audience, this is applied to a multiplier, then offset by boredom.
		play_01.getAudience()->addToSad(10);
		//add an arbitrary value to the "sad" value of the scene, this is used to determine which scene is gone to next.
		play_01.getScene()->addToSad(1);
		return "kitchen/option2";		
	});
	//If Sad Option - Pick 2 -
	dialogue_tree.addDialogue("kitchen/sad/start44", "nick", "Like I was saying...\nThere this old couple, had a car wreck out on the interstate.", "kitchen/sad/start45");
	dialogue_tree.addDialogue("kitchen/sad/start45", "nick", "Some drunk kid plowed his dad's pick up into their camper.", "kitchen/option3");

	//If Sad Option - Pick 3 - 
	dialogue_tree.addDialogue("kitchen/sad/start46", "nick", "Fucking teenager.\nBy the time I got to the hospital,\nThe kid was dead.", "kitchen/sad/start47");
	dialogue_tree.addDialogue("kitchen/sad/start47", "nick", "He was off in a corner laid out on a gurney.\nWe took the old couple up to the O.R...", "kitchen/sad/start48");
	dialogue_tree.addDialogue("kitchen/sad/start48", "nick", "They were a mess.\nWe worked like hell on them for most of the night...", "kitchen/option4");

	//If Sad Option - Pick 4 - (done)
	dialogue_tree.addDialogue("kitchen/sad/start49", "nick","When we were done,\n we wrapped then in full body casts.\nThe husband was depressed", "kitchen/sad/start50");
	dialogue_tree.addDialogue("kitchen/sad/start50", "nick","I told him his wife was gonna pull through,\nHe was still depressed.", "kitchen/sad/start51");
	dialogue_tree.addDialogue("kitchen/sad/start51", "nick", "So,\nI got up to his mouth hole and asked him why", "kitchen/sad/start52");
	dialogue_tree.addDialogue("kitchen/sad/start52", "nick", "And he told me it was because\nhe couldn't see her through the eye holes.", "kitchen/sad/start53");
	dialogue_tree.addDialogue("kitchen/sad/start53", "nick", "Can you imagine?,\nI'm telling you, the man's heart was breaking beacuse\n he couldn't turn his goddamn head and see he goddamn wife.", "apartment/sad/start0");

	//Move scene
	dialogue_tree.addDialogue("apartment/sad/start0", "",
		[&]()
	{
		play_01.moveToNextScene();
		return play_01.getNextScene()->getDescription();
	},
		[&]()
	{
		return "apartment/sad/start1";
	});

	dialogue_tree.addDialogue("apartment/sad/start1", "ed", "Terri! Terri!\nTerri! I know you're in there!", "apartment/sad/start2");
	dialogue_tree.addDialogue("apartment/sad/start2", "ed", "*You bang on the door loudly with force. It opens.*", "apartment/sad/start3");
	dialogue_tree.addDialogue("apartment/sad/start3", "ed", "Terri????", "apartment/sad/start4");
	dialogue_tree.addDialogue("apartment/sad/start4", "terri", "Ed!...\nWhat are you doing here?", "apartment/sad/start5");
	dialogue_tree.addDialogue("apartment/sad/start5", "ed", "Why? I need you to tell me why.\nI lived for you, I worshipped you...", "apartment/sad/start6");
	dialogue_tree.addDialogue("apartment/sad/start6", "mel", "Listen Ed, I know this is hard but...", "apartment/sad/start7");
	dialogue_tree.addDialogue("apartment/sad/start7", "ed", "Fuck you.\nShut up\nFuck you!", "apartment/sad/start8");
	dialogue_tree.addDialogue("apartment/sad/start8", "ed", "*You push Mel, he falls to the floor*", "apartment/sad/start9");
	dialogue_tree.addDialogue("apartment/sad/start9", "terri", "Eddie!\nPlease!", "apartment/sad/start10");
	dialogue_tree.addDialogue("apartment/sad/start10", "ed", "*You point the gun at Mel's head*", "apartment/sad/start11");
	dialogue_tree.addDialogue("apartment/sad/start11", "ed", "What's wrong with me.\nWhy do I end up having to beg people to love me?", "apartment/sad/start12");
	dialogue_tree.addDialogue("apartment/sad/start12", "terri", "Ed.\nEddie.\nPlease..... Give me the gun.", "apartment/sad/start13");
	dialogue_tree.addDialogue("apartment/sad/start13", "terri", "*Terri begins to cry*", "apartment/sad/start14");
	dialogue_tree.addDialogue("apartment/sad/start14", "terri", "Just look at me.\nI was drowning.\nI was not capable of...", "apartment/sad/start15");
	dialogue_tree.addDialogue("apartment/sad/start15", "terri", "You deserve to be loved.\nYou do.", "apartment/sad/start16");
	dialogue_tree.addDialogue("apartment/sad/start16", "ed", "I just wanted to be what you wanted.", "apartment/sad/start17");
	dialogue_tree.addDialogue("apartment/sad/start17", "ed", "Now I spend every fucking minute praying to be somebody else.\nSomeone I'm not.\nAnyone...", "apartment/sad/start18");
	dialogue_tree.addDialogue("apartment/sad/start18", "mel", "Put down the gun, Ed.\nShe just doesn't love you anymore.\"", "apartment/option1");

	//If Sad Option - Pick 5 - ()
	dialogue_tree.addDialogue("apartment/sad/start19", "ed", "*Ed looks at Terri with a sad smile*", "apartment/sad/start20");
	dialogue_tree.addDialogue("apartment/sad/start20", "ed", "You don't, do you?", "apartment/sad/start21");
	dialogue_tree.addDialogue("apartment/sad/start21", "terri", "No...", "apartment/sad/start22");
	dialogue_tree.addDialogue("apartment/sad/start22", "ed", "And you never will...", "apartment/sad/start23");
	dialogue_tree.addDialogue("apartment/sad/start23", "terri", "I'm sorry Ed", "apartment/sad/start24");
	dialogue_tree.addDialogue("apartment/sad/start24", "ed", "I don't exist.\nI'm not even here.", "apartment/sad/start25");
	dialogue_tree.addDialogue("apartment/sad/start25", "ed", "I don't exist.\nNone of this matters.", "apartment/sad/start26");
	dialogue_tree.addDialogue("apartment/sad/start26", "ed", "*Ed points the gun at Terri...*", "apartment/sad/start27");
	dialogue_tree.addDialogue("apartment/sad/start27", "ed", "*Then towards Mel*", "apartment/sad/start28");
	dialogue_tree.addDialogue("apartment/sad/start28", "ed", "*Finally towards his own head...*", "apartment/sad/start29");
	dialogue_tree.addDialogue("apartment/sad/start29", "ed", "*Ed then pulls the trigger and drops to the floor dead!*", "next night");


	//If Comedy Option - Pick 1 - (done)

	dialogue_tree.addDialogue("kitchen/comedy/start9", "nick", "Well if you ask me.\nSeems like the guy likes tough love.\nIf thats the case reply with tough love.", "kitchen/comedy/start10");
	dialogue_tree.addDialogue("kitchen/comedy/start10", "nick", "if you still loved him.\nYou should of them smacked him with a pan or even a bat.", "kitchen/comedy/start11");
	dialogue_tree.addDialogue("kitchen/comedy/start11", "mel", "This isn't something to joke about.\nThe kind of love I'm talking about is...", "kitchen/comedy/start12");
	dialogue_tree.addDialogue("kitchen/comedy/start12", "mel", "The kind of love I'm talking about,\nyou don't try and kill people or hurt them.", "kitchen/comedy/start13");
	dialogue_tree.addDialogue("kitchen/comedy/start13", "terri", "It was love, Mel.\nTo Eddie, it was.", "kitchen/comedy/start14");
	dialogue_tree.addDialogue("kitchen/comedy/start14", "terri", "I don't care what anybody says.\nHe was ready to die for it.", "kitchen/comedy/start15");
	dialogue_tree.addDialogue("kitchen/comedy/start15", "mel", "Ask her what he did after she left him.", "kitchen/comedy/start16");
	dialogue_tree.addDialogue("kitchen/comedy/start16", "terri", "He shot himself in the mouth.\nBut he screwed that up, too.\nPoor Ed.", "kitchen/comedy/start17");
	dialogue_tree.addDialogue("kitchen/comedy/start17", "mel", "Poor Ed, my ass.\nThe guy was dangerous.", "kitchen/comedy/start18");
	dialogue_tree.addDialogue("kitchen/comedy/start18", "laura", "How'd he screw it up if he shot himself in the mouth?", "kitchen/comedy/start19");
	dialogue_tree.addDialogue("kitchen/comedy/start19", "mel", "He used to carry this twenty-two.\nWe lived like fugitives those days.\n", "kitchen/comedy/start20");
	dialogue_tree.addDialogue("kitchen/comedy/start20", "mel", "I never knew if he was going to come\nout of the bushes or from behind\na car and just start shooting.", "kitchen/comedy/start21");
	dialogue_tree.addDialogue("kitchen/comedy/start21", "mel", "The man was crazy.\nHe was capable of anything.", "kitchen/comedy/start22");
	dialogue_tree.addDialogue("kitchen/comedy/start22", "laura", "Christ. What a nightmare...", "kitchen/comedy/start23");
	dialogue_tree.addDialogue("kitchen/comedy/start23", "mel", "He used to call me at the hospital and say...", "kitchen/comedy/start24");
	dialogue_tree.addDialogue("kitchen/comedy/start24", "mel", "\"Son of a bitch. Your days are numbered.\"", "kitchen/comedy/start25");
	

	dialogue_tree.addDialogue("kitchen/comedy/start25", "mel", "The maniac shot himself right in front of us\n I rode with him in the ambulance to the hospital.", "kitchen/comedy/start26");
	dialogue_tree.addDialogue("kitchen/comedy/start26", "terri", "I'll never get that image out of my head.\n Right before he did it, his eyes...\n they were so sad.", "kitchen/comedy/start27");
	dialogue_tree.addDialogue("kitchen/comedy/start27", "laura", "Did you have to treat him?", "kitchen/comedy/start28");
	dialogue_tree.addDialogue("kitchen/comedy/start28", "mel", "I didn't have to, But i did.\nHe was in bad shape.", "kitchen/comedy/start29");
	dialogue_tree.addDialogue("kitchen/comedy/start29", "mel", "His head swelled up to like twice the size of a normal head.\nI'd never seen anything like it.", "kitchen/comedy/start30");
	dialogue_tree.addDialogue("kitchen/comedy/start30", "mel", "And I swear to God.\nThat i hope i never do again", "kitchen/comedy/start31");
	dialogue_tree.addDialogue("kitchen/comedy/start31", "mel", "Ask Nick what real love is.\nHe'll agree with me, You watch", "kitchen/comedy/start32");
	dialogue_tree.addDialogue("kitchen/comedy/start32", "terri", "Why don't we just head to the restaurant?", "kitchen/comedy/start33");
	dialogue_tree.addDialogue("kitchen/comedy/start33", "laura", "Don't get him started, Mel.\nYou haven't seen how he's been lately.\n He's been depressed.", "kitchen/comedy/start34");
	dialogue_tree.addDialogue("kitchen/comedy/start34", "laura", "I'm worried about him, he's been...", "kitchen/comedy/start35");
	dialogue_tree.addDialogue("kitchen/comedy/start35", "nick", "Been what? , I'll tell you what real love is\n This happened a few years ago back in 1982\n I was a teen still back then.", "kitchen/comedy/start36");
	dialogue_tree.addDialogue("kitchen/comedy/start36", "nick", "And it ought to make us ashamed when we talk\nWe have forgotten how to truly love unlike back then.", "kitchen/comedy/start37");
	dialogue_tree.addDialogue("kitchen/comedy/start37", "laura", "Nick, for God's sake. Are you getting drunk?", "kitchen/comedy/start38");
	dialogue_tree.addDialogue("kitchen/comedy/start38", "nick", "I don't have to be drunk to think back on good memories.", "kitchen/comedy/start39");
	dialogue_tree.addDialogue("kitchen/comedy/start39", "mel", "Nobody's drunk. We're just having a few drinks", "kitchen/comedy/start40");
	dialogue_tree.addDialogue("kitchen/comedy/start40", "terri", "You've had more than a few.", "kitchen/comedy/start41");
	dialogue_tree.addDialogue("kitchen/comedy/start41", "nick", "Why you have to be so boring,\n we are just having a bit of fun?", "kitchen/comedy/start42");
	dialogue_tree.addDialogue("kitchen/comedy/start42", "laura", "Don't talk like a drunk if you not...", "kitchen/comedy/start42");
	dialogue_tree.addDialogue("kitchen/comedy/start43", "nick", "Shut up,\nFor once in your life.\nWill you do me a favor and shut up for a minute?\"", "apartment/comedy/start0");

	//If Comedy Option - Pick 2 -
	dialogue_tree.addDialogue("kitchen/comedy/start44", "nick", "Like I was saying...\nThere this old couple, had a car wreck out on the interstate.", "kitchen/comedy/start45");
	dialogue_tree.addDialogue("kitchen/comedy/start45", "nick", "Some drunk teen crashed into them,\n because he was to busy singing\n Let it go and not looking at the road .", "kitchen/option3");

	//If Comedy Option - Pick 3 - 
	dialogue_tree.addDialogue("kitchen/comedy/start46", "nick", "Fucking teenagers man.\nOnce I'd got to the hospital,\nThe kid was already high as fuck on meds.", "kitchen/comedy/start47");
	dialogue_tree.addDialogue("kitchen/comedy/start47", "nick", "He was off in the corner rocking back and forth.\nWe took the old couple up to the O.R...", "kitchen/comedy/start48");
	dialogue_tree.addDialogue("kitchen/comedy/start48", "nick", "They were a mess.\nYou'd think they had a lads night out\nWe worked like hell on them for most of the night...", "kitchen/option4");

	//If Comedy Option - Pick 4 - (done)
	dialogue_tree.addDialogue("kitchen/comedy/start49", "nick", "Once we were done,\n they were wrapped in full body casts.\nThe husband was so fucking depressed", "kitchen/comedy/start50");
	dialogue_tree.addDialogue("kitchen/comedy/start50", "nick", "I supported him saying his wife will be fine,\nHe was still depressed.", "kitchen/comedy/start51");
	dialogue_tree.addDialogue("kitchen/comedy/start51", "nick", "So,\nI got up to his mouth hole and asked him why", "kitchen/comedy/start52");
	dialogue_tree.addDialogue("kitchen/comedy/start52", "nick", "And he told me it was because\nhe though he finally got rid of the old bag.", "kitchen/comedy/start53");
	dialogue_tree.addDialogue("kitchen/comedy/start53", "nick", "Can you imagine?,\nI'm telling you, the man's heart was breaking beacuse\nhe though he became a free man once again, but it was a lie.", "apartment/comedy/start0");

	//Move scene

	dialogue_tree.addDialogue("apartment/comedy/start0", "",
		[&]()
	{
		play_01.moveToNextScene();
		return play_01.getNextScene()->getDescription();
	},
		[&]()
	{
		return "apartment/comedy/start1";
	});

	dialogue_tree.addDialogue("apartment/comedy/start1", "ed", "Terri!\nI know your in there, I'm coming in!", "apartment/comedy/start2");
	dialogue_tree.addDialogue("apartment/comedy/start2", "ed", "*You enter through the door*", "apartment/comedy/start3");
	dialogue_tree.addDialogue("apartment/comedy/start3", "ed", "How's it going", "apartment/comedy/start4");
	dialogue_tree.addDialogue("apartment/comedy/start4", "terri", "Ed!...\n What are you doing here?", "apartment/comedy/start5");
	dialogue_tree.addDialogue("apartment/comedy/start5", "ed", "What? Did Mel not tell you?.\n Mel knows how much i worship you...\nSo he invited me for a threesome", "apartment/comedy/start6");
	dialogue_tree.addDialogue("apartment/comedy/start6", "mel", "Listen Ed, I know this is hard but...\n the threesome is off", "apartment/comedy/start7");
	dialogue_tree.addDialogue("apartment/comedy/start7", "ed", "Fuck you.\n Shut up\nWhy would you do this to me!", "apartment/comedy/start8");
	dialogue_tree.addDialogue("apartment/comedy/start8", "ed", "*You push Mel, he falls to the floor*", "apartment/comedy/start9");
	dialogue_tree.addDialogue("apartment/comedy/start9", "terri", "Eddie!\nPlease don't!", "apartment/comedy/start10");
	dialogue_tree.addDialogue("apartment/comedy/start10", "ed", "*You pull a water gun out your jacket, then point it to Mel's head*", "apartment/comedy/start11");
	dialogue_tree.addDialogue("apartment/comedy/start11", "ed", "Why would you hurt my feelings like this.\n Why do I end up having to be the failed third wheel?", "apartment/comedy/start12");
	dialogue_tree.addDialogue("apartment/comedy/start12", "terri", "Ed.\nEddie.\nPlease..... Give me the gun.", "apartment/comedy/start13");
	dialogue_tree.addDialogue("apartment/comedy/start13", "terri", "*Terri begin to cry*", "apartment/comedy/start14");
	dialogue_tree.addDialogue("apartment/comedy/start14", "terri", "If you really want me, then have me.\nJust dont hurt Mel.\n I am not capable of...", "apartment/comedy/start15");
	dialogue_tree.addDialogue("apartment/comedy/start15", "terri", "You deserve to be loved.\n You do and I even mean not as a third wheel.", "apartment/comedy/start16");
	dialogue_tree.addDialogue("apartment/comedy/start16", "ed", "I just wanted to be what you wanted.", "apartment/comedy/start17");
	dialogue_tree.addDialogue("apartment/comedy/start17", "ed", "Now i spend every fucking minute thinking of this fake bastard.\nI can't believe you left me for him.\nAnyone else but him", "apartment/comedy/start18");
	dialogue_tree.addDialogue("apartment/comedy/start18", "mel", "Put down the gun, Ed.\nDon't do anything stupid.", "apartment/comedy/start19");

	//INSERT PLAYER OPTION HERE

	//If Comedy Option - Pick 5 - ()

	dialogue_tree.addDialogue("apartment/comedy/start19", "ed", "*You spray water into Mel's face*", "apartment/comedy/start20");
	dialogue_tree.addDialogue("apartment/comedy/start20", "mel", "*Mel's makeup caked face starts to run, it now shows the monster beneath*", "apartment/comedy/start21");
	dialogue_tree.addDialogue("apartment/comedy/start21", "ed", "Oh my god, you left me for this\nI can't even look at you Terri\n I can't believe i wanted a threesome with that...", "apartment/comedy/start22");
	dialogue_tree.addDialogue("apartment/comedy/start22", "ed", "*You walk out the room\n You are so shocked and disgusted that you might shoot yourself*", "next night");


	//If Light Option - Pick 1 - (done)

	dialogue_tree.addDialogue("kitchen/light/start9", "nick", "Apologises for being late.\nWell he might of loved her deep down.\nI've only heard his name mentioned in passing.", "kitchen/light/start10");
	dialogue_tree.addDialogue("kitchen/light/start10", "nick", "He didnt seem like to bad a man then.\nBut I don't think that's really love.\nYou don't try to kill people you love!", "kitchen/light/start11");
	dialogue_tree.addDialogue("kitchen/light/start11", "mel", "Exactly, it's like you've read my mind.", "kitchen/light/start12");
	dialogue_tree.addDialogue("kitchen/light/start12", "mel", "With real love, you don't try and kill people.", "kitchen/light/start13");
	dialogue_tree.addDialogue("kitchen/light/start13", "terri", "It was love, Mel.\nTo Eddie, it was.", "kitchen/light/start14");
	dialogue_tree.addDialogue("kitchen/light/start14", "terri", "I don't care what anybody says.\nHe was ready to die for it.", "kitchen/light/start15");
	dialogue_tree.addDialogue("kitchen/light/start15", "mel", "Ask her what he did after she left him.", "kitchen/light/start16");
	dialogue_tree.addDialogue("kitchen/light/start16", "terri", "He shot himself in the mouth.\nBut he screwed that up, too.\nPoor Ed.", "kitchen/light/start17");
	dialogue_tree.addDialogue("kitchen/light/start17", "mel", "Poor Ed, my ass.\nThe guy was dangerous.", "kitchen/light/start18");
	dialogue_tree.addDialogue("kitchen/light/start18", "laura", "How'd he screw it up if he shot himself in the mouth?", "kitchen/light/start19");
	dialogue_tree.addDialogue("kitchen/light/start19", "mel", "He used to carry this twenty-two.\nWe lived like fugitives those days.\n", "kitchen/light/start20");
	dialogue_tree.addDialogue("kitchen/light/start20", "mel", "I never knew if he was going to come\nout of the bushes or from behind\na car and just start shooting.", "kitchen/light/start21");
	dialogue_tree.addDialogue("kitchen/light/start21", "mel", "The man was crazy.\nHe was capable of anything.", "kitchen/light/start22");
	dialogue_tree.addDialogue("kitchen/light/start22", "laura", "Christ. What a nightmare...", "kitchen/light/start23");
	dialogue_tree.addDialogue("kitchen/light/start23", "mel", "He used to call me at the hospital and say...", "kitchen/light/start24");
	dialogue_tree.addDialogue("kitchen/light/start24", "mel", "\"Son of a bitch. Your days are numbered.\"", "kitchen/light/start25");

	dialogue_tree.addDialogue("kitchen/light/start25", "mel", "The maniac shot himself right in front of us\n I rode with him in the ambulance to the hospital.", "kitchen/light/start26");
	dialogue_tree.addDialogue("kitchen/light/start26", "terri", "I'll never get that image out of my head.\n Right before he did it, his eyes...\n they were so sad.", "kitchen/light/start27");
	dialogue_tree.addDialogue("kitchen/light/start27", "laura", "Did you have to treat him?", "kitchen/light/start28");
	dialogue_tree.addDialogue("kitchen/light/start28", "mel", "I didn't have to, But i did.\nHe was in bad shape.", "kitchen/light/start29");
	dialogue_tree.addDialogue("kitchen/light/start29", "mel", "His head swelled up to like twice the size of a normal head.\nI'd never seen anything like it.", "kitchen/light/start30");
	dialogue_tree.addDialogue("kitchen/light/start30", "mel", "And I swear to God.\nThat i hope i never do again", "kitchen/light/start31");
	dialogue_tree.addDialogue("kitchen/light/start31", "mel", "Ask Nick what real love is.\nHe'll agree with me, You watch", "kitchen/light/start32");
	dialogue_tree.addDialogue("kitchen/light/start32", "terri", "Why don't we just head to the restaurant?", "kitchen/light/start33");
	dialogue_tree.addDialogue("kitchen/light/start33", "laura", "Don't get him started, Mel.\nYou haven't seen how he's been lately.\n He's been depressed.", "kitchen/light/start34");
	dialogue_tree.addDialogue("kitchen/light/start34", "laura", "I'm worried about him, he's been...", "kitchen/light/start35");
	dialogue_tree.addDialogue("kitchen/light/start35", "nick", "Been what?\n Im fine dont worry Laura,But I will tell you what real love is\n This happened some time ago.", "kitchen/light/start36");
	dialogue_tree.addDialogue("kitchen/light/start36", "nick", "It will make us feel ashamed when we talk about love\n we talk about love as if we know it well, but that is wrong.", "kitchen/light/start37");
	dialogue_tree.addDialogue("kitchen/light/start37", "laura", "Nick, for God's sake. Are you getting drunk?", "kitchen/light/start38");
	dialogue_tree.addDialogue("kitchen/light/start38", "nick", "No of course not just enjoying some cocktail with my friends.", "kitchen/light/start39");
	dialogue_tree.addDialogue("kitchen/light/start39", "mel", "Yeah nobodies getting drunk.\n We're just having a few drinks as friends", "kitchen/light/start40");
	dialogue_tree.addDialogue("kitchen/light/start40", "terri", "You've had more than a few.", "kitchen/light/start41");
	dialogue_tree.addDialogue("kitchen/light/start41", "nick", "Common Terri relax, have some fun?", "kitchen/light/start42");
	dialogue_tree.addDialogue("kitchen/light/start42", "laura", "Don't talk like your plan is going to get drunk then...", "kitchen/light/start42");
	dialogue_tree.addDialogue("kitchen/light/start43", "nick", "Shut up,\nFor once in your life.\nWill you do me a favor and shut up for a minute?\"", "apartment/light/start0");

	//If Light Option - Pick 2 -
	dialogue_tree.addDialogue("kitchen/light/start44", "nick", "Like I was saying...\nThere this old couple, had a car wreck out on the interstate.", "kitchen/light/start45");
	dialogue_tree.addDialogue("kitchen/light/start45", "nick", "Some drunk kid plowed his dad's pick up into their camper.", "kitchen/option3");

	//If Light Option - Pick 3 - 
	dialogue_tree.addDialogue("kitchen/light/start46", "nick", "Fucking teenager.\nBy the time I got to the hospital,\nThe kid was dead.", "kitchen/light/start47");
	dialogue_tree.addDialogue("kitchen/light/start47", "nick", "He was off in a corner laid out on a gurney.\nWe took the old couple up to the O.R...", "kitchen/light/start48");
	dialogue_tree.addDialogue("kitchen/light/start48", "nick", "They were a mess.\nWe worked like hell on them for most of the night...", "kitchen/option4");

	//If Light Option - Pick 4 - (done)
	dialogue_tree.addDialogue("kitchen/light/start49", "nick", "When we were done,\n we wrapped then in full body casts.\nThe husband was depressed", "kitchen/light/start50");
	dialogue_tree.addDialogue("kitchen/light/start50", "nick", "I told him his wife was gonna pull through,\nHe was still depressed.", "kitchen/light/start51");
	dialogue_tree.addDialogue("kitchen/light/start51", "nick", "So,\nI got up to his mouth hole and asked him why", "kitchen/sad/start52");
	dialogue_tree.addDialogue("kitchen/light/start52", "nick", "And he told me it was because\nhe couldn't see her through the eye holes.", "kitchen/sad/start53");
	dialogue_tree.addDialogue("kitchen/light/start53", "nick", "Can you imagine?,\nI'm telling you, the man's heart was breaking beacuse\n he couldn't turn his goddamn head and see he goddamn wife.", "apartment/light/start0");

	//Move scene

	dialogue_tree.addDialogue("apartment/light/start0", "",
	[&]()
	{
		play_01.moveToNextScene();
		return play_01.getNextScene()->getDescription();
	},
	[&]()
	{
		return "apartment/light/start1";
	});

	dialogue_tree.addDialogue("apartment/light/start1", "ed", "Terri!\nCome answer the door Terri, we need to talk!\nI know you're in there.", "apartment/light/start2");
	dialogue_tree.addDialogue("apartment/light/start2", "ed", "*You repetitively knock on the door\nAs you try to force your way in\nYou find the door is open.*", "apartment/light/start3");
	dialogue_tree.addDialogue("apartment/light/start3", "ed", "Terri????", "apartment/light/start4");
	dialogue_tree.addDialogue("apartment/light/start4", "terri", "Ed!...\nWhat are you doing here?", "apartment/light/start5");
	dialogue_tree.addDialogue("apartment/light/start5", "ed", "I need you to tell me why.\nWhy would you do this to me\nI lived for you, I worshipped you...", "apartment/light/start6");
	dialogue_tree.addDialogue("apartment/light/start6", "mel", "Listen Ed, I know you're in a bad place right now...", "apartment/light/start7");
	dialogue_tree.addDialogue("apartment/light/start7", "ed", "Shut up\nI don't want to hear anything from you!", "apartment/light/start8");
	dialogue_tree.addDialogue("apartment/light/start8", "ed", "*You push Mel, he falls to the floor*", "apartment/light/start9");
	dialogue_tree.addDialogue("apartment/light/start9", "terri", "Eddie!\nPlease!", "apartment/light/start10");
	dialogue_tree.addDialogue("apartment/light/start10", "ed", "*You point your gun towards Mel*", "apartment/light/start11");
	dialogue_tree.addDialogue("apartment/light/start11", "ed", "What's wrong with me.\n Why do I end up having to beg people to love me?", "apartment/light/start12");
	dialogue_tree.addDialogue("apartment/light/start12", "terri", "Ed.\nEddie.\nPlease..... Give me the gun.", "apartment/light/start13");
	dialogue_tree.addDialogue("apartment/light/start13", "terri", "*Terri begin to cry*", "apartment/light/start14");
	dialogue_tree.addDialogue("apartment/light/start14", "terri", "Just look at me.\n I was drowning.\n I was not capable of...", "apartment/light/start15");
	dialogue_tree.addDialogue("apartment/light/start15", "terri", "You deserve to be loved.\n You do.", "apartment/light/start16");
	dialogue_tree.addDialogue("apartment/light/start16", "ed", "I just wanted to be what you wanted, it all i ever wanted.\nI did it for you!.", "apartment/light/start17");
	dialogue_tree.addDialogue("apartment/light/start17", "ed", "Now i spend every fucking minute praying to be somebody else.\nSomeone I'm not.\n I love you, after all i've done for you", "apartment/light/start18");
	dialogue_tree.addDialogue("apartment/light/start18", "mel", "Put down the gun, Ed.\nShe just doesn't love you anymore.\"", "apartment/light/start19");

	//If Light Option - Pick 5 - ()

	dialogue_tree.addDialogue("apartment/light/start19", "ed", "I'm really sorry Terri\n Please just give me another chance\nI'll do anything for you i can change!", "apartment/light/start20");
	dialogue_tree.addDialogue("apartment/light/start20", "terri", "*I don't know Ed....\n I don't want to get hurt like before...", "apartment/light/start21");
	dialogue_tree.addDialogue("apartment/light/start21", "ed", "Please Terri, I'm begging you.\nPlease just give me one more chance you won't regret it.", "apartment/light/start22");
	dialogue_tree.addDialogue("apartment/light/start22", "mel", "Terri...\n You can't seriously be thinking about it\nThat insane after everything he's just done?!", "apartment/light/start23");
	dialogue_tree.addDialogue("apartment/light/start23", "terri", "Be quiet Mel!\nYou don't understand how much i love him.", "apartment/light/start24");
	dialogue_tree.addDialogue("apartment/light/start24", "mel", "Fine, I'll leave!\nI don't want to be apart of this mess!", "apartment/light/start25");
	dialogue_tree.addDialogue("apartment/light/start25", "mel", "*Mel storms out the apartment*", "apartment/light/start26");
	dialogue_tree.addDialogue("apartment/light/start26", "ed", "Terri so does this mean...\nYou'll give me another chance?", "apartment/light/start27");
	dialogue_tree.addDialogue("apartment/light/start27", "terri", "Yes Ed i will,\nBut if you ever hurt me again it over ok", "apartment/light/start28");
	dialogue_tree.addDialogue("apartment/light/start28", "ed", "Don't worry I won't!\nI love you Terri!", "next night");

	//If Dark Option - Pick 1 - (done)
	dialogue_tree.addDialogue("kitchen/dark/start9", "nick", "He beat you?! If he did, you should\nof stuck up for yourself.\nI think if you...", "kitchen/dark/start10");
	dialogue_tree.addDialogue("kitchen/dark/start10", "nick", "had beaten him till he\nwas on deaths door, he would learnt his lesson\nHe deserves it.", "kitchen/dark/start11");
	dialogue_tree.addDialogue("kitchen/dark/start11", "mel", "I don't think that's the correct thing to do...", "kitchen/dark/start12");
	dialogue_tree.addDialogue("kitchen/dark/start12", "mel", "What I was try to say was that\nthe kind of love I'm talking about\nyou don't try and kill people.", "kitchen/dark/start13");
	dialogue_tree.addDialogue("kitchen/dark/start13", "terri", "It was love, Mel.\nTo Eddie, it was.", "kitchen/dark/start14");
	dialogue_tree.addDialogue("kitchen/dark/start14", "terri", "I don't care what anybody says.\nHe was ready to die for it.", "kitchen/dark/start15");
	dialogue_tree.addDialogue("kitchen/dark/start15", "mel", "Ask her what he did after she left him.", "kitchen/dark/start16");
	dialogue_tree.addDialogue("kitchen/dark/start16", "terri", "He shot himself in the mouth.\nBut he screwed that up, too.\nPoor Ed.", "kitchen/dark/start17");
	dialogue_tree.addDialogue("kitchen/dark/start17", "mel", "Poor Ed, my ass.\nThe guy was dangerous.", "kitchen/dark/start18");
	dialogue_tree.addDialogue("kitchen/dark/start18", "laura", "How'd he screw it up if he shot himself in the mouth?", "kitchen/dark/start19");
	dialogue_tree.addDialogue("kitchen/dark/start19", "mel", "He used to carry this twenty-two.\nWe lived like fugitives those days.\n", "kitchen/dark/start20");
	dialogue_tree.addDialogue("kitchen/dark/start20", "mel", "I never knew if he was going to come\nout of the bushes or from behind\na car and just start shooting.", "kitchen/dark/start21");
	dialogue_tree.addDialogue("kitchen/dark/start21", "mel", "The man was crazy.\nHe was capable of anything.", "kitchen/dark/start22");
	dialogue_tree.addDialogue("kitchen/dark/start22", "laura", "Christ. What a nightmare...", "kitchen/dark/start23");
	dialogue_tree.addDialogue("kitchen/dark/start23", "mel", "He used to call me at the hospital and say...", "kitchen/dark/start24");

	dialogue_tree.addDialogue("kitchen/dark/start24", "mel", "\"Son of a bitch. Your days are numbered.", "kitchen/dark/start25");
	dialogue_tree.addDialogue("kitchen/dark/start25", "mel", "The maniac shot himself right in front of us\n I rode with him in the ambulance to the hospital.", "kitchen/dark/start26");
	dialogue_tree.addDialogue("kitchen/dark/start26", "terri", "I'll never get that image out of my head.\n Right before he did it, his eyes...\n they were so sad.", "kitchen/dark/start27");
	dialogue_tree.addDialogue("kitchen/dark/start27", "laura", "Did you have to treat him?", "kitchen/dark/start28");
	dialogue_tree.addDialogue("kitchen/dark/start28", "mel", "I didn't have to, But i did.\nHe was in bad shape.", "kitchen/dark/start29");
	dialogue_tree.addDialogue("kitchen/dark/start29", "mel", "His head swelled up to like twice the size of a normal head.\nI'd never seen anything like it.", "kitchen/dark/start30");
	dialogue_tree.addDialogue("kitchen/dark/start30", "mel", "And I swear to God.\nThat i hope i never do again", "kitchen/dark/start31");
	dialogue_tree.addDialogue("kitchen/dark/start31", "mel", "Ask Nick what real love is.\nHe'll agree with me, You watch", "kitchen/dark/start32");
	dialogue_tree.addDialogue("kitchen/dark/start32", "terri", "Why don't we just head to the restaurant?", "kitchen/dark/start33");
	dialogue_tree.addDialogue("kitchen/dark/start33", "laura", "Don't get him started, Mel.\nYou haven't seen how he's been lately.\n He's been depressed.", "kitchen/dark/start34");
	dialogue_tree.addDialogue("kitchen/dark/start34", "laura", "I'm worried about him, he's been...", "kitchen/dark/start35");
	dialogue_tree.addDialogue("kitchen/dark/start35", "nick", "Been what? Have you been stalking me?\n What do you know?!, But yeah real love is pain\n it first happened a few months ago.", "kitchen/dark/start36");
	dialogue_tree.addDialogue("kitchen/dark/start36", "nick", "And it ought to make you guys ashamed to not know\n you talk about knowing what love is but you dont really know real love\n but i do now, i can show you.", "kitchen/dark/start37");
	dialogue_tree.addDialogue("kitchen/dark/start37", "laura", "Nick, for God's sake. Are you drunk?, You speaking nonse", "kitchen/dark/start38");
	dialogue_tree.addDialogue("kitchen/dark/start38", "nick", "If I want to drink myself to death let me\n It's my problem not yours, mind your own business.", "kitchen/dark/start39");
	dialogue_tree.addDialogue("kitchen/dark/start39", "mel", "We're just having a few drinks as mates, nobody's drunk\n Isn't that right Nick just to mates having a drink is all", "kitchen/dark/start40");
	dialogue_tree.addDialogue("kitchen/dark/start40", "terri", "You say that but I've seen you drink way over your limit already.", "kitchen/dark/start41");
	dialogue_tree.addDialogue("kitchen/dark/start41", "nick", "Fuck you Terri, let us drink in peace", "kitchen/dark/start42");
	dialogue_tree.addDialogue("kitchen/dark/start42", "laura", "If your not drunk, stop acting like an asshole then...", "kitchen/dark/start42");
	dialogue_tree.addDialogue("kitchen/comedy/start43", "nick", "Shut up,\nFor once in your life.\nWill you do me a favor and shut up for a minute?\"", "apartment/dark/start0");



	//If Dark Option - Pick 4 - ()

	dialogue_tree.addDialogue("apartment/dark/start0", "",
	[&]()
	{
		play_01.moveToNextScene();
		return play_01.getNextScene()->getDescription();
	},
	[&]()
	{
		return "apartment/dark/start2";
	});

	dialogue_tree.addDialogue("apartment/dark/start2", "ed", "*You bang on the door loudly with force and open the door*", "apartment/dark/start3");
	dialogue_tree.addDialogue("apartment/dark/start3", "ed", "Terri????", "apartment/dark/start4");
	dialogue_tree.addDialogue("apartment/dark/start4", "terri", "Ed!...\n What are you doing here?", "apartment/dark/start5");
	dialogue_tree.addDialogue("apartment/dark/start5", "ed", "Why? I need you to tell me why.\n I lived for you, I worshipped you...", "apartment/dark/start6");
	dialogue_tree.addDialogue("apartment/dark/start6", "mel", "Listen Ed, I know this is hard but...", "apartment/dark/start7");
	dialogue_tree.addDialogue("apartment/dark/start7", "ed", "Fuck you.\n Shut up\nI'll fucking kill you!", "apartment/dark/start8");
	dialogue_tree.addDialogue("apartment/dark/start8", "ed", "*You punch Mel, he falls to the floor*", "apartment/dark/start9");
	dialogue_tree.addDialogue("apartment/dark/start9", "terri", "Eddie!\nPlease!", "apartment/dark/start10");
	dialogue_tree.addDialogue("apartment/dark/start10", "ed", "*You point the gun at Mel's head*", "apartment/dark/start11");
	dialogue_tree.addDialogue("apartment/dark/start11", "ed", "What's wrong with me.\nWhat have i done wrong.\n Why don't you want me?", "apartment/dark/start12");
	dialogue_tree.addDialogue("apartment/dark/start12", "terri", "Ed.\nEddie.\nPlease..... Give me the gun.\nThen we can talk about this .", "apartment/dark/start13");
	dialogue_tree.addDialogue("apartment/dark/start13", "terri", "*Terri begin to cry*", "apartment/dark/start14");
	dialogue_tree.addDialogue("apartment/dark/start14", "terri", "Just look at me.\n I was drowning.\n I was not capable of...", "apartment/dark/start15");
	dialogue_tree.addDialogue("apartment/dark/start15", "terri", "You deserve to be loved.\n You do i mean it.\nCalm down please, lets just talk.", "apartment/dark/start16");
	dialogue_tree.addDialogue("apartment/dark/start16", "ed", "I just wanted to be what you wanted\nSo why don't you want me.", "apartment/dark/start17");
	dialogue_tree.addDialogue("apartment/dark/start17", "ed", "Now i spend every fucking minute praying to be somebody else.\nSomeone I'm not.\nAll of that for you!", "apartment/dark/start18");
	dialogue_tree.addDialogue("apartment/dark/start18", "mel", "Put down the gun, Ed.\nShe just doesn't love you anymore\nYou need to move on.\"", "apartment/option1");

	//If Dark Option - Pick 5 - ()
	dialogue_tree.addDialogue("apartment/dark/start19", "ed", "Terri tell me it's not true!", "apartment/dark/start20");
	dialogue_tree.addDialogue("apartment/dark/start20", "terri", "I'm sorry Ed.\nI've moved on, I'm with Mel now.", "apartment/dark/start21");
	dialogue_tree.addDialogue("apartment/dark/start21", "ed", "After all I've done.\nThis is what you fucking do to me!!", "apartment/dark/start22");
	dialogue_tree.addDialogue("apartment/dark/start22", "mel", "Please calm down Ed!", "apartment/dark/start23");
	dialogue_tree.addDialogue("apartment/dark/start23", "ed", "Shut up you bastard!\nI'm going to fucking kill you all!!", "apartment/dark/start24");
	dialogue_tree.addDialogue("apartment/dark/start24", "mel", "*No Ed don't!!", "apartment/dark/start25");
	dialogue_tree.addDialogue("apartment/dark/start25", "terri", "*Terri screams in fear*", "apartment/dark/start26");
	dialogue_tree.addDialogue("apartment/dark/start26", "mel", "*Mel shoots Ed point blank!\nThen turn towards TerrI*", "apartment/dark/start27");
	dialogue_tree.addDialogue("apartment/dark/start27", "mel", "Goodbye Terri....\nI'll always love you in my heart", "apartment/dark/start28");
	dialogue_tree.addDialogue("apartment/dark/start28", "mel", "*Mel then shoots Terri 3 times in the stomach and procceds to leave the apartment*", "next night");


	//Player Option 2
	dialogue_tree.addPlayerOption("kitchen/option2", "Drunk teen crashes into an old couple", "kitchen/sad/start44");
	dialogue_tree.addPlayerOption("kitchen/option2", "Drunk teen singing let it go, crashes into old couple", "kitchen/comedy/start9");
	dialogue_tree.addPlayerOption("kitchen/option2", "Learner driver crashes into an old couple", "kitchen/light/start9");
	dialogue_tree.addPlayerOption("kitchen/option2", "Drunk teen rams into old couple", "kitchen/dark/start9");

	//Player Option 3
	dialogue_tree.addPlayerOption("kitchen/option3", "Teen doesn't make it",
		[&]
	{
		auto mover = play_01.getScene()->getCharacter(Play::RIGGAN);
		mover->setFacing(Character::CharacterFacing::WEST);
		mover->slowMoveToPosition(mover->getXPosition() - 200.0f, mover->getYPosition());
		return  "kitchen/sad/start47";
	});
	dialogue_tree.addPlayerOption("kitchen/option3", "Teen gets high off painkillers", "kitchen/comedy/start9");
	dialogue_tree.addPlayerOption("kitchen/option3", "Teen get away with only a few broken bones", "kitchen/light/start9");
	dialogue_tree.addPlayerOption("kitchen/option3", "Teens body was completely mutilated in crash", "kitchen/dark/start9");

	//Player Option 4
	dialogue_tree.addPlayerOption("kitchen/option4", "Old couple is put into full body casts", "kitchen/sad/start50");
	dialogue_tree.addPlayerOption("kitchen/option4", "Old man jokes about the situation", "kitchen/comedy/start9");
	dialogue_tree.addPlayerOption("kitchen/option4", "Couple walk away with just a few bad bruises", "kitchen/light/start9");
	dialogue_tree.addPlayerOption("kitchen/option4", "Couple have no insurance, so are just left for dead", "kitchen/dark/start9");

	//Player Option 5
	dialogue_tree.addPlayerOption("apartment/option1", "Take you own life", "apartment/sad/start19");
	dialogue_tree.addPlayerOption("apartment/option1", "Spray Mel with the water gun", "kitchen/comedy/start9");
	dialogue_tree.addPlayerOption("apartment/option1", "Beg Terri for another chance", "kitchen/light/start9");
	dialogue_tree.addPlayerOption("apartment/option1", "Let your anger take over", "kitchen/dark/start9");

	dialogue_tree.addDialogue("next night", "", [&]()
	{
		play_01.moveToNextNight();
		return " ";

	}, "empty");

	dialogue_tree.addDialogue("empty", "", "", "empty");
}