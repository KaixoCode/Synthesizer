#pragma once

namespace Audio {

    int Start();
    void Clean();
    void SetCallback(void func(float*));

    extern const unsigned int SAMPLE_RATE;
    extern const unsigned int BUFFER_SIZE;
}