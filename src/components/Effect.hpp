#pragma once
#include "Component.hpp"

struct Effect : public Component
{
    virtual Sample Apply(Sample) = 0;
};

Sample operator>>(Sample b, Effect& a);