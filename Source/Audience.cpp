#include "Audience.h"
#include <random>

int Audience::getSad()
{
	return approval_sad;
}

int Audience::getLight()
{
	return approval_light;
}

int Audience::getDark()
{
	return approval_dark;
}

int Audience::getShocking()
{
	return approval_shocking;
}

void Audience::init()
{
	//todo: move this to some kind of RNG class we can use anywhere
	std::random_device rd;
	std::mt19937 mt(rd());

	std::uniform_int_distribution<int> random(1, max_approval);
	approval_sad = random(mt);
	approval_shocking = random(mt);
	approval_light = random(mt);
	approval_dark = random(mt);
}

void Audience::varyApprovalsBetweenNights()
{
}
