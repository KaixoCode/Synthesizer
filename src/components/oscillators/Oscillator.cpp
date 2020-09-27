#include "Oscillator.hpp"
#include "../../utils/Constants.hpp"
#include <cmath>

double Oscillator::NextSample() 
{
    phase = std::fmod(phase + fm + (frequency) * TWO_PI / Audio::SAMPLE_RATE, TWO_PI);
    fm = 0;
    sample = waveTable->value(phase);
    return sample;
}

double Oscillator::Sample()
{
    return sample;
}

void Oscillator::ResetPhase() 
{
    phase = 0;
}

void Oscillator::FM(Oscillator& fm, double amt) 
{
    this->fm = fm.Sample() * amt;
}

void Oscillator::AM(Oscillator& am)
{
    this->am = am.Sample();
}