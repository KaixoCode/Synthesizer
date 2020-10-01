#include "Oscillator.hpp"
#define _USE_MATH_DEFINES
#include <cmath>

Sample Oscillator::NextSample()
{
    double f = frequency;
    if (detune >= 0) f *= detune + 1;
    else f *= 1/(-detune + 1);
    double delta = (sync * (f) + fm) * (2 * M_PI) / (double)SAMPLE_RATE;
    syncCounter += delta / (2 * M_PI);
    phase = std::fmod((2 * M_PI) + phase + delta, (2 * M_PI));
    if (syncCounter > sync) {
        syncCounter = 0;
        phase = 0;
    }
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

Oscillator& Oscillator::FM(Sample fm)
{
    this->fm = fm;
    return *this;
}

Oscillator& Oscillator::Detune(double f)
{
    this->detune = f;
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

double Oscillator::Phase() 
{
    return phase;
}

Oscillator& Oscillator::Sync(double o) 
{
    sync = o;
    return *this;
}