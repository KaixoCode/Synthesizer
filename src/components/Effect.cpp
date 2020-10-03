#include "Effect.hpp"
#include <array>

Sample operator>>(Sample b, Effect& a)
{
    return a.Apply(b);
}

Stereo operator>>(Sample b, StereoEffect& a)
{
    Stereo ef{ a.left.Apply(b), a.right.Apply(b) };
    return ef;
}

Stereo operator>>(Stereo& b, StereoEffect& a)
{
    Stereo ef{ a.left.Apply(b.left), a.right.Apply(b.right) };
    return ef;
}
