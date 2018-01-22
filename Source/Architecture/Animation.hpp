#pragma once

//STD
#include <vector>
#include <memory>
#include <chrono>

//LIB
#include <Engine/Sprite.h>

typedef std::chrono::high_resolution_clock Clock;

class Animation
{
public:
	struct FrameData
	{
		float relative_x;
		float relative_y;
		float frame_length_milliseconds;
	};

	Animation(ASGE::Renderer* renderer, bool loop = true);

	Animation(const Animation& other) = delete;
	Animation(Animation&& other) = delete;
	Animation& operator= (const Animation& other) = delete;
	Animation& operator= (Animation&& other) = delete;
	
	void update(double dt);

	void addFrame(std::string texture, float frame_length_milliseconds, float relative_x = 0, float relative_y = 0);
	ASGE::Sprite* getCurrentFrameSprite() const;

	void play();
	void pause();
	bool isOver();

	float xPos = 0;
	float yPos = 0;

private:
	void nextFrame();

	ASGE::Renderer* renderer;

	bool playing = true;
	bool loop;

	std::vector<std::pair<std::unique_ptr<ASGE::Sprite>, FrameData>> frames;
	size_t current_frame = 0;
	double frame_time = 0;
};