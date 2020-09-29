#pragma once
#include "../Effect.hpp"
#include "../../utils/Utils.hpp"

class Filter : public Effect
{
protected:
    Sample x[3];
    Sample y[3];

    double cutoff = 1000;

public:
    Filter& Cutoff(double);
};