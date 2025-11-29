#pragma once
#include <Arduino.h>


class IClockSource
{
public:
	virtual void Init() = 0;
	virtual void Pump() = 0;
	virtual bool Running() = 0;
	virtual uint8_t GetHours() = 0;
	virtual uint8_t GetMins() = 0;
	virtual uint8_t GetSecs() = 0;

	virtual uint8_t GetDate() = 0;
	virtual uint8_t GetMonth() = 0;
	virtual uint8_t GetYear() = 0;

	virtual uint8_t GetYearDay() = 0;
	virtual uint8_t GetDayOfWeek() = 0;

	virtual void SetHour(int hour) = 0;
	virtual void SetMinute(int minute) = 0;

};
