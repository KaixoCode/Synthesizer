#include "Oscillator.hpp"
#include "../../utils/Utils.hpp"
#include <cmath>

Sample Oscillator::NextSample()
{
    double delta = (sync * frequency + fm) * TWO_PI / (double)SAMPLE_RATE;
    syncCounter += delta / TWO_PI;
    phase = std::fmod(TWO_PI + phase + delta, TWO_PI);
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