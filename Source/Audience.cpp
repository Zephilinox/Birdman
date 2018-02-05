#include "Audience.h"
#include <random>

float Audience::getSad()
{
	return approval_sad;
}

float Audience::getLight()
{
	return approval_light;
}

float Audience::getDark()
{
	return approval_dark;
}

float Audience::getShocking()
{
	return approval_shocking;
}

void Audience::init()
{
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
