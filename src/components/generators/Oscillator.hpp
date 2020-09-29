#pragma once
#include "../../utils/audio/Audio.hpp"
#include "../Generator.hpp"
#include "Wavetables.hpp"

class Oscillator : public Generator
{
private:
    double phase = 0;
    Sample sample = 0;

    double fm = 0;
    double am = 1;

    double frequency = 60; // Hz
public:

    Oscillator& Frequency(double f);
    double Frequency();

    Wavetables::WaveTable* waveTable = new Wavetables::Sine;

    Oscillator() {};

    ~Oscillator() { delete waveTable; };

    Sample NextSample() override;
    Sample GetSample() override;

    void ResetPhase();

    Oscillator& FM(Sample); // Frequency modulate
    Oscillator& AM(Sample); // Amplitude modulate
};

