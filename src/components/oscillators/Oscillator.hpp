#pragma once
#include "../../utils/audio/Audio.hpp"
#include "../Component.hpp"
#include "Wavetables.hpp"

class Oscillator : public Component
{
private:
    double phase = 0;
    Sample sample = 0;

    double fm = 0;
    double am = 1;

public:
    double frequency = 60; // Hz

    Wavetables::WaveTable* waveTable = new Wavetables::Sine;

    Oscillator() {};

    ~Oscillator() { delete waveTable; };

    Sample NextSample() override;
    Sample GetSample() override;

    void ResetPhase();

    void FM(Sample, double); // Frequency modulate
    void AM(Sample); // Amplitude modulate
};

