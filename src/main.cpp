#include "utils/Utils.hpp"
#include "utils/audio/Audio.hpp"
#include "utils/midi/Midi.hpp"
#include "components/envelopes/ADSR.hpp"
#include "components/filters/LPF.hpp"
#include "components/filters/HPF.hpp"
#include "components/oscillators/Oscillator.hpp"
#include "components/effects/Delay.hpp"
#include "components/effects/Flanger.hpp"
#include "components/effects/Chorus.hpp"
#include "components/effects/Phaser.hpp"
#include "components/effects/Reverb.hpp"



void AudioCallback(float *buffer);

Oscillator lfo;
Oscillator osc;
ADSR env{ 0, 0.6, 1, 0.3 };
ADSR env2{ 0, 0.3, 0.2, 0.3 };
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



double params[10];
bool pressed = false;

#ifdef WIN32
#include "utils/gui/guilib.hpp"
class Synth : Window 
{
    Knob knob1;
    Knob knob2;
    Knob knob3;
    Sensor sensor1;


    Sensor sensor2;

    bool trig = false, trig2 = false;

    void Draw() {
        if (pressed)
            Background({ 0.3, 0, 0 });
        else
            Background({ 0, 0, 0 });
        Window::Draw(knob1, 500, 500);
        Window::Draw(knob2, 400, 200);
        Window::Draw(knob3, 800, 300);

        Window::Draw(sensor1, 600, 300);


        Window::Draw(sensor2, 1000, 300);

        params[0] = knob1.val;
        params[1] = knob2.val;
        params[2] = knob3.val;

        if (sensor1.val != 1) 
        {
            osc.Frequency(Midi::NoteToFreq(Midi::NoteToScale(sensor1.val * 36 + 24, new int[7]{ 0, 2, 3, 5, 7, 8, 10 }, 7)));
            if (!trig) {
                osc.ResetPhase();
                fmosc.ResetPhase();
                env.Trigger();
                env.Gate(true);
                env2.Trigger();
                env2.Gate(true);
                trig = true;
            }
        }
        else
        {
            if (trig) env.Gate(false);
            trig = false;
        }

        if (sensor2.val != 1) {
            if (!trig2) {
                kenv1.Trigger();
                kenv2.Trigger();
                kick.ResetPhase();
                trig2 = true;
            }
        }
        else
        {
            trig2 = false;
        }

    };
};
#endif

int a = 0;
void MidiPress(int note, int velocity) {
    a++;
    osc.Frequency(Midi::NoteToFreq(note));
    osc.ResetPhase();
    fmosc.ResetPhase();
    env.Trigger();
    env.Gate(true);
    env2.Trigger();
    env2.Gate(true);
}

void MidiRelease(int note, int velocity) {
    a--;
    if (a == 0)env.Gate(false);
    if (a == 0)env2.Gate(false);
}

int main(void) {
#ifdef WIN32
    Synth gui;
#endif



    Midi midi;
    midi.MidiPress = MidiPress;
    midi.MidiRelease = MidiRelease;
    
    reverb1.Offset(0.0032);
    reverb2.Offset(0.0038);

    osc.waveTable = new Wavetables::Saw;
    lfo.waveTable = new Wavetables::Triangle;
    fmosc.waveTable = new Wavetables::Sine;

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

void AudioCallback(float* buffer)
{
    for (int i = 0; i < CHANNELS * BUFFER_SIZE;)
    {
        // Mono pre-effect channel
        Sample pre = flanger.Feedback(0.3).Mix(0.5).Intensity(2).Frequency(0.25) >> ( // Flanger effect
            lpf2.Cutoff(20000) >> ( // Filter everything above 20kHz away
                env >> ( // Apply the envelope
                    lpf.Cutoff(++env2 * 20000 + 1000) >> (// Lowpass filter whose cutoff is modulated by env2.
                        ++osc.FM(++fmosc.Frequency(osc.Frequency() * params[1] * 8.0), params[0] * 0.1))))); // Osc is FMed by fmosc

        // Left stereo channel
        Sample post1 = 0.5 * (
            delay1.Mix(params[2]).Time(0.300).Feedback(0.4) >> ( // Delay effect
                chorus1.Intensity(0.5).Frequency(0.20).Feedback(0.1) >> pre)); // Chorus effect
           
        // Right stereo channel
        Sample post2 = 0.5 * (
            delay2.Mix(params[2]).Time(0.320).Feedback(0.4) >> ( // Delay effect
                chorus2.Intensity(0.5).Frequency(0.30).Feedback(0.1) >> pre)); // Chorus effect
             
        // Limit value between -1 and 1 to prevent clipping.
        buffer[i++] = post1 > 1 ? 1 : post1 < -1 ? -1 : post1;
        buffer[i++] = post2 > 1 ? 1 : post2 < -1 ? -1 : post2;
    }
}