#include "Effect.hpp"

Sample operator>>(Sample b, Effect& a)
{
    return a.Apply(b);
}