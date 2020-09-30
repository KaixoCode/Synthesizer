#define _USE_MATH_DEFINES
#include "Wavetables.hpp"
#include <cmath>

double Wavetables::Square::value(double phase)
{
    return phase > M_PI ? -1 : 1;
}

double Wavetables::Sine::value(double phase)
{
    return std::sin(phase);
}

double Wavetables::Saw::value(double phase)
{
    return 2 * ((-phase / (2 * M_PI)) + 0.5);
}

double Wavetables::Triangle::value(double phase)
{
    return 4 * std::abs(0.5 + (-phase / (2 * M_PI))) - 1;
}