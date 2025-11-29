#pragma once
#include <TimeLib.h>
#include "IClockSource.h"

class NTPClockSource : public IClockSource
{
public:
	void SetHour(int hour) { ; } 
	void SetMinute(int minute) { ;  }
	// Inherited via IClockSource
	virtual uint8_t GetHours() override;
	virtual uint8_t GetMins() override;
	virtual uint8_t GetSecs() override;
	virtual uint8_t GetDate() override;
	virtual uint8_t GetMonth() override;
	virtual uint8_t GetYear() override;
	virtual uint8_t GetYearDay() override;
	virtual uint8_t GetDayOfWeek() override;

	// Inherited via IClockSource
	virtual void Init() override;
	virtual void Pump() override;
	virtual bool Running() override;
	NTPClockSource();
private:
	time_t m_Now;
	uint8_t m_LastHour;
	uint8_t m_LastLocalHour;
	static const uint8_t m_InvalidHour = 0xff;
	bool m_InitDone;
	uint8_t m_LastHours;
};