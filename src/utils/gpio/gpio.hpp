#pragma once

#define PIN_AMOUNT 32
#include <array>

#ifdef WIN32

#include "../gui/guilib.hpp"
class Synth : Window
{
    Knob knob1;
    Knob knob2;
    Knob knob3;
    Slider s1, s2, s3, s4, s5, s6, s7, s8;
    Sensor sensor1;
 
    void Draw() {
        Background({ 0, 0, 0 });
        Window::Draw(knob1, 500, 500);
        Window::Draw(knob2, 400, 200);
        Window::Draw(knob3, 800, 300);
        int dx = 60, x = 20 - dx, y = height - 320;
        
        Fill({1, 1, 1, 1});
        DrawString(L"Envelope 1", 10, x + dx, y - 38);
        Window::Draw(s1, x += dx, y);
        Window::Draw(s2, x += dx, y);
        Window::Draw(s3, x += dx, y);
        Window::Draw(s4, x += dx, y);

        x += dx;
        DrawString(L"Envelope 2", 10, x + dx, y - 38);
        Window::Draw(s5, x += dx, y);
        Window::Draw(s6, x += dx, y);
        Window::Draw(s7, x += dx, y);
        Window::Draw(s8, x += dx, y);
        Window::Draw(sensor1, 600, 300);
    };

public:
    Synth(std::array<double, PIN_AMOUNT> &params) :
        knob1(params[0], 0), 
        knob2(params[1], 0),
        knob3(params[2], 0),
        s1(params[16], 0.0),
        s2(params[17], 0.6),
        s3(params[18], 1.0),
        s4(params[19], 0.4),
        s5(params[20], 0.0),
        s6(params[21], 0.6),
        s7(params[22], 1.0),
        s8(params[23], 0.4),
        sensor1(params[31])
    {
    }
};
#endif

class GPIO
{

private:
    std::array<double, PIN_AMOUNT> params;

#ifdef WIN32
    Synth gui{ params };
#endif

public:
    double Param(int a) const { return params[a]; };
    double operator[](int a) const { return Param(a); };
};