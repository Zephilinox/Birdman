#pragma once

class Audience
{
public:

	Audience() = default;
	~Audience() = default;

	float getSad();
	float getLight();
	float getDark();
	float getShocking();

	void init();
	void varyApprovalsBetweenNights();

private:
	float approval_sad = 0;
	float approval_light = 0;
	float approval_dark = 0;
	float approval_shocking = 0;

	const int max_approval = 100.0f;
};