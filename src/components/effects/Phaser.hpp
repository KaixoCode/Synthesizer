#pragma once
#include "Delay.hpp"
#include "../Component.hpp"
#include "../../utils/audio/Audio.hpp"
#include "../oscillators/Oscillator.hpp"

class Phaser : public Component
{
	Delay delay;
	Sample now = 0;
	double mix = 0.5;
	double intensity = 0.5;
	double feedback = 0;
	double d = 0.1;

public:
	Sample NextSample() override;
	Sample GetSample() override;

	Phaser& Mix(double);
	Phaser& Delay(double);
	Phaser& Feedback(double);
	Phaser& Intensity(double);

	Sample Apply(Sample);

	Sample operator>>(Sample a) { return Apply(a); };
};