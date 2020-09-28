#pragma once
#include "../Component.hpp"
#include "../../utils/audio/Audio.hpp"
#include "../oscillators/Oscillator.hpp"

class Reverb : public Component
{
public:
	static const unsigned int MAX_SIZE = 20 * SAMPLE_RATE;
	double dif = 0.005;

private:
	unsigned int current = 0;
	unsigned int offsets[32];
	
	Sample now = 0;
	Sample buffer[MAX_SIZE];
	double mix = 0.3;

public:
	Sample NextSample() override;
	Sample GetSample() override;

	Sample Apply(Sample);

	Reverb& Mix(double);
	Reverb& Offset(double);

	Sample operator>>(Sample a) { return Apply(a); };
};