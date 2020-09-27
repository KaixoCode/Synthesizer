#pragma once
#include "Delay.hpp"
#include "../Component.hpp"
#include "../../utils/audio/Audio.hpp"
#include "../oscillators/Oscillator.hpp"

class Flanger : public Component
{
	Oscillator lfo;
	Delay delay;
	Sample now = 0;
	double mix = 0.5;
	double intensity = 0.5;
	double feedback = 0;

public:

	Flanger();

	Sample NextSample() override;
	Sample GetSample() override;

	Flanger& Mix(double);
	Flanger& Frequency(double);
	Flanger& Feedback(double);
	Flanger& Intensity(double);

	Sample Apply(Sample);

	Sample operator>>(Sample a) { return Apply(a); };
};