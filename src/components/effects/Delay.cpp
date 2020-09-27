#include "Delay.hpp"
#include <algorithm>

Sample Delay::Apply(Sample a) 
{
	buffer[current] = a;
	return NextSample() * mix + a * (1 - mix);
};

Sample Delay::NextSample() 
{
	unsigned int index = (current + MAX_SIZE - offset) % MAX_SIZE;
	now = buffer[index];
	buffer[current] += now * feedback;
	current = (current + 1) % MAX_SIZE;
	return now;
};

Sample Delay::GetSample() 
{
	return now;
};

Delay& Delay::Time(double a)
{
	offset = std::max(std::min(a * SAMPLE_RATE, (double) Delay::MAX_SIZE), 1.0);
	return *this;
}

Delay& Delay::Feedback(double a)
{
	feedback = a;
	return *this;
}

Delay& Delay::Mix(double a)
{
	mix = a;
	return *this;
}