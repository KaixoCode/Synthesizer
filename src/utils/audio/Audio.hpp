#pragma once
#define OVERSAMPLING 1
#define SAMPLE_RATE (48000 * OVERSAMPLING)
#define BUFFER_SIZE 512
#define CHANNELS 2

#include <functional>
#include <array>
typedef double Sample;

typedef std::array<float, CHANNELS* BUFFER_SIZE> Buffer;

struct Stereo
{
    Sample left;
    Sample right;
};

typedef const Stereo (*Channel)(void);
typedef Sample (*MonoChannel)(void) ;
typedef void (*BufferCallback)(Buffer&);


void FillBuffer(Buffer& buffer, Channel& chain);

void FillBuffer(Buffer& buffer, MonoChannel& chain);

namespace Audio {

    int Start();
    void Clean();
    void SetCallback(BufferCallback);
}