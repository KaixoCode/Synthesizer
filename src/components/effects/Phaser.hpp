#pragma once
#include "Delay.hpp"
#include "../../utils/audio/Audio.hpp"

class Phaser : public Effect
{
	Delay delay;
	double mix = 0.5;
	double intensity = 0.5;
	double feedback = 0;
	double d = 0.1;

public:
	Phaser& Mix(double);
	Phaser& Delay(double);
	Phaser& Feedback(double);
	Phaser& Intensity(double);

	Sample Apply(Sample) override;
};