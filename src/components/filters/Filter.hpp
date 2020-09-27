#include "../Component.hpp"
#include "../../utils/Utils.hpp"

class Filter : public Component
{
    Sample x[6];
    Sample y[3];
public:

    Sample Apply(Sample a);
    Sample NextSample() override;
    Sample GetSample() override;
};