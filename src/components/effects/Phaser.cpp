#include "Phaser.hpp"
#include <algorithm>

Sample Phaser::Apply(Sample a)
{
	now = a - intensity * (delay
		.Mix(1)
		.Time(d)
		.Feedback(feedback)
		>> a);
	return NextSample();
};

Sample Phaser::NextSample()
{
	return now;
};

Sample Phaser::GetSample()
{
	return now;
};


Phaser& Phaser::Delay(double a)
{
	d = a;
	return *this;
}

Phaser& Phaser::Mix(double a)
{
	mix = a;
	return *this;
}

Phaser& Phaser::Feedback(double a)
{
	feedback = a;
	return *this;
}

Phaser& Phaser::Intensity(double a)
{
	intensity = a;
	return *this;
}

