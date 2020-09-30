#include "utils/gpio/gpio.hpp"
#include "utils/audio/Audio.hpp"
#include "utils/midi/Midi.hpp"
#include "components/envelopes/ADSR.hpp"
#include "components/filters/LPF.hpp"
#include "components/filters/HPF.hpp"
#include "components/generators/Oscillator.hpp"
#include "components/effects/Delay.hpp"
#include "components/effects/Flanger.hpp"
#include "components/effects/Chorus.hpp"
#include "components/effects/Phaser.hpp"
#include "components/effects/Reverb.hpp"


GPIO gpio;


void AudioCallback(float *buffer);

Oscillator lfo;
Oscillator osc;
ADSR env{ 0, 0.6, 1, 0.3 };
ADSR env2{ 0, 0.3, 0.2, 0.3 };
ADSR env3{ 0, 1.8, 0, 0 };

Oscillator fmosc;
LPF lpf;
LPF lpf2;
Delay delay1;
Delay delay2;
Flanger flanger;
Chorus chorus1;
Chorus chorus2;
Phaser phaser1;
Phaser phaser2;
Reverb reverb1;
Reverb reverb2;


Oscillator kick;
ADSR kenv1{ 0, 0.09, 0, 0 };
ADSR kenv2{ 0, 0.2, 0, 0 };

int a = 0;
void MidiPress(int note, int velocity) 
{
    a++;
    osc.Frequency(Midi::NoteToFreq(note));
    osc.ResetPhase();
    fmosc.ResetPhase();
    env.Trigger();
    env.Gate(true);
    env2.Trigger();
    env3.Trigger();
    env2.Gate(true);
}

void MidiRelease(int note, int velocity) 
{
    a--;
    if (a == 0)env.Gate(false);
    if (a == 0)env2.Gate(false);
}


int main(void) 
{
    Midi midi;
    midi.MidiPress = MidiPress;
    midi.MidiRelease = MidiRelease;
    
    reverb1.Offset(0.0032);
    reverb2.Offset(0.0038);

    osc.waveTable = new Wavetables::Saw;
    lfo.waveTable = new Wavetables::Triangle;
    fmosc.waveTable = new Wavetables::Triangle;
    
    kick.waveTable = new Wavetables::Sine;
    kick.Frequency(60);

    Audio::Start();    
    Audio::SetCallback(AudioCallback);
   
    while (true)
    {

    }

    Audio::Clean();
    return 0;
}

bool trig = false;
void AudioCallback(float* buffer)
{
    if (gpio[31] != 1)
    {
        int note = Midi::NoteToScale(gpio[31] * 64 + 24, new int[7]{ 0, 2, 3, 5, 7, 8, 10 }, 7);
        if (!trig) MidiPress(note, 1);
        osc.Frequency(Midi::NoteToFreq(note ));
        trig = true;
    }
    else
    {
        if (trig) MidiRelease(0, 1);
        trig = false;
    }

    Channel master = [](){
        Stereo mix = 0.5*
            osc                         // Oscillator
            .Sync(env3 * gpio[3] * 10 + 1)
            .FM(gpio[0] * 20000.0 *     // FMed by fmosc
                fmosc                   // Frequency of fmosc dependent on osc.
                .Frequency(gpio[1] * 8 * osc.Frequency()).GetSample())
            .AM(gpio[24])
            >> lpf                      // Running through a lowpass filter
            .Cutoff(20000 *             // Cutoff dependend on env2
                env2
                .Attack(std::pow(gpio[20], 2) * 2)       // Params modulated by gpio values
                .Decay(std::pow(gpio[21], 2) * 2)
                .Sustain(gpio[22])
                .Release(std::pow(gpio[23], 2) * 2))
            >> env                      // Running through an envelope
            .Attack(std::pow(gpio[16], 2) * 2)           // Params modulated by gpio values
            .Decay(std::pow(gpio[17], 4) * 2)
            .Sustain(gpio[18])
            .Release(std::pow(gpio[19], 2) * 2)
            >> lpf2.Cutoff(20000)       // Running through a lowpass filter
            >> StereoEffect{            // Running through stereo chorus
                chorus1
                .Intensity(0.5)
                .Frequency(0.20)
                .Feedback(0.1),
                chorus2
                .Intensity(0.5)
                .Frequency(0.30)
                .Feedback(0.1) }
            >> StereoEffect{            // Running through stereo delay
                delay1
               .Mix(gpio[2])            // Mix modulated by gpio value
               .Time(0.300)
               .Feedback(0.4),
               delay2
               .Mix(gpio[2])            // Mix modulated by gpio value
               .Time(0.320)
               .Feedback(0.4) };

        return mix;
    };

    FillBuffer(buffer, master);
}





/*

            
        */