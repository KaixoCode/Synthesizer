#pragma once

#define PIN_AMOUNT 32
#include <array>

#ifdef WIN32

#include "../gui/guilib.hpp"
class Synth : Window
{
    Knob k1, k2, k3, k4, k5, k6, k7, k8, k9, k10;
    Slider s1, s2, s3, s4, s5, s6, s7, s8, s9, s10;
    Sensor sensor1;
 
    void Draw() {
        Background({ 0, 0, 0 });
        Fill({ 1, 1, 1, 1 });
        DrawString(L"O S C   O N E", 13, 200, 30);
        Window::Draw(s9, 130, 30);
        Window::Draw(k1, 235, 108);
        Window::Draw(k2, 340, 198);
        Window::Draw(k3, 235, 288);
        
        int mo = 300;
        DrawString(L"O S C   T W O", 13, 200 + mo, 30);
        Window::Draw(s10, 130 + mo, 30);
        Window::Draw(k4, 340 + mo, 108);
        Window::Draw(k5, 235 + mo, 198);
        Window::Draw(k6, 340 + mo, 288);


        int dx = 70, x = 30 - dx, y = height - 330;
        
        Window::Draw(sensor1, x += dx, height - 430);
        DrawString(L"E N V E L O P E   1", 19, (x += 30) + dx, y - 38);
        Window::Draw(s1, x += dx, y);
        Window::Draw(s2, x += dx, y);
        Window::Draw(s3, x += dx, y);
        Window::Draw(s4, x += dx, y);

        x += dx;
        DrawString(L"E N V E L O P E   2", 19, x + dx, y - 38);
        Window::Draw(s5, x += dx, y);
        Window::Draw(s6, x += dx, y);
        Window::Draw(s7, x += dx, y);
        Window::Draw(s8, x += dx, y);
    };

public:
    Synth(std::array<double, PIN_AMOUNT> &params) :
        k1(params[0], 0), 
        k2(params[1], 0),
        k3(params[2], 0),
        k4(params[3], 0),
        k5(params[4], 0),
        k6(params[5], 0),
        k7(params[6], 0),
        k8(params[7], 0),
        k9(params[7], 0),
        k10(params[7], 0),
        s1(params[16], 0.0),
        s2(params[17], 0.6),
        s3(params[18], 1.0),
        s4(params[19], 0.4),
        s5(params[20], 0.0),
        s6(params[21], 0.6),
        s7(params[22], 1.0),
        s8(params[23], 0.4),
        s9(params[24], 0.4),
        s10(params[25], 0.5),
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