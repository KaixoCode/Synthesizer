#include "Oscillator.hpp"
#include "../../utils/Utils.hpp"
#include <cmath>

Sample Oscillator::NextSample()
{
    phase = std::fmod(phase + fm + (frequency) * TWO_PI / Audio::SAMPLE_RATE, TWO_PI);
    sample = waveTable->value(phase) * am;
    fm = 0;
    am = 1;
    return sample;
}

Sample Oscillator::GetSample()
{
    return sample;
}

void Oscillator::ResetPhase() 
{
    phase = 0;
}

void Oscillator::FM(Sample fm, double amt) 
{
    this->fm = fm * amt;
}

void Oscillator::AM(Sample am)
{
    this->am = am;
}