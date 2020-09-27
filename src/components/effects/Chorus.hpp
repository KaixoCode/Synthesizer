#pragma once
#include "Delay.hpp"
#include "../Component.hpp"
#include "../../utils/audio/Audio.hpp"
#include "../oscillators/Oscillator.hpp"

class Chorus : public Component
{
	Oscillator lfo;
	Delay delay;
	Sample now = 0;
	double mix = 0.5;
	double intensity = 1;
	double feedback = 0;

public:

	Chorus();

	Sample NextSample() override;
	Sample GetSample() override;

	Chorus& Mix(double);
	Chorus& Frequency(double);
	Chorus& Feedback(double);
	Chorus& Intensity(double);

	Sample Apply(Sample);

	Sample operator>>(Sample a) { return Apply(a); };
};