#include "Delay.hpp"


Sample Delay::Apply(Sample a) 
{
	unsigned int o = current;
	buffer[o] += a;
	return NextSample();
};

Sample Delay::NextSample() 
{
	now = buffer[current];
	buffer[current] *= feedback;
	current = (current + 1) % offset;
	return now;
};

Sample Delay::GetSample() 
{
	return now;
};

Delay& Delay::Time(double a)
{
	offset = a * (sizeof(buffer) / sizeof(Sample));
	return *this;
}

Delay& Delay::Feedback(double a)
{
	feedback = a;
	return *this;
}