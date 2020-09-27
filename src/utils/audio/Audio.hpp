#pragma once

namespace Audio {

    int Start();
    void Clean();
    void SetCallback(void func(float*));
}