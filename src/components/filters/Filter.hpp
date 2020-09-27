#pragma once
#include "../Component.hpp"
#include "../../utils/Utils.hpp"

class Filter : public Component
{
protected:
    Sample x[3];
    Sample y[3];

    double cutoff = 1000;

public:
    Filter& Cutoff(double);
    Sample Apply(Sample a);
    Sample GetSample() override;
};