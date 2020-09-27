#include "Wavetables.hpp"
#include "../../utils/Utils.hpp"
#include <cmath>

double Wavetables::Square::value(double phase)
{
    return phase > PI ? -1 : 1;
}

double Wavetables::Sine::value(double phase)
{
    return std::sin(phase);
}

double Wavetables::Saw::value(double phase)
{
    return 2 * ((-phase / TWO_PI) + 0.5);
}