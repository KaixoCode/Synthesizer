#include "ADSR.hpp"
#include "../../utils/audio/Audio.hpp"
#include <vector>
#include <cmath>



Sample ADSR::NextSample()
{
    if (phase >= 0 && (phase < A + D || !gate)) phase += 1.0/(double)SAMPLE_RATE;
    else if (gate)
        phase = A + D;
    sample = phase < 0 ? 0 : phase < A ? std::pow(phase / A, AC) : phase <= A + D ? 1 - (1-S)*std::pow((phase-A) / D, DC) : phase < A + D + R ? S - S*std::pow((phase-A-D) / R, RC) : 0;
    return sample;
}

Sample ADSR::GetSample()
{
    return sample;
}

void ADSR::Trigger() 
{
    phase = 0;
}

void ADSR::Gate(bool g) 
{
    if (gate && !g) phase = A + D;
    gate = g;
}

Sample ADSR::Apply(Sample s) 
{
    return s * NextSample();
}

ADSR& ADSR::Attack(double v) 
{
    A = v;
    return *this;
}

ADSR& ADSR::Decay(double v)
{
    D = v;
    return *this;
}

ADSR& ADSR::Sustain(double v)
{
    S = v;
    return *this;
}

ADSR& ADSR::Release(double v)
{
    R = v;
    return *this;
}
