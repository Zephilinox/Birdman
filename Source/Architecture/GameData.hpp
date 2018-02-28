#pragma once

#include <Engine/Renderer.h>
#include "Managers/StateManager.hpp"
#include "Rng.h"
#include "Messages/MessageQueue.hpp"
#include "Managers/InputManager.hpp"

namespace ASGE
{
	class Renderer;
}

class StateManager;
class InputManager;
class FontManager;
class MessageQueue;
class AudioManager;
class NetworkManager;
class Rng;

class GameData
{
public:	
	static void				initialize(ASGE::Renderer* render, int width, int height);

	static ASGE::Renderer*	getRenderer()		noexcept;
	static StateManager*	getStates()			noexcept;
	static InputManager*	getInput()			noexcept;
	static FontManager*		getFonts()			noexcept;
	static MessageQueue*	getMessageQueue()	noexcept;
	static AudioManager*	getAudio()			noexcept;
	static NetworkManager*	getNetwork()		noexcept;
	static Rng*				getRNG()			noexcept;
	
	static int				getWindowWidth()	noexcept;
	static int				getWindowHeight()	noexcept;

private:
	static ASGE::Renderer*					renderer;
	static std::unique_ptr<InputManager>	input;
	static std::unique_ptr<StateManager>	states;
	static std::unique_ptr<FontManager>		fonts;
	static std::unique_ptr<MessageQueue>	message_queue;
	static std::unique_ptr<AudioManager>	audio;
	static std::unique_ptr<NetworkManager>	network;
	static std::unique_ptr<Rng>				RNG;

	static int window_width;
	static int window_height;

	static bool initialized;
};
