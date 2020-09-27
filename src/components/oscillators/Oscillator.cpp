#include "Oscillator.hpp"
#include "../../utils/Utils.hpp"
#include <cmath>

Sample Oscillator::NextSample()
{
    phase = std::fmod(TWO_PI + phase + fm + (frequency) * TWO_PI / (double) SAMPLE_RATE, TWO_PI);
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

Oscillator& Oscillator::FM(Sample fm, double amt)
{
    this->fm = fm * amt;
    return *this;
}

Oscillator& Oscillator::AM(Sample am)
{
    this->am = am;
    return *this;
}

Oscillator& Oscillator::Frequency(double f) 
{
    frequency = f;
    return *this;
}

double Oscillator::Frequency()
{
    return frequency;
}