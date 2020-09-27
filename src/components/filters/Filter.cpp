#include "Filter.hpp"


Sample Filter::Apply(Sample a)
{
	x[0] = a;
	return NextSample();
}

Sample Filter::NextSample()
{	
	y[0] = y[2] * 0.19582 + y[1] * 0.36953 + 0.20657 * x[0] + 0.41314 * x[1] + 0.20657 * x[2];

	for (int i = 0; i < sizeof(y) / sizeof(double) - 1; i++)
		y[i + 1] = y[i];

	for (int i = 0; i < sizeof(x) / sizeof(double) - 1; i++)
		x[i + 1] = x[i];

	return y[0];
}

Sample Filter::GetSample() 
{
	return y[0];
}