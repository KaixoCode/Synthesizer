#include "Filter.hpp"


class HPF : public Filter
{
public:
    Sample NextSample() override;
};