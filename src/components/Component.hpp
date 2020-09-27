#pragma once
struct Component 
{
	virtual double NextSample() = 0;
	virtual double Sample() = 0;
};