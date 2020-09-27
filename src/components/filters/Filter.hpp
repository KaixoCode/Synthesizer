#include "../Component.hpp"
#include "../../utils/Utils.hpp"

class Filter : public Component
{

    Sample x[3];
    Sample y[3];
public:
    double cutoff = 1000;

    Sample Apply(Sample a);
    Sample NextSample() override;
    Sample GetSample() override;
};