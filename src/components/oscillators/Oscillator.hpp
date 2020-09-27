#pragma once
#include "../../utils/audio/Audio.hpp"
#include "../Component.hpp"
#include "Wavetables.hpp"

class Oscillator : public Component
{
private:
    double phase = 0;
    double sample = 0;

    double fm = 0;
    double am = 1;

public:
    double frequency = 60; // Hz

    Wavetables::WaveTable* waveTable = new Wavetables::Sine;

    Oscillator() {};

    ~Oscillator() { delete waveTable; };

    double NextSample() override;
    double Sample() override;

    void ResetPhase();

    void FM(Oscillator&, double); // Frequency modulate
    void AM(Oscillator&); // Amplitude modulate
};

