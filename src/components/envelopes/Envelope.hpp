#include "../Component.hpp"

struct Envelope : public Component
{
    virtual void Trigger() = 0;
    virtual void Gate(bool) = 0;
    virtual Sample Apply(Sample) = 0;
};
