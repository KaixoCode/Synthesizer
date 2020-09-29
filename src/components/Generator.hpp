#pragma once
#include "Component.hpp"

struct Generator : public Component
{
	virtual Sample NextSample() = 0;
	Sample operator++() { return NextSample(); }
	virtual Sample GetSample() = 0;
};