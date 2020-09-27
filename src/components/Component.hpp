#pragma once
#include "../utils/Utils.hpp"
struct Component 
{
	virtual Sample NextSample() = 0;
	Sample operator++() { return NextSample(); }
	virtual Sample GetSample() = 0;
};