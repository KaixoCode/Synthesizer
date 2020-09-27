#include "../Component.hpp"
#include "../../utils/audio/Audio.hpp"

class Delay : public Component 
{
	unsigned int current = 0;
	unsigned int offset = SAMPLE_RATE;
	Sample now = 0;
	Sample buffer[SAMPLE_RATE];
	double feedback = 0.1;

public:
	Sample NextSample() override;
	Sample GetSample() override;

	Delay& Time(double);
	Delay& Feedback(double);

	Sample Apply(Sample);
	Sample operator>>(Sample a) { return Apply(a); };
};