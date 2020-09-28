#include "Reverb.hpp"
#include <algorithm>

Sample Reverb::Apply(Sample a)
{
	buffer[current] = a;
	return NextSample() * mix + a * (1 - mix);
};

Sample Reverb::NextSample()
{
	now = 0;
	double mix = 0.5;
	for (int i = 0; i < sizeof(offsets)/sizeof(unsigned int); i++) 
	{
		unsigned int index = (current + MAX_SIZE - offsets[i]) % MAX_SIZE;
		now += mix * buffer[index];
		mix *= 0.95;
	}

	buffer[current] += (1.0/32.0) * now;
	current = (current + 1) % MAX_SIZE;
	return now;
};

Sample Reverb::GetSample()
{
	return now;
};

Reverb& Reverb::Mix(double a)
{
	mix = a;
	return *this;
}

Reverb& Reverb::Offset(double a)
{
	dif = a;
	int f = 0;
	for (int i = 0; i < sizeof(offsets) / sizeof(unsigned int); i++)
	{
		offsets[i] = (unsigned int)(f++ * (dif *= 1.04) * SAMPLE_RATE);
	}

	return *this;
}
