#pragma once
#include <Arduino.h>

class IClockSource;

class IClock
{
public:
	virtual void Init(IClockSource* clock) = 0;
	virtual void Pump() = 0;

};
