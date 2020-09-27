
#include "Chorus.hpp"

Chorus::Chorus()
{
	lfo.Frequency(0.25);
}

Sample Chorus::Apply(Sample a)
{
	now = delay
		.Mix(mix)
		.Time(++lfo * (intensity * 0.02) + (intensity * 0.08))
		.Feedback(feedback)
		>> a;
	return NextSample();
};

Sample Chorus::NextSample()
{
	return now;
};

Sample Chorus::GetSample()
{
	return now;
};


Chorus& Chorus::Frequency(double a)
{
	lfo.Frequency(a);
	return *this;
}

Chorus& Chorus::Mix(double a)
{
	mix = a;
	return *this;
}

Chorus& Chorus::Feedback(double a)
{
	feedback = a;
	return *this;
}

Chorus& Chorus::Intensity(double a)
{
	intensity = a;
	return *this;
}

