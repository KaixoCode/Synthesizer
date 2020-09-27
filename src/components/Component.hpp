#pragma once
#include "../utils/Utils.hpp"
struct Component 
{
	virtual Sample NextSample() = 0;
	virtual Sample GetSample() = 0;
};