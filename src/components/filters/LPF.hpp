#include "Filter.hpp"


class LPF : public Filter
{
public:
    Sample NextSample() override;
};