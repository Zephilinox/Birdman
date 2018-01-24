#include "Animation.hpp"

//LIB
#include <Engine/Renderer.h>

Animation::Animation(ASGE::Renderer* renderer, bool loop)
	: renderer(renderer)
	, loop(loop)
{}

void Animation::update(double dt_milli)
{
	frames[current_frame].first->xPos(xPos + frames[current_frame].second.relative_x);
	frames[current_frame].first->yPos(yPos + frames[current_frame].second.relative_y);

	if (playing && frames.size())
	{
		frame_time += dt_milli;
		if (frame_time >= frames[current_frame].second.frame_length_milliseconds)
		{
			nextFrame();
		}
	}
}

void Animation::addFrame(std::string texture, float frame_length_milliseconds,  float relative_x, float relative_y)
{
	FrameData frame_data;
	frame_data.frame_length_milliseconds = frame_length_milliseconds;
	frame_data.relative_x = relative_x;
	frame_data.relative_y = relative_y;

	std::unique_ptr<ASGE::Sprite> spr(renderer->createRawSprite());
	frames.push_back(std::make_pair(std::move(spr), frame_data));
	if (!frames.back().first->loadTexture(std::string("..\\..\\Resources\\Textures\\" + texture + ".png").c_str()))
	{
		throw std::exception(std::string("[Animation::addFrame()] Failed to load texture '..\\..\\Resources\\Textures\\'" + texture + ".png").c_str());
	}
}

ASGE::Sprite* Animation::getCurrentFrameSprite() const
{
	return frames[current_frame].first.get();
}

void Animation::play()
{
	playing = true;
}

void Animation::pause()
{
	playing = false;
}

bool Animation::isOver()
{
	return current_frame == frames.size() - 1 && !loop;
}

void Animation::nextFrame()
{
	if (current_frame < frames.size() - 1)
	{
		current_frame++;
	}
	else if (loop)
	{
		current_frame = 0;
	}
	else
	{
		return;
	}

	frames[current_frame].first->xPos(xPos + frames[current_frame].second.relative_x);
	frames[current_frame].first->yPos(yPos + frames[current_frame].second.relative_y);

	frame_time = 0;
}
