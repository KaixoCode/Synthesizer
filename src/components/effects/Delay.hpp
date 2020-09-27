#pragma once
#include "../Component.hpp"
#include "../../utils/audio/Audio.hpp"

class Delay : public Component 
{
public:
	static const unsigned int MAX_SIZE = 2 * SAMPLE_RATE;

private:
	unsigned int current = 0;
	unsigned int offset = SAMPLE_RATE;
	Sample now = 0;
	Sample buffer[MAX_SIZE];
	double feedback = 0.1;
	double mix = 0.5;

public:
	Sample NextSample() override;
	Sample GetSample() override;

	Delay& Time(double);
	Delay& Feedback(double);
	Delay& Mix(double);

	Sample Apply(Sample);
	Sample operator>>(Sample a) { return Apply(a); };
};