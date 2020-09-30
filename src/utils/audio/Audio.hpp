#pragma once
#define SAMPLE_RATE (44100 * 8)
#define BUFFER_SIZE 512
#define CHANNELS 2

typedef double Sample;

struct Stereo
{
    Sample left;
    Sample right;
};

void FillBuffer(float* buffer, Stereo(*chain)(void));

void FillBuffer(float* buffer, Sample(*chain)(void));

namespace Audio {

    int Start();
    void Clean();
    void SetCallback(void func(float*));
}