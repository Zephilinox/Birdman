#include "Audience.hpp"

//STD
#include <random>

Audience::Audience()
{
	//TODO: move this to some kind of RNG class we can use anywhere
	std::random_device rd;
	std::mt19937 mt(rd());

	std::uniform_int_distribution<int> random(1, max_approval);
	approval_sad = random(mt);
	approval_shocking = random(mt);
	approval_light = random(mt);
	approval_dark = random(mt);
}

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

void Audience::varyApprovalsBetweenNights()
{
	float ten_percent_sad = approval_sad / 10.0f;
	float ten_percent_shocking = approval_shocking / 10.0f;
	float ten_percent_light = approval_light/10.0f;
	float ten_percent_dark = approval_dark / 10.0f;

	//TODO: move this to some kind of RNG class we can use anywhere
	std::random_device rd;
	std::mt19937 mt(rd());

	std::uniform_int_distribution<int> sad_variance(-ten_percent_sad, ten_percent_sad);
	std::uniform_int_distribution<int> shocking_variance(-ten_percent_shocking, ten_percent_shocking);
	std::uniform_int_distribution<int> light_variance(-ten_percent_light, ten_percent_light);
	std::uniform_int_distribution<int> dark_variance(-ten_percent_dark, ten_percent_dark);

	approval_sad += (int)sad_variance(mt);
	approval_shocking += (int)shocking_variance(mt);
	approval_light += (int)light_variance(mt);
	approval_dark += (int)dark_variance(mt);

}
