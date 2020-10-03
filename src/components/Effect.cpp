#include "Effect.hpp"
#include <array>

Sample operator>>(Sample b, Effect& a)
{
    return a.Apply(b);
}

Stereo operator>>(Sample b, const StereoEffect& a)
{
    return { a.left.Apply(b), a.right.Apply(b) };
}

Stereo operator>>(Stereo& b, const StereoEffect& a)
{
    return { a.left.Apply(b.left), a.right.Apply(b.right) };
}
