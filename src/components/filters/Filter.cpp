#include "Filter.hpp"

Sample Filter::Apply(Sample a)
{
	x[0] = a;
	return NextSample();
}

Sample Filter::GetSample()
{
	return y[0];
}

Filter& Filter::Cutoff(double c) 
{
	cutoff = c;
	return *this;
}