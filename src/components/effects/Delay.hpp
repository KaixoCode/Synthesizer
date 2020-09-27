#include "../Component.hpp"
#include "../../utils/audio/Audio.hpp"

class Delay : public Component 
{
	Sample buffer[SAMPLE_RATE];
};