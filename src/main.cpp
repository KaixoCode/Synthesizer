#include "utils/Utils.hpp"
#include "utils/audio/Audio.hpp"
#include "utils/midi/Midi.hpp"
#include "components/envelopes/ADSR.hpp"
#include "components/filters/Filter.hpp"
#include "components/oscillators/Oscillator.hpp"

void AudioCallback(float *buffer);

Oscillator osc;
ADSR env{ 0, 0.6, 0, 0 };
ADSR filt{ 0, 0.3, 0, 0 };
Oscillator fmosc;
Filter filter;

double params[10];
bool pressed = false;

#ifdef WIN32
#include "utils/gui/guilib.hpp"
class Synth : Window 
{
    Knob knob1;
    Knob knob2;
    Sensor sensor1;

    bool trig = false;

    void Draw() {
        if (pressed)
            Background({ 0.3, 0, 0 });
        else
            Background({ 0, 0, 0 });
        Window::Draw(knob1, 500, 500);
        Window::Draw(knob2, 400, 200);
        Window::Draw(sensor1, 600, 300);

        params[0] = knob1.val;
        params[1] = knob2.val;
        if (sensor1.val != 1) 
        {
            osc.frequency = Midi::NoteToFreq(Midi::NoteToScale(sensor1.val * 36 + 24, new int[7]{ 0, 2, 3, 5, 7, 8, 10 }, 7));
            if (!trig) {
                env.Trigger();
                osc.ResetPhase();
                fmosc.ResetPhase();
                filt.Trigger();
                env.Gate(true);
                trig = true;
            }
        }
        else
        {
            if (trig) env.Gate(false);
            trig = false;
        }
        params[2] = sensor1.val;

    };
};
#endif

int a = 0;
void MidiPress(int note, int velocity) {
    a++;
    osc.frequency = Midi::NoteToFreq(note);
    osc.ResetPhase();
    fmosc.ResetPhase();
    env.Trigger();
    filt.Trigger();
    env.Gate(true);
}

void MidiRelease(int note, int velocity) {
    a--;
    if (a == 0)env.Gate(false);
}

int main(void) {
#ifdef WIN32
    Synth gui;
#endif



    Midi midi;
    midi.MidiPress = MidiPress;
    midi.MidiRelease = MidiRelease;
    

    osc.waveTable = new Wavetables::Saw;
    fmosc.waveTable = new Wavetables::Sine;

    Audio::Start();    
    Audio::SetCallback(AudioCallback);
   
    while (true)
    {

    }

    Audio::Clean();
    return 0;
}

void AudioCallback(float *buffer)
{
    for (int i = 0; i < Audio::BUFFER_SIZE;) {
        Sample mix = 0;

        fmosc.frequency = osc.frequency * params[1] * 8.0;
        osc.FM(fmosc.NextSample(), params[0] * 0.1);
        filter.cutoff = filt.NextSample() * 10000 + 1000;
        Sample o1 = env.Apply(filter.Apply(osc.NextSample()));
        
        

        buffer[i++] = o1 * 0.5;
    }
}